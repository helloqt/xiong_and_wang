#include "widgetwaveshow.h"

widgetWaveShow::widgetWaveShow(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    waveWidget = new WaveWidget;
    waveWidget->setScale(0.1);
    waveWidget->SetProfile(2);
    waveWidget->setMaxNumber(400);
    layoutShowWave->addWidget(waveWidget);
}

widgetWaveShow::~widgetWaveShow()
{
    delete waveWidget;
}
