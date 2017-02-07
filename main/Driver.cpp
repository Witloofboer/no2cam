#include "Driver.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QMessageBox>
#include <QThread>
#include <string>
#include "Generator.h"
#include <math.h>
#include "windows.h"

//------------------------------------------------------------------------------

Driver::Driver(const Generator *generator)
    : core::PowerDriver()
    , _generator(generator)
{
    _serial = new QSerialPort(this);
    _serial->setPortName("COM12"); // select the correct port to send the data to...
    _serial->setBaudRate(QSerialPort::Baud9600);
    _serial->setDataBits(QSerialPort::Data8);
    _serial->setStopBits(QSerialPort::OneStop);
    _serial->setParity(QSerialPort::NoParity);
    _serial->setFlowControl(QSerialPort::NoFlowControl);
    _serial->open(QSerialPort::ReadWrite);
    connect(_serial, QSerialPort::readyRead, this, Driver::serialReceived);
}

void Driver::serialReceived()
{
    QMessageBox msgBox;
    QByteArray receivedBytes;
    receivedBytes =_serial->readAll();
    QString received;
    for (auto it=receivedBytes.begin(); it!=receivedBytes.end(); ++it)
    {
        printf("%02x ", static_cast<quint8>(*it));
//        received += QString("%1").arg(static_cast<quint8>(*it), 2, 16, '0');
    }
    printf("\n");
//    msgBox.setText("Received: " + received);
//    msgBox.exec();
}

//------------------------------------------------------------------------------

void Driver::writeDDS(double power)
{
    qInfo("<power: %.1f mW>", power);

    const double per_mW = 1.0;
    const double per_MHz = 4294967.296;

    QByteArray stream(29, 0);
    stream[4] = 0x02;
    stream[5] = 0x01;
    stream[7] = 0x40;
    stream[8] = 0x08;
    stream[9] = 0x20;
    stream[10] = 0x02;
    stream[11] = 0x1d;
    stream[12] = 0x3f;
    stream[13] = 0x41;
    stream[14] = 0x50;
    stream[15] = 0x03;
    stream[19] = static_cast<quint8> (round(power * per_mW));
    stream[20] = 0x0e;
    stream[21] = 0x3f;
    stream[22] = 0xff;

    // lineair correction formula is needed to correct the frequency offset!
    int freq = round((_generator->getFrequency() * per_MHz)+(224.71*_generator->getFrequency()+263.83));

    stream[28] = freq & 0x000000ff;
    stream[27] = (freq & 0x0000ff00)>>8;
    stream[26] = (freq & 0x00ff0000)>>16;
    stream[25] = (freq & 0xff000000)>>24;

    _serial->write(stream);
    QThread::msleep(2);
}

//------------------------------------------------------------------------------

void Driver::writePLL()
{

    const double per_MHz = 1e6;
    const double VREF = 40;
    double VCO_freq = 1040;    //choose: NIR=1472, UV=1040, VIS=1152

    VCO_freq = 8*(_generator->getFrequency());

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
    functionLatch[0] = functionLatch[0] | (prescalerIndex<<6);
    functionLatch[0] = functionLatch[0] | (powerdown2<<5);
    functionLatch[0] = functionLatch[0] | (CPset2<<2);
    functionLatch[0] = functionLatch[0] | (CPset1>>1);

    functionLatch[1] = functionLatch[1] | (CPset1<<7);
    functionLatch[1] = functionLatch[1] | (timerCounter<<3);
    functionLatch[1] = functionLatch[1] | (fastLock<<1);
    functionLatch[1] = functionLatch[1] | (CPoutput);

    functionLatch[2] = functionLatch[2] | (pfdPolarity<<7);
    functionLatch[2] = functionLatch[2] | (muxOut<<4);
    functionLatch[2] = functionLatch[2] | (powerdown1<<3);
    functionLatch[2] = functionLatch[2] | (counterReset<<2) | 0b10;

    //set up the initialisation latch register
    initLatch[0] = functionLatch [0];
    initLatch[1] = functionLatch [1];
    initLatch[2] = functionLatch [2] | 0b1;

    //set up the r-Counter register
    rCounterLatch[0] = rCounterLatch[0] | (lockDetectprecision<<4);
    rCounterLatch[0] = rCounterLatch[0] | (abpw);

    rCounterLatch[1] = rCounterLatch[1] | (((rCounter<<2) & 0xff00)>>8);

    rCounterLatch[2] = rCounterLatch[2] | (((rCounter) & 0x00ff))<<2 | 0b00;

    //set up the ab-Counter register
    abCounterLatch[0] = abCounterLatch[0] | (CPgain<<5);
    abCounterLatch[0] = abCounterLatch[0] | ((bCounter)>>8);

    abCounterLatch[1] = abCounterLatch[1] | (bCounter& 0xff);

    abCounterLatch[2] = abCounterLatch[2] | (aCounter& 0x3f)<<2 | 0b01;

    //write the 4 registers to the selected serial port


    QByteArray dummy(3,0) ;

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

void Driver::setPower(double power)
{
    // chose one of the following options to send data to the correct device

    power = 400;

    if (power < 92)
    {
        power = 92;
    }
    if (power > 365)
    {
        power = 365;
    }
    double dummy_power = power;
    power = (0.000008*(pow(dummy_power,3))) - (0.0049*(pow(dummy_power,2))) + (1.3178*dummy_power) - 86.215;
    //writeDDS(power);
    writePLL();
}

//------------------------------------------------------------------------------