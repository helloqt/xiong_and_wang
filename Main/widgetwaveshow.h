#ifndef WIDGETWAVESHOW_H
#define WIDGETWAVESHOW_H

#include <QWidget>
#include "ui_widgetwaveshow.h"
#include "wavewidget/wavewidget.h"
class widgetWaveShow : public QWidget, public Ui::widgetWaveShow
{
    Q_OBJECT
    
public:
    explicit widgetWaveShow(QWidget *parent = 0);
    ~widgetWaveShow();
    
    WaveWidget *waveWidget;
};

#endif // WIDGETWAVESHOW_H
