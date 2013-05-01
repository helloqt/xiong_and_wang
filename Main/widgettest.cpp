#include "widgettest.h"


widgetTest::widgetTest(QWidget *parent) :
    QWidget(parent)
{
   setupUi(this);

   XWidget = new SamplingWidget;
   XxWidget = new SamplingWidget;
   YWidget = new SamplingWidget;
   YyWidget = new SamplingWidget;
   startDlg = new testViewStartDlg;
   finishDlg = new testViewFinishDlg;
   layoutX->addWidget(XWidget);
   layoutXx->addWidget(XxWidget);
   layoutY->addWidget(YWidget);
   layoutYy->addWidget(YyWidget);

   connect(XWidget, SIGNAL(valueChanged(int, int)), this, SLOT(XValueChange(int, int)));
   connect(XxWidget, SIGNAL(valueChanged(int, int)), this, SLOT(XxValueChange(int, int)));
   connect(YWidget, SIGNAL(valueChanged(int, int)), this, SLOT(YValueChange(int, int)));
   connect(YyWidget, SIGNAL(valueChanged(int, int)), this, SLOT(YyValueChange(int, int)));
}

widgetTest::~widgetTest()
{

}


/*******************************************
  *类型：公有函数
  *功能：设置X波形显示的数据
        更新数据后自动覆盖上一次的数据
  ******************************************/
void widgetTest::updataXData(QList<qint16> data)
{
    XWidget->updateDate(data);
}

/*******************************************
  *类型：公有函数
  *功能：设置Y波形显示的数据
        更新数据后自动覆盖上一次的数据
  ******************************************/
void widgetTest::updataYData(QList<qint16> data)
{
    YWidget->updateDate(data);
}

/*******************************************
  *类型：公有函数
  *功能：设置X'波形显示的数据
        更新数据后自动覆盖上一次的数据
  ******************************************/
void widgetTest::updataXxData(QList<qint16> data)
{
    XxWidget->updateDate(data);
}

/*******************************************
  *类型：公有函数
  *功能：设置Y'波形显示的数据
        更新数据后自动覆盖上一次的数据
  ******************************************/
void widgetTest::updataYyData(QList<qint16> data)
{
    YyWidget->updateDate(data);
}

/*******************************************
  *类型：公有函数
  *功能：得到XWidget中用户选择的值
        返回值1--400
  ******************************************/
int widgetTest::getXValue()
{
    return XWidget->getValue();
}

/*******************************************
  *类型：公有函数
  *功能：得到YWidget中用户选择的值
        返回值1--400
  ******************************************/
int widgetTest::getYValue()
{
    return YWidget->getValue();
}

/*******************************************
  *类型：公有函数
  *功能：得到XpWidget中用户选择的值
        返回值1--400
  ******************************************/
int widgetTest::getXxValue()
{
    return XxWidget->getValue();
}

/*******************************************
  *类型：公有函数
  *功能：得到YpWidget中用户选择的值
        返回值1--400
  ******************************************/
int widgetTest::getYyValue()
{
    return YyWidget->getValue();
}

void widgetTest::SetProfile(int profileIndex)
{
    switch(profileIndex)
    {
    case 0:
        widget1->show();
        widget2->show();
        widget3->hide();
        widget4->hide();
        break;
    case 1:
        widget1->hide();
        widget2->hide();
        widget3->show();
        widget4->show();
        break;
    case 2:
        widget1->show();
        widget2->show();
        widget3->show();
        widget4->show();
        break;
    default:
        widget1->show();
        widget2->show();
        widget3->show();
        widget4->show();
        break;
    }
}

/*******************************************
  *类型：公有函数
  *功能：设置显示时的增益
  ******************************************/
void widgetTest::setScale(qreal value)
{
    XWidget->setScale(value);
    YWidget->setScale(value);
    XxWidget->setScale(value);
    YyWidget->setScale(value);
}

/*******************************************
  *类型：公有函数
  *功能：设置所有波形显示的数据
        更新数据后自动覆盖上一次的数据
  ******************************************/
void widgetTest::updataData(MyDataNode &data)
{
    QList<qint16 > list;
    for(int i=0;i<4;i++)
        for(int j=0;j<400;j++)
            list.append(data.data[i][j]);
    //0-400 X
    XWidget->updateDate(list.mid(0,WAVE_LENGHT_VALUE));
    //400-800 Y
    YWidget->updateDate(list.mid(WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
    //800-1200 X'
    XxWidget->updateDate(list.mid(2*WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
    //1200-1600 Y'
    YyWidget->updateDate(list.mid(3*WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
}

void widgetTest::updataData(QList<qint16> listData)
{
    //0-400 X
    XWidget->updateDate(listData.mid(0,WAVE_LENGHT_VALUE));
    //400-800 Y
    YWidget->updateDate(listData.mid(WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
    //800-1200 X'
    XxWidget->updateDate(listData.mid(2*WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
    //1200-1600 Y'
    YyWidget->updateDate(listData.mid(3*WAVE_LENGHT_VALUE-1,WAVE_LENGHT_VALUE));
}

void widgetTest::XValueChange(int valueIndex, int value)
{
    labelXT->setText("T(us):" + QString::number(valueIndex));
    labelXA->setText("A(mV):" + QString::number(value));
    labelXValueIndex->setText("信号读时:" + QString::number(valueIndex));
}

void widgetTest::XxValueChange(int valueIndex, int value)
{
    labelXxT->setText("T(us):" + QString::number(valueIndex));
    labelXxA->setText("A(mV):" + QString::number(value));
    labelXxValueIndex->setText("信号读时:" + QString::number(valueIndex));
}
void widgetTest::YValueChange(int valueIndex, int value)
{
    labelYT->setText("T(us):" + QString::number(valueIndex));
    labelYA->setText("A(mV):" + QString::number(value));
    labelYValueIndex->setText("信号读时:" + QString::number(valueIndex));
}
void widgetTest::YyValueChange(int valueIndex, int value)
{
    labelYyT->setText("T(us):" + QString::number(valueIndex));
    labelYyA->setText("A(mV):" + QString::number(value));
    labelYyValueIndex->setText("信号读时:" + QString::number(valueIndex));
}

void widgetTest::on_btnPreX_clicked()
{
    XWidget->valueIndex--;
    if (XWidget->valueIndex <= 0)
        XWidget->valueIndex = 1;

    XValueChange(XWidget->valueIndex, XWidget->m_data.at(XWidget->valueIndex - 1));
    XWidget->update();
}

void widgetTest::on_btnNextX_clicked()
{
    XWidget->valueIndex++;
    if (XWidget->valueIndex > POINTCOUNT)
        XWidget->valueIndex = POINTCOUNT;

    XValueChange(XWidget->valueIndex, XWidget->m_data.at(XWidget->valueIndex - 1));
    XWidget->update();
}

void widgetTest::on_btnPreXx_clicked()
{
    XxWidget->valueIndex--;
    if (XxWidget->valueIndex <= 0)
        XxWidget->valueIndex = 1;

    XxValueChange(XxWidget->valueIndex, XxWidget->m_data.at(XxWidget->valueIndex - 1));
    XxWidget->update();
}

void widgetTest::on_btnNextXx_clicked()
{
    XxWidget->valueIndex++;
    if (XxWidget->valueIndex > POINTCOUNT)
        XxWidget->valueIndex = POINTCOUNT;

    XxValueChange(XxWidget->valueIndex, XxWidget->m_data.at(XxWidget->valueIndex - 1));
    XxWidget->update();
}

void widgetTest::on_btnPreY_clicked()
{
    YWidget->valueIndex--;
    if (YWidget->valueIndex <= 0)
        YWidget->valueIndex = 1;

    YValueChange(YWidget->valueIndex, YWidget->m_data.at(YWidget->valueIndex - 1));
    YWidget->update();
}

void widgetTest::on_btnNextY_clicked()
{
    YWidget->valueIndex++;
    if (YWidget->valueIndex > POINTCOUNT)
        YWidget->valueIndex = POINTCOUNT;

    YValueChange(YWidget->valueIndex, YWidget->m_data.at(YWidget->valueIndex - 1));
    YWidget->update();
}

void widgetTest::on_btnPreYy_clicked()
{
    YyWidget->valueIndex--;
    if (YyWidget->valueIndex <= 0)
        YyWidget->valueIndex = 1;

    YyValueChange(YyWidget->valueIndex, YyWidget->m_data.at(YyWidget->valueIndex - 1));
    YyWidget->update();
}

void widgetTest::on_btnNextYy_clicked()
{
    YyWidget->valueIndex++;
    if (YyWidget->valueIndex > POINTCOUNT)
        YyWidget->valueIndex = POINTCOUNT;

    YyValueChange(YyWidget->valueIndex, YyWidget->m_data.at(YyWidget->valueIndex - 1));
    YyWidget->update();
}
