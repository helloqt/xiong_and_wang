#include "UserDef.h"
#ifndef MYPORT_H
#define MYPORT_H


#include <QString>
#include <QObject>
#include "UserDef.h"
#include <QTimer>
#include <QWaitCondition>
#include <QMutex>
#include <QByteArray>
#include <QTime>
#include "qextserialport.h"
#include "heightcom/heightcom.h"

#define WAITFORCOMMAND 0
#define WAITFORSTART 1
#define WAITFORSTOP 2
#define WAITFORSETRADIUS 3
#define WAITFORSETDENSITY 4
#define WAITFORSETTVG 5
#define WAITFORDATA 6
#define WAITNOTHING 7

#define RESULT_OK 0
#define RESULT_CANTCOMMAND 1
#define RESULT_COMMANDERROR 2
#define RESULT_RESPONDERROR 3
#define RESULT_PORTNOOPEN 4

#define USEEVENT

class MyPort : public QObject
{
    Q_OBJECT;
public:
    MyPort(void);
    ~MyPort(void);

public:
    /************************************************************************/
    /* 操作函数                                                                     */
    /************************************************************************/
    bool OpenPort(QString portName);
    bool OpenHeightPort(QString portName);
    bool SetBaudPart(BaudRateType baudRate);
    bool ClostPort();
    void CloseHeightPort();
    bool IsOpen() { return serialPort->isOpen(); }
    bool IsHeightPortOpen() {return heightPort->IsOpen();}
    qreal GetCurrentHeight();
    int StartSampling();
    int StopSampling();
    int Command(quint8 command, quint8 parameter);
    int SetRadius(quint8 parameter);
    int SetDensity(quint8 parameter);
    int SetTVG(quint8 parameter);


signals:
    void SamplingIsStarted();
    void SamplingIsStopped();
    void FinishThisRecv(MyDataNode&);
    void StartNextRecv();

    void CommandSucceed(QByteArray&);

private:
    void Init();
    void MatchCommand(QByteArray &command);
private slots:
    void onDataAvailable();


    /************************************************************************/
    /* 变量                                                                     */
public:
    HeightCom *heightPort;
    QextSerialPort *serialPort;
private:
    MyDataNode node;
#ifdef USEPOLLING
    QTimer *recvTimer;
#endif

    QMutex mutexForRespond;
    QWaitCondition waitForRespond;

    int portState;
    int result;

    QByteArray dataHeaderBuffer;
    int headerAACount;
    QByteArray recvDataBuffer;
    QByteArray thisCommand;

#ifdef MY_DEBUG
    QTime time;
#endif
    /************************************************************************/
};


#endif // MYPORT_H
