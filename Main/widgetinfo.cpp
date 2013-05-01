#include "widgetinfo.h"

#include <QTime>
widgetInfo::widgetInfo(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    historyCount = 0;
    speed = 0;
}

widgetInfo::~widgetInfo()
{

}


void widgetInfo::SetSpeed(qreal speed)
{
    labelSpeed->setText(QString::number(speed));
}

void widgetInfo::SetHeight(qreal height)
{
    labelHeight->setText(QString::number(height));
}

void widgetInfo::SetGain(qint32 scale)
{
    spinScale->setValue(scale);
}

void widgetInfo::SetState(QString state)
{
    labelState->setText(state);
}

void widgetInfo::SetDataPort(QString portName)
{
    if (portName.count() > 4)
        portName = portName.remove(0, 5);
    labelDataPort->setText("数据串口：" + portName);
}

void widgetInfo::SetHeightPort(QString portName)
{
    if (portName.count() > 4)
        portName = portName.remove(0, 5);
    labelHeightPort->setText("高度串口：" + portName);
}

void widgetInfo::SetPointCount(QString pointCount)
{
    labelPointCount->setText("当前采样点数：" + pointCount);
}

int widgetInfo::GetShowMode()
{
    return comboShowMode->currentIndex();
}

int widgetInfo::GetShowProfile()
{
    return comboProfile->currentIndex();
}

double widgetInfo::GetGain()
{
    return spinScale->value();
}

int widgetInfo::GetWaveCount()
{
    return spinBoxWaveCount->value();
}

void widgetInfo::AppendHistoryInfo(QString info)
{

    if (historyCount > 100)
    {
        historyCount = 0;
        historyInfo->clear();
    }
    historyInfo->append(QTime::currentTime().toString() + ":");
    historyInfo->append(info);
    historyCount++;
}

qreal widgetInfo::GetSpeed()
{
    return speed;
}
