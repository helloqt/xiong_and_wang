#include "./myport/myport.h"
#include <QByteArray>
#include <QTime>
MyPort::MyPort(void)
{
#ifdef USEPOLLING
    recvTimer = new QTimer(this);
#endif
    Init();
}

MyPort::~MyPort()
{

}

void MyPort::Init()
{

    serialPort = new QextSerialPort;
#ifdef USEEVENT
    serialPort->setQueryMode(QextSerialPort::EventDriven);
#endif
#ifdef USEPOLLING
    serialPort->setQueryMode(QextSerialPort::Polling);
#endif

    serialPort->setBaudRate(BAUD115200); //BaudRate
    serialPort->setDataBits(DATA_8); //DataBits
    serialPort->setParity(PAR_NONE); //Parity
    serialPort->setStopBits(STOP_1); //StopBits
    serialPort->setFlowControl(FLOW_OFF);     //数据流控制设置
    serialPort->setTimeout(5);     //延时设置，
#ifdef USEEVENT
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
#endif

#ifdef USEPOLLING
    connect(recvTimer, SIGNAL(timeout()), this, SLOT(onDataAvailable()));

#endif

    heightPort = new HeightCom;
    portState = WAITNOTHING;
    headerAACount = 0;
}

void MyPort::MatchCommand(QByteArray &command)
{

    if ( (quint8)~command.at(0) == (quint8)thisCommand.at(0) &&
         (quint8)~command.at(1) == (quint8)thisCommand.at(1))
    {
        QByteArray tempCommand(thisCommand);
        emit this->CommandSucceed(tempCommand);
        if ((quint8)~command.at(0) == 0x01)
            emit this->SamplingIsStarted();
        if ((quint8)~command.at(0) == 0x02)
            emit this->SamplingIsStopped();
    }
    this->thisCommand.clear();
}

bool MyPort::OpenPort(QString portName)
{

    if (serialPort->isOpen())
    {
        serialPort->close();
    }

    serialPort->setPortName(portName);
    if (!serialPort->open(QIODevice::ReadWrite))//以读写方式打开串口
    {
#ifdef MY_DEBUG
        qDebug() << "Open" << portName << "failed";
#endif
        return false;
    }

#ifdef USEPOLLING
    recvTimer->start(20);
#endif

    recvDataBuffer.clear();
    return true;
}

bool MyPort::OpenHeightPort(QString portName)
{
    return heightPort->openPort(portName);
}

bool MyPort::SetBaudPart(BaudRateType baudRate)
{
    serialPort->setBaudRate(baudRate);
}

bool MyPort::ClostPort()
{
    if (serialPort->isOpen())
    {
        serialPort->close();
    }
    return true;
}

void MyPort::CloseHeightPort()
{
    heightPort->Close();
}

qreal MyPort::GetCurrentHeight()
{
   return heightPort->getHeight();
}

int MyPort::Command(quint8 command, quint8 parameter)
{

    thisCommand.clear();
    thisCommand.append(command);
    thisCommand.append(parameter);
    switch (command)
    {
    case 0x01:
        return StartSampling();
    case 0x02:
        return StopSampling();
    case 0x03:
        return SetRadius(parameter);
    case 0x04:
        return SetDensity(parameter);
    case 0x05:
        return SetTVG(parameter);
    default:
        break;
    }
    return RESULT_COMMANDERROR;
}

int MyPort::StartSampling()
{

    if (serialPort->isOpen())
    {
        if (portState != WAITNOTHING)
            return RESULT_CANTCOMMAND;

        thisCommand.clear();
        thisCommand.append((quint8)0x01);
        thisCommand.append((char)0x00);
        serialPort->write(thisCommand);

        return RESULT_OK;
    }
    return RESULT_CANTCOMMAND;
}

int MyPort::StopSampling()
{
    if (serialPort->isOpen())
    {
        if (portState != WAITNOTHING)
            return RESULT_CANTCOMMAND;

        thisCommand.clear();
        thisCommand.append((quint8)0x02);
        thisCommand.append((char)0x00);
        serialPort->write(thisCommand);

        return RESULT_OK;
    }

    return RESULT_CANTCOMMAND;
}

int MyPort::SetRadius(quint8 parameter)
{

    if (serialPort->isOpen())
    {
        if (portState != WAITNOTHING)
            return RESULT_CANTCOMMAND;

        thisCommand.clear();
        thisCommand.append((quint8)0x03);
        thisCommand.append((quint8)parameter);
        serialPort->write(thisCommand);

        return RESULT_OK;
    }
    return RESULT_CANTCOMMAND;
}
int MyPort::SetDensity(quint8 parameter)
{
    if (serialPort->isOpen())
    {
        if (portState != WAITNOTHING)
            return RESULT_CANTCOMMAND;

        thisCommand.clear();
        thisCommand.append((quint8)0x04);
        thisCommand.append((quint8)parameter);
        serialPort->write(thisCommand);

        return RESULT_OK;
    }
    return RESULT_CANTCOMMAND;
}

int MyPort::SetTVG(quint8 parameter)
{

    if (serialPort->isOpen())
    {
        if (portState != WAITNOTHING)
            return RESULT_CANTCOMMAND;

        thisCommand.clear();
        thisCommand.append((quint8)0x05);
        thisCommand.append((quint8)parameter);
        serialPort->write(thisCommand);

        return RESULT_OK;
    }
    return RESULT_CANTCOMMAND;
}

void MyPort::onDataAvailable()
{

    QByteArray temp = serialPort->readAll();

    if (!temp.count())
        return;
#ifdef MY_DEBUG
    qDebug() << "DataCount is " << temp.count();
#endif
    if (temp.count() == 2 && thisCommand.count() != 0)
    {
        MatchCommand(temp);
        return;
    }

    recvDataBuffer.append(temp);

    portState = WAITFORDATA;

    if (recvDataBuffer.count() >= LOWSENDBYTECOUNT - 4)
    {
        int index = 0;

        for (index = 0; index < recvDataBuffer.count(); index++)
        {
            if ((quint8)recvDataBuffer.at(index) == 0xAA)
                headerAACount++;
            else
                headerAACount = 0;

            if (headerAACount == 8)
                break;
        }

        if (headerAACount == 8)
        {
            recvDataBuffer.remove(0, index + 1);
            headerAACount = 0;
#ifdef MY_DEBUG
            qDebug() << "start ok";
#endif
        }
#ifdef MY_DEBUG
        qDebug() << "after start, index is " << index;
        qDebug() << "recvDataBuffer count is " << recvDataBuffer.count();
#endif
        for (index = LOWSENDBYTECOUNT - 16 - 4; index < recvDataBuffer.count(); index++)
        {
            if ((quint8)recvDataBuffer.at(index) == 0xAA)
                headerAACount++;
            else
                headerAACount = 0;

            if (headerAACount == 8)
                break;
        }

        if (headerAACount == 8)
        {
            headerAACount = 0;
#ifdef MY_DEBUG
            qDebug() << "end ok";
#endif
            int section = 0;
            int height = 0;

            recvDataBuffer.remove(index - 7, 8);
            index -= 8;
            if (index % 2 == 0)
            {
                index--;
                recvDataBuffer.remove(0, 1);
            }

            for (int i = 0; i <= index; i++)
            {
                short dataTemp = (quint8)recvDataBuffer.at(i);
                dataTemp = dataTemp << 8;
                dataTemp = dataTemp + (quint8)recvDataBuffer.at(++i);
                this->node.data[section++][height] = dataTemp;
                if (section == 4)
                {
                    section = 0;
                    height++;
                }
            }

            portState = WAITNOTHING;
            node.height = heightPort->getHeight();
            emit FinishThisRecv(this->node);

            recvDataBuffer.remove(0, index + 1);
#ifdef MY_DEBUG
            qDebug() << "Port receive time is :" << time.elapsed();
#endif
        }
        else
        {
#ifdef MY_DEBUG
            qDebug() << "before clean recvDataBuffer, index is " << index;
            qDebug() << "clean recvDataBuffer";
#endif
            recvDataBuffer.clear();
            headerAACount = 0;
        }

    }
}
