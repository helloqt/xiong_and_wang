#ifndef HEIGHTCOM_H
#define HEIGHTCOM_H

#include <QObject>
#include <QTimer>
#include "qextserialport.h"

const static qreal  mmPerValue = 10;

class HeightCom : public QObject
{
    Q_OBJECT
public:
    explicit HeightCom(QObject *parent = 0);
    ~HeightCom();
    bool openPort(QString PortName);
    void beginToCount();
    void stopCount();
    qreal getHeight();
    bool IsOpen();
    void Close(){ m_Com->close();}
signals:

private slots:
    void readCom();
    void requireReturn();
private:
    QextSerialPort *m_Com;
    QTimer *m_read_timer;
    QTimer *m_write_timer;
    qreal byteArrayToHeight(QByteArray data);

    qreal m_height;
};

#endif // HEIGHTCOM_H
