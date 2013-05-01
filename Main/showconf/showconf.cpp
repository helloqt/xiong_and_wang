#include "showconf.h"
#include <QPainter>
#include <QStaticText>
#include <QIntValidator>
ShowConf::ShowConf(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    init();
}

ShowConf::~ShowConf()
{

}

void ShowConf::init()
{

    comboProfiles->setCurrentIndex(2);
    m_maxNum = 500;
    sliderGray->setValue(20);
    lEditWaveScale->setText("50");
    lEditWaveCount->setText("8");

}

void ShowConf::drawGrayDemo()
{
    QPainter painter(this);

    QString strNum;

    float rate = 0;

    QPoint leftTop(picGrayDemo->mapTo(this, QPoint(0, 0)));//获取控件在窗体中的坐标
    int x1 = leftTop.x();
    int y1 = leftTop.y();
    x1 = x1 + 50;

    QPoint rightButton(picGrayDemo->mapTo(this, QPoint(picGrayDemo->width(), picGrayDemo->height())));//获取控件在窗体中的坐标
    int x2 = rightButton.x();
    int y2 = rightButton.y() - 20;

    rate = (float)sliderGray->value() * (y2 - y1) / 100.0 / 2;

    strNum.sprintf("%d", (int)(sliderGray->value() * m_maxNum / 100.0));
    QStaticText strStatic1(strNum);
    strNum.sprintf("%d", -(int)(sliderGray->value() * m_maxNum / 100.0));
    QStaticText strStatic2(strNum);

    QString strH, strL;
    strH.sprintf("%d", m_maxNum);
    strL.sprintf("%d", -m_maxNum);
    QStaticText strStaticH(strH);

    QStaticText strStaticL(strL);


    painter.fillRect(x1, y1, x2 - x1, y2 - y1, Qt::white);

    QRect rectTop;

    rectTop.setRect(x1, y1, x2 - x1, (y2 - y1) / 2 - rate);
    QLinearGradient linearGradientTop(rectTop.left(), rectTop.top(), rectTop.left(), rectTop.bottom());
    linearGradientTop.setColorAt(0, Qt::black);
    linearGradientTop.setColorAt(1.0, Qt::white);

    painter.fillRect(rectTop, linearGradientTop);

    QRect rectButton;
    rectButton.setRect(x1, (y2 + y1) / 2 + rate, x2 - x1, (y2 - y1) / 2 - rate);
    QLinearGradient linearGradientButton(rectButton.left(), rectButton.top(), rectButton.left(), rectButton.bottom());
    linearGradientButton.setColorAt(0, Qt::white);
    linearGradientButton.setColorAt(1.0, Qt::black);


    painter.fillRect(rectButton, linearGradientButton);


    painter.drawStaticText(rectTop.left() - 30, rectTop.bottom(), strStatic1);
    painter.drawStaticText(rectTop.left() - 30, rectTop.top(), strStaticH);

    painter.drawStaticText(rectButton.left() - 30, rectButton.top(), strStatic2);
    painter.drawStaticText(rectButton.left() - 30, rectButton.bottom(), strStaticL);
}


int ShowConf::GetScale()
{
    return lEditWaveScale->text().toInt();
}


int ShowConf::GetShowMode()
{
    return comboShowMode->currentIndex();
}

int ShowConf::GetProfile()
{
    return comboProfiles->currentIndex();
}

int ShowConf::GetWaveCount()
{
    return lEditWaveCount->text().toInt();
}

void ShowConf::SetWaveCount(int waveCount)
{
    lEditWaveCount->setText(QString::number(waveCount));
}
void ShowConf::SetScale(int scale)
{
    lEditWaveScale->setText(QString::number(scale));
}

void ShowConf::SetShowMode(int mode)
{
    comboShowMode->setCurrentIndex(mode);
}

void ShowConf::SetProfile(int profile)
{
    comboProfiles->setCurrentIndex(profile);
}

void ShowConf::SetThresholdHeight(int maxNum)
{
    m_maxNum = maxNum;
}

int ShowConf::GetThresholdLow()
{
    return sliderGray->value() * m_maxNum / sliderGray->maximum();
}

int ShowConf::GetThresholdHeight()
{
    return m_maxNum;
}

void ShowConf::paintEvent(QPaintEvent *event)
{
    drawGrayDemo();
}



void ShowConf::on_radioDistance_released()
{
}

void ShowConf::on_radioTime_clicked()
{
}


void ShowConf::on_btnAccept_clicked()
{
    accept();
}

void ShowConf::on_btnReject_clicked()
{
    reject();
}

void ShowConf::on_sliderGray_valueChanged(int value)
{
    QString str;
    str.sprintf("%d", value);
    str += "%";
    labelGrayLimen->setText(str);


    QPoint leftTop(picGrayDemo->mapTo(this, QPoint(0, 0)));//获取控件在窗体中的坐标
    int x1 = leftTop.x();
    int y1 = leftTop.y();

    QPoint rightButton(picGrayDemo->mapTo(this, QPoint(picGrayDemo->width(), picGrayDemo->height())));//获取控件在窗体中的坐标
    int x2 = rightButton.x();
    int y2 = rightButton.y();

    update(x1, y1, x2 - x1, y2 - y1);
}

void ShowConf::on_btnThresholdPre_clicked()
{
    sliderGray->setValue(sliderGray->value() - 1);
}

void ShowConf::on_btnThresholdNext_clicked()
{
    sliderGray->setValue(sliderGray->value() + 1);
}

void ShowConf::on_btnScalePre_clicked()
{
    lEditWaveScale->setText(QString::number(lEditWaveScale->text().toInt() - 1));
}

void ShowConf::on_btnScaleNext_clicked()
{
    lEditWaveScale->setText(QString::number(lEditWaveScale->text().toInt() + 1));
}

void ShowConf::on_btnWaveCountPre_clicked()
{
    lEditWaveCount->setText(QString::number(lEditWaveCount->text().toInt() - 1));
}

void ShowConf::on_btnWaveCountNext_clicked()
{
    lEditWaveCount->setText(QString::number(lEditWaveCount->text().toInt() + 1));
}
