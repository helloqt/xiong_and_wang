#include "heightcom.h"
#include "UserDef.h"
#ifdef MY_DEBUG
#include <QDebug>
#endif

HeightCom::HeightCom(QObject *parent) :
    QObject(parent)
{
    m_Com = new QextSerialPort;
    m_read_timer = new QTimer(this);
    m_write_timer = new QTimer(this);
    connect(m_read_timer,SIGNAL(timeout()),this,SLOT(readCom()));
    connect(m_write_timer,SIGNAL(timeout()),this,SLOT(requireReturn()));
    m_height = 0;
}

HeightCom::~HeightCom()
{
    delete m_read_timer;
    delete m_write_timer;
    delete m_Com;
    m_Com->close();
}

/*******************************************
  *类型：公有函数
  *功能：以默认配置打开串口portName
        成功 return true 失败return false
  *****************************************/
bool HeightCom::openPort(QString PortName)
{
    if(m_Com->isOpen())
        m_Com->close();
    m_Com->setPortName(PortName);
    m_Com->setBaudRate(BAUD9600);
    m_Com->setDataBits(DATA_8);
    m_Com->setStopBits(STOP_1);
    m_Com->setParity(PAR_NONE);
    if(m_Com->open(QIODevice::ReadWrite))
    {
#ifdef MY_DEBUG
        qDebug()<<"Height Com open succeed!";
#endif
        m_read_timer->start(100);
        return true;
    }else
    {
        qDebug()<<"Height Com open failed!";
        return false;
    }
}

/******************************************
  *类型：槽函数
  *对应信号：读取计时器时间到
  *功能：读取串口数据
        并根据读到的数据实时更改m_height
        **********************************/
void HeightCom::readCom()
{
    if(m_Com->bytesAvailable()>0)
    {
        char check = m_Com->read(1).at(0);
        if(check=='$')
        {
            QByteArray data = m_Com->read(4);
            m_height = byteArrayToHeight(data);
        }
    }
}

/*******************************************
  *类型：共有函数
  *功能：向串口发送指令!A，计数清零，开始计数
  *****************************************/
void HeightCom::beginToCount()
{
    m_Com->write(QString("!A").toLatin1());
    m_write_timer->start(100);
}


/*******************************************
  *类型：共有函数
  *功能：向串口发送指令!Z，停止计数，计数清零
  *****************************************/
void HeightCom::stopCount()
{
    m_Com->write(QString("!Z").toLatin1());
}

/*******************************************
  *类型：私有函数
  *功能：向串口发送指令!R，请求深度数据
  *****************************************/
void HeightCom::requireReturn()
{
    m_Com->write(QString("!R").toLatin1());
}

/********************************************
  *类型：私有函数
  *功能：将读到的数据解密
        并转换成深度的实数型
        ************************************/
qreal HeightCom::byteArrayToHeight(QByteArray data)
{
    int number = 0;
    QByteArray key("0915");
    for(int i=0;i<data.size();i++)
    {
        number += (data.at(i) ^ key.at(i));
        number <<= 8;
    }
    return number*mmPerValue;
}
/********************************************
  *类型：公有函数
  *功能：返回当前深度值
        ************************************/
qreal HeightCom::getHeight()
{
    return m_height;
}

bool HeightCom::IsOpen()
{
    return m_Com->isOpen();
}
