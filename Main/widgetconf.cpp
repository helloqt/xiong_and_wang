#include "widgetconf.h"
#include "UserDef.h"
widgetConf::widgetConf(QWidget *parent) :
    QWidget(parent)
{
   setupUi(this);
   SetFileName("aa.uds");
}

widgetConf::~widgetConf()
{

}

void widgetConf::on_btnConfSave_clicked()
{
    emit SaveConf();
}

void widgetConf::SetFileName(QString fileName)
{
    lEditFileName->setText(fileName);
}

QString widgetConf::GetDataPortName()
{
    return comboPortData->currentText();
}
BaudRateType widgetConf::GetDataBaud()
{
    switch (comboBaudData->currentIndex())
    {
    case 0:
        return BAUD9600;
    case 1:
        return BAUD115200;

#if defined(Q_OS_UNIX) || defined(qdoc)
    case 2:
        return BAUD230400;
#endif
    default:
        return BAUD115200;
    }
}
QString widgetConf::GetHeightPortName()
{
    return comboPortHeight->currentText();
}

BaudRateType widgetConf::GetHeightBaud()
{
    switch (comboBaudHeight->currentIndex())
    {
    case 0:
        return BAUD9600;
    default:
        return BAUD9600;
    }
}


//获取基本信息
QString widgetConf::GetProjectName()
{
    return lEditProjectName->text();
}

QString widgetConf::GetItemName()
{
   return lEditItemName->text();
}

int widgetConf::GetItemType()
{
    return comboItemType->currentIndex();
}

//获取配置参数
QString widgetConf::GetHoleDepth()
{
    return lEditHoleDepth->text();
}

QString widgetConf::GetHoleDiameter()
{
    return lEditHoleDiameter->text();
}

qreal widgetConf::GetSpeed()
{
    return lEditSpeed->text().toDouble();
}

qreal widgetConf::GetZeroSound()
{
    return lEditZeroSound->text().toDouble();
}

qreal widgetConf::GetDistance()
{
    return lEditDistance->text().toDouble();
}

qreal widgetConf::GetPointDistance()
{
    return lEditPointDistance->text().toDouble();
}

qreal widgetConf::GetDiameter()
{
    return lEditDiameter->text().toDouble();
}

//文件信息
QString widgetConf::GetFileName()
{
    return lEditFileName->text();
}
