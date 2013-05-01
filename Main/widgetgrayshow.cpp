#include "widgetgrayshow.h"


widgetGrayShow::widgetGrayShow(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    grayWidget = new GrayWidget;
    layoutGrayShow->addWidget(grayWidget);

    grayWidget->setGraduation(0, 400);
}

widgetGrayShow::~widgetGrayShow()
{
    delete grayWidget;
}
