#ifndef WIDGETCONF_H
#define WIDGETCONF_H

#include <QWidget>
#include "ui_widgetconf.h"
#include <QString>
#include "qextserialport.h"
class widgetConf : public QWidget, public Ui::widgetConf
{
    Q_OBJECT
    
public:
    explicit widgetConf(QWidget *parent = 0);
    ~widgetConf();

signals:
    void SaveConf();
private slots:
    void on_btnConfSave_clicked();

public:

    void SetFileName(QString fileName);
    QString GetDataPortName();
    BaudRateType GetDataBaud();
    QString GetHeightPortName();
    BaudRateType GetHeightBaud();

    //获取基本信息
    QString GetProjectName();
    QString GetItemName();
    int GetItemType();

    //获取配置参数
    QString GetHoleDepth();
    QString GetHoleDiameter();
    qreal GetSpeed();
    qreal GetZeroSound();
    qreal GetDistance();
    qreal GetPointDistance();
    qreal GetDiameter();

    //文件信息
    QString GetFileName();
};

#endif // WIDGETCONF_H
