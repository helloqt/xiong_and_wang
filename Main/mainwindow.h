#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_mainwindow.h"
#include "widgetconf.h"
#include "widgetgrayshow.h"
#include "widgetinfo.h"
#include "widgettest.h"
#include "widgetwaveshow.h"
#include "myport/myport.h"
#include "myfile/myfile.h"
#include "UserDef.h"
#include <QTimer>
#include "finishshow.h"
#include "showconf/showconf.h"
#include <QWidget>
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    //*******************************************************
    //工具函数
    void Init();
    //void updateStatusBar();
    void AppendDataNode(MyDataNode &node);
    void AppendDataNode(QList<qint16> &listRow, qreal height);
    void UpdateThreshold();


    void WhenConfUi();
    void WhenConfFinishUi();
    void WhenTestUi();
    void WhenTestFinishUi();
    void WhenStartUi();
    void WhenFinishUi();

    bool IsStartSampling() { return isStartSampling;}

    bool OpenDataPort();
    bool OpenHeightPort();
private slots:
    void FinishDataRecv(MyDataNode&);

    void on_btnConf_clicked();

    void on_btnTest_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_ScaleChange(int);
    void on_WaveCountChange(int);
    void on_ShowModeChange(int);
    void on_ProfileChange(int);
    void on_ConfSave();

    void on_BrowseModeSelected(int mode);
//debug部分**************************************************************************************************************
    void on_btnSendCommand_clicked();

    void on_btnLoadFile_clicked();

    void on_btnShowNext_clicked();

    void on_btnAutoShow_clicked();

    void on_btnOpenPort_clicked();

    void on_btnOpenPortHeight_clicked();

    void on_btnSeekToNodeIndex_clicked();

    void on_btnTempFileSaveAs_clicked();

    void on_AutoShowTimer();
    void on_btnShowConf_clicked();

    void on_pushButton_clicked();

    void on_btnBrowseMode_clicked();

private:
    QTimer *autoShowTimer;
//debug部分**************************************************************************************************************
private:
    //Ui对象
    widgetConf *confShow;
    //widgetGrayShow *grayShow;
    widgetInfo *infoShow;
    widgetTest *testShow;
    widgetWaveShow *waveShow;
    FinishShow *finishShow;
    ShowConf *showConf;
    QWidget *widgetSpace;
    //自定义类对象
    MyPort *port;
    MyFile *fileLoad;
    MyFile *fileTemp;
    MyFile *fileData;


    //变量
    //保存原始数据
    QList<QList<qint16> > dataNodeListRaw;
    //保存阈值化数据
    QList<QList<qint16> > dataNodeListPro;
    //保存高度
    QList<qreal> dataNodeHeigth;

    //当前设置阈值
    qint16 thresholdLow, thresholdHeight;
    //链表数据中的阈值
    qint16 listThresholdLow, listThreholdHeight;


    bool isStartSampling;//为真，表示当前已开始数据接收；
    float currentHeight;
    int recvDataCount;

    int currentDataNodeIndex;
    int grayWidgetCurrentPage;
};

#endif // MAINWINDOW_H
