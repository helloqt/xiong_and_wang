#ifndef SHOWCONF_H
#define SHOWCONF_H

#include <QDialog>


#include "ui_showconf.h"
class ShowConf : public QDialog, public Ui::ShowConf
{
    Q_OBJECT
    
public:
    explicit ShowConf(QWidget *parent = 0);
    ~ShowConf();
private:
    int m_maxNum;
private slots:

    void on_radioDistance_released();

    void on_radioTime_clicked();

    void on_btnAccept_clicked();

    void on_btnReject_clicked();


    void on_sliderGray_valueChanged(int value);

    void on_btnThresholdPre_clicked();

    void on_btnThresholdNext_clicked();

    void on_btnScalePre_clicked();

    void on_btnScaleNext_clicked();

    void on_btnWaveCountPre_clicked();

    void on_btnWaveCountNext_clicked();

private:
    void paintEvent(QPaintEvent *event);
    void init();

    void drawGrayDemo();
public:
    int GetScale();
    int GetShowMode();
    int GetProfile();

    int GetWaveCount();
    int GetThresholdLow();
    int GetThresholdHeight();

    void SetWaveCount(int waveCount);
    void SetScale(int scale);
    void SetShowMode(int mdoe);
    void SetProfile(int profile);
    void SetThresholdHeight(int maxNum);
};

#endif // SHOWCONF_H
