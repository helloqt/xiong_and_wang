#ifndef WIDGETGRAYSHOW_H
#define WIDGETGRAYSHOW_H

#include <QWidget>
#include "ui_widgetgrayshow.h"
#include "graywidget/graywidget.h"

class widgetGrayShow : public QWidget, public Ui::widgetGrayShow
{
    Q_OBJECT
    
public:
    explicit widgetGrayShow(QWidget *parent = 0);
    ~widgetGrayShow();
    

    GrayWidget *grayWidget;
};

#endif // WIDGETGRAYSHOW_H
