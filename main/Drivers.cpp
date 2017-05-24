#include "Drivers.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <string>
#include <math.h>
#include <exception>
#include "windows.h"

//------------------------------------------------------------------------------
// BaseDriver
//------------------------------------------------------------------------------

BaseDriver::BaseDriver(QSerialPort *serial)
    :_serial(serial)
{
    _serial->setParent(this);
}

//------------------------------------------------------------------------------

BaseDriver *BaseDriver::getDriver()
{
    for(auto &portInfo: QSerialPortInfo::availablePorts())
    {
        qDebug() << "Port: " << portInfo.portName();
        qDebug() << "    Description: " << portInfo.description();
        qDebug() << "    Manufacturer: " << portInfo.manufacturer();
        qDebug() << "    Product identifier: " << portInfo.productIdentifier();
        qDebug() << "    Vendor identifier: " << portInfo.vendorIdentifier();

        if (    portInfo.description() == "USB Communication Port" &&
                portInfo.manufacturer() == "Microchip Technology. Inc." &&
                portInfo.productIdentifier() == 10 &&
                portInfo.productIdentifier() == 1240)
        {
            qDebug("Candidate driver board detected");

            QSerialPort *serial = new QSerialPort(portInfo);
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setParity(QSerialPort::NoParity);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            bool ok = serial->open(QSerialPort::ReadWrite);

            if (ok)
            {
                qDebug("Serial port succesfully opened");
            }
            else
            {
                qCritical("Failure to open serial port -> Ignoring it");
                delete serial;
                continue;
            }

            serial->write("\n");
            QString answer = QString::fromStdString(
                                 serial->readAll().toStdString()
                             );
            qDebug() << "Driver board identification: " << answer;

            if (answer == "DDS")
            {
                qDebug("DDS board succesfully detected");
                return new DdsDriver(serial);
            }
            else if (answer == "PLL")
            {
                qDebug("PLL board succesfully detected");
                return new PllDriver(serial);
            }
            else
            {
                qWarning("Unexpected board identification -> board ignored");
                delete serial;
                continue;
            }
        }
    }

    qCritical("No acoustic driver board detected");
    QMessageBox::critical(
        0,
        "Aborting",
        "<p><b>Failure</b>: no acoustic driver detected</p>");
    return 0;
}

//------------------------------------------------------------------------------
// DDS driver
//------------------------------------------------------------------------------

QVector<double> fromTo(double from, double to)
{
    QVector<double> result;

    for(double i=from; i<=to; ++i)
        result.push_back(i);

    return result;
}

QVector<double> _calibratedFreq(fromTo(125, 165));
QVector<double> _calibratedPower{18.0, 19.0, 20.0, 21.0, 21.5};
QVector<QVector<double>> _calibratedStepsCrystal1{
    {16,  29,  44,  62,  71},
    {16,  29,  44,  61,  70},
    {15,  28,  42,  60,  70},
    {14,  27,  41,  59,  68},
    {14,  27,  40,  58,  67},
    {13,  26,  40,  57,  66},
    {12,  26,  39,  56,  65},
    {11,  24,  38,  55,  64},
    {11,  23,  37,  54,  63},
    {11,  23,  37,  54,  63},
    {11,  24,  37,  54,  63},
    {12,  24,  39,  56,  64},
    {14,  27,  41,  58,  67},
    {16,  29,  44,  61,  70},
    {19,  33,  48,  66,  76},
    {22,  37,  52,  71,  81},
    {26,  41,  57,  76,  86},
    {32,  47,  63,  84,  94},
    {34,  50,  66,  87,  97},
    {37,  53,  71,  91, 102},
    {40,  57,  74,  95, 107},
    {44,  60,  79, 100, 112},
    {45,  62,  80, 102, 114},
    {47,  64,  83, 105, 117},
    {49,  66,  85, 107, 119},
    {50,  68,  87, 110, 122},
    {51,  69,  88, 111, 123},
    {51,  69,  88, 111, 123},
    {50,  68,  87, 110, 122},
    {49,  66,  85, 107, 119},
    {46,  63,  81, 104, 115},
    {42,  59,  77,  98, 110},
    {38,  54,  72,  92, 103},
    {34,  49,  66,  85,  96},
    {33,  49,  66,  85,  96},
    {35,  51,  68,  88,  99},
    {42,  58,  76,  97, 109},
    {54,  71,  91, 114, 126},
    {67,  87, 108, 132, 146},
    {80, 101, 124, 151, 165},
    {94, 117, 141, 170, 185}};

QVector<QVector<double>> _calibratedStepsCrystal2{
    { 14,  27,  45,  62,  69},
    { 13,  26,  44,  61,  68},
    { 13,  25,  43,  60,  66},
    { 11,  22,  41,  58,  64},
    { 10,  22,  39,  56,  62},
    {  9,  21,  38,  54,  60},
    {  7,  19,  36,  52,  59},
    {  6,  18,  35,  50,  57},
    {  5,  17,  33,  49,  55},
    {  5,  16,  33,  48,  54},
    {  5,  16,  33,  48,  55},
    {  5,  17,  34,  50,  56},
    {  7,  19,  36,  51,  58},
    {  9,  21,  39,  56,  61},
    { 12,  25,  43,  60,  65},
    { 16,  28,  47,  64,  70},
    { 19,  31,  50,  67,  74},
    { 21,  34,  53,  71,  78},
    { 23,  36,  56,  74,  81},
    { 25,  38,  57,  75,  83},
    { 25,  38,  57,  76,  84},
    { 24,  37,  57,  75,  83},
    { 23,  35,  54,  72,  80},
    { 19,  32,  50,  68,  75},
    { 14,  26,  44,  61,  68},
    {  9,  20,  37,  53,  60},
    {  2,  13,  28,  44,  50},
    {  0,   7,  22,  37,  43},
    {  0,   6,  21,  35,  42},
    {  0,  11,  26,  42,  48},
    { 10,  22,  38,  55,  63},
    { 22,  35,  54,  73,  81},
    { 36,  50,  71,  92, 101},
    { 48,  64,  87, 109, 119},
    { 60,  77, 102, 126, 138},
    { 71,  89, 116, 142, 155},
    { 81,  98, 128, 156, 170},
    { 91, 110, 141, 171, 186},
    {101, 120, 154, 185, 200},
    {110, 131, 166, 199, 214},
    {121, 143, 179, 214, 230}};

DdsDriver::DdsDriver(QSerialPort *serial)
    : BaseDriver(serial)
    , _stream(29, 0)
    , _interpolator(_calibratedFreq,
                    _calibratedPower,
                    _calibratedStepsCrystal1)
{
    _stream[4] = 0x02;
    _stream[5] = 0x01;
    _stream[7] = 0x40;
    _stream[8] = 0x08;
    _stream[9] = 0x20;
    _stream[10] = 0x02;
    _stream[11] = 0x1d;
    _stream[12] = 0x3f;
    _stream[13] = 0x41;
    _stream[14] = 0x50;
    _stream[15] = 0x03;

    _stream[20] = 0x0e;
    _stream[21] = 0x3f;
    _stream[22] = 0xff;
}

//------------------------------------------------------------------------------

void DdsDriver::set(double frequency, double power)
{
    qDebug("<acoustic wave: %.3f MHz, %.1f mW>", frequency, power);

    if (frequency > 165)
    {
        qWarning("Too high frequency required: %.1f MHz (must be <= 165 MHz)",
                 frequency);
        throw std::domain_error(
            QString(
                "<p>Too high acoustic frequency. </p>"
                "<p>Requested: %1 MHz. Maximum allowed: 165 MHz.</p>")
            .arg(frequency, 0, 'f', 1).toStdString());
    }

    // chose one of the following options to send data to the correct device

    quint8 step;

    if (power == 0)
    {
        frequency = 232.61; //Filter cut-off
        step = 0;
    }
    else
    {
        //step = round(((8e-6*power-0.0049)*power+1.3178)*power-86.215);
        step = std::round(_interpolator(frequency, 10*std::log10(power)));
    }

    qDebug("<acoustic wave: step = %d>", step);

    const double per_MHz = 4294967.296;

    _stream[19] = step;

    // lineair correction formula is needed to correct the frequency offset!
    int freq = round(frequency*(per_MHz+ 224.71) + 263.83);

    _stream[28] = freq & 0x000000ff;
    _stream[27] = (freq & 0x0000ff00)>>8;
    _stream[26] = (freq & 0x00ff0000)>>16;
    _stream[25] = (freq & 0xff000000)>>24;

    _serial->write(_stream);
    _serial->flush();
}

//------------------------------------------------------------------------------
// PLL Driver
//------------------------------------------------------------------------------

PllDriver::PllDriver(QSerialPort *serial)
    : BaseDriver(serial)
{}

//------------------------------------------------------------------------------

void PllDriver::set(double frequency, double power)
{
    qDebug("<acoustic wave: %.3f MHz, %.1f mW>", frequency, power);

    // chose one of the following options to send data to the correct device

    if (power == 0)
    {
        frequency = 232.61; // This frequency is cut-off by a filter
    }

    const double per_MHz = 1e6;
    const double VREF = 40;
    double VCO_freq = 1040;    //choose: NIR=1472, UV=1040, VIS=1152

    VCO_freq = 8*(frequency);

    QByteArray functionLatch(3,0);
    QByteArray initLatch(3,0);
    QByteArray rCounterLatch(3,0);
    QByteArray abCounterLatch(3,0);


    double channelSpacing = 40000;      //choose: 40 to 100000
    double rfPFD = channelSpacing;

    quint8 prescalerIndex = 2;          //choose: 8/9=0, 16/17=1, 32/33=2, 64/65=3
    quint8 powerdown1 = 0;              //choose: Normal operation=0, Asynchronous power-down=1 ,Synchronous power-down=1
    quint8 powerdown2 = 0;              //choose: Normal operation=0, Asynchronous power-down=0 ,Synchronous power-down=1
    quint8 CPset1 = 0;                  //choose: 0.625mA=0, 1.25mA=1, 1.875mA=2, 2.5mA=3, 3.125mA=4, 3.75mA=5, 4.375mA=6, 5mA=7
    quint8 CPset2 = 0;                  //choose: 0.625mA=0, 1.25mA=1, 1.875mA=2, 2.5mA=3, 3.125mA=4, 3.75mA=5, 4.375mA=6, 5mA=7
    quint8 timerCounter = 7;            //choose: 3=0, 7=1, 11=2, 15=3, 19=4, 23=5, 27=6, 31=7, 35=8, 39=9, 43=10, 47=11, 51=12, 55=13, 59=14, 63=15
    quint8 fastLock = 0;                //choose: Disabled=0, Mode_1=1, Mode_2=2
    quint8 CPoutput = 0;                //choose: Normal=0, Three-state=1
    quint8 pfdPolarity = 0;             //choose: Negative=0, Positive=1
    quint8 muxOut = 1;                  //choose: Three-state output=0, Digital lock detect=1, N divider output=2, DVDD=3, R divider output=4, Analog lock detect=5, Serial data output=6, DGND=7
    quint8 counterReset = 0;            //choose: OFF=0, ON=1
    quint8 lockDetectprecision = 0;     //choose: 3cycles=0, 5cycles=1
    quint8 abpw = 0;                    //choose: 2.9ns=0, 1.3ns (TEST MODE ONLY)=1, 6.0ns=2, 2.9ns=3
    quint8 CPgain = 1;                  //choose: OFF=0, ON=1

    quint16 rCounter = (quint16)((VREF*per_MHz)/channelSpacing);
    int     nCounter = ((VCO_freq*per_MHz)/rfPFD);
    quint16 bCounter = (quint16)(nCounter/32);
    quint8  aCounter = (quint8)(nCounter%32);

    //set up the function latch register
    functionLatch[0] = (prescalerIndex<<6) | (powerdown2<<5) | (CPset2<<2) | (CPset1>>1);

    functionLatch[1] = (CPset1<<7)| (timerCounter<<3)| (fastLock<<1) | (CPoutput);

    functionLatch[2] = (pfdPolarity<<7) | (muxOut<<4) | (powerdown1<<3) | (counterReset<<2) | 2;

    //set up the initialisation latch register
    initLatch[0] = functionLatch [0];
    initLatch[1] = functionLatch [1];
    initLatch[2] = functionLatch [2] | 1;

    //set up the r-Counter register
    rCounterLatch[0] = (lockDetectprecision<<4) | abpw;

    rCounterLatch[1] = ((rCounter<<2) & 0xff00)>>8;

    rCounterLatch[2] = ((rCounter) & 0x00ff)<<2;

    //set up the ab-Counter register
    abCounterLatch[0] = (CPgain<<5) | ((bCounter)>>8);
    abCounterLatch[1] = bCounter & 0xff;
    abCounterLatch[2] = (aCounter & 0x3f)<<2 | 1;

    //write the 4 registers to the selected serial port
    _serial->write(initLatch);
    _serial->waitForBytesWritten(1);
    QThread::msleep(1);

    _serial->write(functionLatch);
    _serial->waitForBytesWritten(1);
    QThread::msleep(1);

    _serial->write(rCounterLatch);
    _serial->waitForBytesWritten(1);
    QThread::msleep(1);

    _serial->write(abCounterLatch);
    _serial->waitForBytesWritten(1);

    QThread::msleep(2);
}

//------------------------------------------------------------------------------
