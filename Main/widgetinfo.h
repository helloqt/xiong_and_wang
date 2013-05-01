#ifndef WIDGETINFO_H
#define WIDGETINFO_H

#include <QWidget>
#include "ui_widgetinfo.h"
#include <QString>
class widgetInfo : public QWidget, public Ui::widgetInfo
{
    Q_OBJECT
    
public:
    explicit widgetInfo(QWidget *parent = 0);
    ~widgetInfo();

public:
    void SetSpeed(qreal speed);
    void SetHeight(qreal height);
    void SetGain(qint32 grain);
    void SetState(QString state);
    void SetDataPort(QString portName);
    void SetHeightPort(QString portName);
    void SetPointCount(QString pointCount);


    int GetShowMode();
    int GetShowProfile();
    double GetGain();
    int GetWaveCount();
    void AppendHistoryInfo(QString info);
    qreal GetSpeed();
private:
    int historyCount;
    qreal speed;

};

#endif // WIDGETINFO_H
