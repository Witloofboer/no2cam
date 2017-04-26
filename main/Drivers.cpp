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

BaseDriver::BaseDriver(const QSerialPortInfo &portInfo)
    :_serial(new QSerialPort(portInfo, this))
{
    //_serial->setPortName("COM12"); // select the correct port to send the data to...
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setFlowControl(QSerialPort::NoFlowControl);
    _ok = _serial->open(QSerialPort::ReadWrite);
    if (_ok)
        qDebug("Success opening serial port of acousting driver");
    else
        qCritical("Failure opening serial port of acousting driver");
}

//------------------------------------------------------------------------------

BaseDriver *BaseDriver::getDriver()
{
    BaseDriver *driver = nullptr;

    for(auto &portInfo: QSerialPortInfo::availablePorts())
    {
        const auto description = portInfo.description();

        qDebug() << "Port " << portInfo.portName()
                 << ": " << description;

        if (description == "DDS Solution_-_-_-_-_-_-_")
        {
            qDebug("DDS driver detected");
            driver = new DdsDriver(portInfo);
            break;
        }
        else if (description == "PLL Solution_-_-_-_-_-_-_")
        {
            qDebug("PLL driver detected");
            driver = new PllDriver(portInfo);
            break;
        }
    }

    if (driver)
    {
        if (driver->_ok)
        {
            return driver;
        } else {
            qCritical("Failure to open the acoustic driver port");
            QMessageBox::critical(
                0,
                "Aborting",
                "<p><b>Success</b>: acoustic driver detected</p>"
                "<p><b>Failure</b>: acoustic driver port opening</p>");
            delete driver;
            return 0;
        }
    }
    else
    {
        qCritical("No acoustic driver detected");
        QMessageBox::critical(
            0,
            "Aborting",
            "<p><b>Failure</b>: no acoustic driver detected</p>");
        return 0;
    }

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
QVector<QVector<double>> _calibratedSteps{
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

DdsDriver::DdsDriver(const QSerialPortInfo &portInfo)
    : BaseDriver(portInfo)
    , _stream(29, 0)
    , _interpolator(_calibratedFreq, _calibratedPower, _calibratedSteps)
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

PllDriver::PllDriver(const QSerialPortInfo &portInfo)
    : BaseDriver(portInfo)
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
