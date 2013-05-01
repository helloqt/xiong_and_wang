#ifndef FINISHSHOW_H
#define FINISHSHOW_H

#include "ui_finishshow.h"
#include "graywidget/graywidget.h"
#include "wavewidget/wavewidget.h"
#include "browsemode.h"
class FinishShow : public QWidget, private Ui::FinishShow
{
    Q_OBJECT
    
public:
    explicit FinishShow(QWidget *parent = 0);
private slots:
    void on_btnPageUp_clicked();
    void on_btnPageDown_clicked();

public:
    void ShowMode(int showMode);
    void ShowGray();
    void ShowWave();
    void ShowBrowseMode();
    void ShowNextWave();
    void ShowPreviousWave();
    void UpdatePageBtn();
    void InitData(QList<QList<qint16> >* pData, QList<qreal>* pHeight);
public:
    GrayWidget *grayShow;
    WaveWidget *waveShow;
    BrowseMode *browseMode;
    int currentDataNodeStart;
    QList<QList<qint16> >* pDataNodeListRaw;
    QList<qreal>* pDataHeight;
};

#endif // FINISHSHOW_H
