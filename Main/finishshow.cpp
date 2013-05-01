#include "finishshow.h"

FinishShow::FinishShow(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    grayShow = new GrayWidget;
    waveShow = new WaveWidget;
    browseMode = new BrowseMode;

    layoutFinish->addWidget(grayShow);
    layoutFinish->addWidget(waveShow);
    layoutFinish->addWidget(browseMode);
    ShowMode(0);
    currentDataNodeStart = 0;
    pDataNodeListRaw = NULL;
    pDataHeight = NULL;


}

void FinishShow::on_btnPageUp_clicked()
{
    if (grayShow->isVisible())
    {
        grayShow->PageUp();
    }
    else
    {
        ShowPreviousWave();
    }
    UpdatePageBtn();
}

void FinishShow::on_btnPageDown_clicked()
{
    if (grayShow->isVisible())
    {
        grayShow->PageDown();
    }
    else
    {
        ShowNextWave();
    }
    UpdatePageBtn();

}

void FinishShow::ShowMode(int showMode)
{
    if (showMode == 0)
        ShowWave();
    if (showMode == 1)
        ShowGray();
    if (showMode == 2)
        ShowBrowseMode();
    UpdatePageBtn();
}

void FinishShow::ShowGray()
{
    grayShow->show();
    waveShow->hide();
    browseMode->hide();
}

void FinishShow::ShowWave()
{
    grayShow->hide();
    waveShow->show();
    browseMode->hide();
}

void FinishShow::ShowBrowseMode()
{
    grayShow->hide();
    waveShow->hide();
    browseMode->show();
}

void FinishShow::ShowPreviousWave()
{
    if (currentDataNodeStart - waveShow->getRowCount() < 0)
    {
        currentDataNodeStart = 0;
    }
    else
        currentDataNodeStart -= waveShow->getRowCount();
    int templength = 0;
    if (currentDataNodeStart + waveShow->getRowCount() > pDataNodeListRaw->count())
        templength = -1;
    else
        templength = waveShow->getRowCount();

    if (currentDataNodeStart + templength <= pDataNodeListRaw->count())
    {
        waveShow->ShowData(pDataNodeListRaw->mid(currentDataNodeStart, templength),
                           pDataHeight->mid(currentDataNodeStart, templength));
    }

    UpdatePageBtn();
}

void FinishShow::ShowNextWave()
{
    if (currentDataNodeStart + waveShow->getRowCount() > pDataNodeListRaw->count())
        currentDataNodeStart = pDataNodeListRaw->count() - waveShow->getRowCount();
    else
        currentDataNodeStart += waveShow->getRowCount();
    int templength = 0;
    if (currentDataNodeStart + waveShow->getRowCount() > pDataNodeListRaw->count())
        templength = -1;
    else
        templength = waveShow->getRowCount();
    if (currentDataNodeStart + templength <= pDataNodeListRaw->count())
    {
        waveShow->ShowData(pDataNodeListRaw->mid(currentDataNodeStart, templength),
                           pDataHeight->mid(currentDataNodeStart, templength));
    }

    UpdatePageBtn();
}

void FinishShow::UpdatePageBtn()
{
    if (grayShow->isVisible())
    {
        btnPageDown->setEnabled(grayShow->canDown());
        btnPageUp->setEnabled(grayShow->canUp());
    }
    else if (waveShow->isVisible())
    {
        if (pDataNodeListRaw == NULL)
        {
            btnPageDown->setEnabled(false);
            btnPageUp->setEnabled(false);
            return;
        }

        if (currentDataNodeStart > 0)
            btnPageUp->setEnabled(true);
        else
        {
            btnPageUp->setEnabled(false);
            currentDataNodeStart = 0;
        }
        if (currentDataNodeStart + waveShow->getRowCount() < pDataNodeListRaw->count())
            btnPageDown->setEnabled(true);
        else
        {
            btnPageDown->setEnabled(false);
            currentDataNodeStart = pDataNodeListRaw->count() - waveShow->getRowCount();
        }
    }
    else
    {
        btnPageDown->setEnabled(false);
        btnPageUp->setEnabled(false);
    }
}

void FinishShow::InitData(QList<QList<qint16> > *pData, QList<qreal> *pHeight)
{
    pDataNodeListRaw = pData;
    pDataHeight = pHeight;
    currentDataNodeStart = 0;

    int templength = 0;
    if (currentDataNodeStart + waveShow->getRowCount() > pDataNodeListRaw->count())
        templength = -1;
    else
        templength = waveShow->getRowCount();
    waveShow->ShowData(pDataNodeListRaw->mid(currentDataNodeStart, templength),
                       pDataHeight->mid(currentDataNodeStart, templength));

    UpdatePageBtn();
}
