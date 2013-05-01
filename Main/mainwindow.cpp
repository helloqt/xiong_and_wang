#include "mainwindow.h"

#include "wavewidget/wavewidget.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    Init();

//    //判断是否文件有结束标志：
//    if (QFile::exists(TEMPFILENAME))
//    {
//        if (MyFile::IsWriteFinishState(TEMPFILENAME))
//        {
//            this->fileTemp->Open(TEMPFILENAME);
//        }
//        else
//        {
//            if (QMessageBox::Ok == QMessageBox::information(this, "提示", "系统未正常关闭，是否从中断处继续采样？", QMessageBox::Yes, QMessageBox::No))
//            {

//            }
//        }
//    }

}

MainWindow::~MainWindow()
{
    delete confShow;
    //delete grayShow;
    delete infoShow;
    delete testShow;
    delete waveShow;
    delete finishShow;
}

void MainWindow::Init()
{
//debug部分**************************************************************************************************************
    autoShowTimer = new QTimer;
    connect(autoShowTimer,
            SIGNAL(timeout()),
            this,
            SLOT(on_AutoShowTimer()));

    currentDataNodeIndex = 0;
//debug部分**************************************************************************************************************

    //变量部分*******************************************************************************
    port = new MyPort;
    fileLoad = new MyFile;
    fileTemp = new MyFile;
    fileData = new MyFile;

    thresholdHeight = 500;
    thresholdLow = 200;
    currentHeight = 0;
    recvDataCount = 0;
    isStartSampling = false;
    grayWidgetCurrentPage = 0;

    //变量部分*******************************************************************************


    //Ui部分*******************************************************************************
    confShow = new widgetConf;
    //grayShow = new widgetGrayShow;
    infoShow = new widgetInfo;
    testShow = new widgetTest;
    waveShow = new widgetWaveShow;
    finishShow = new FinishShow;
    showConf = new ShowConf;
    widgetSpace = new QWidget;
    //主窗口部分*********************************
    layoutMain->addWidget(infoShow);
    layoutMain->addWidget(confShow);

    layoutMain->addWidget(testShow);
    layoutMain->addWidget(waveShow);
    layoutMain->addWidget(finishShow);
    layoutMain->addWidget(widgetSpace);
    layoutMain->setStretch(0, 1);
    layoutMain->setStretch(1, 10);
    layoutMain->setStretch(2, 10);
    layoutMain->setStretch(3, 10);
    layoutMain->setStretch(4, 10);
    layoutMain->setStretch(5, 10);


    //主窗口部分*********************************

    infoShow->spinBoxWaveCount->setValue(8);
    infoShow->spinScale->setValue(50);
    infoShow->comboProfile->setCurrentIndex(2);
    infoShow->comboShowMode->setCurrentIndex(0);


    on_ScaleChange(50);
    on_WaveCountChange(8);
    on_ProfileChange(2);

    WhenConfUi();
    //Ui部分*******************************************************************************

    //信号槽***********************************************************************************
    //当完成一次接受时触发。
    connect(port, SIGNAL(FinishThisRecv(MyDataNode&)), this, SLOT(FinishDataRecv(MyDataNode&)));

    connect(infoShow->spinScale, SIGNAL(valueChanged(int)), this, SLOT(on_ScaleChange(int)));
    connect(infoShow->spinBoxWaveCount, SIGNAL(valueChanged(int)), this, SLOT(on_WaveCountChange(int)));
    connect(infoShow->comboProfile, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ProfileChange(int)));
    connect(infoShow->comboShowMode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ShowModeChange(int)));

    connect(confShow, SIGNAL(SaveConf()), this, SLOT(on_ConfSave()));

    connect(finishShow->browseMode, SIGNAL(showMode(int)), this, SLOT(on_BrowseModeSelected(int)));
}


void MainWindow::AppendDataNode(MyDataNode &node)
{
    //将原始数据存入listRow中;
    QList<qint16> listRow;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<400;j++)
            listRow.append(node.data[i][j]);
    }
    dataNodeListRaw.append(listRow);

    //将要显示成灰度的数据存入listPro中.
    QList<qint16> listPro;
    qint16 temp;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<400;j+=2)
        {
            temp = abs(node.data[i][j]);
            if (temp <= thresholdLow)
                temp = 255;
            else if (temp >= thresholdHeight)
                temp = 0;
            else
            {
                temp -= thresholdLow;
                temp = abs((qreal)temp * (qreal)255 / (qreal)(thresholdHeight - thresholdLow) - 255);
            }
            listPro.append(temp);
        }
    }

    dataNodeListPro.append(listPro);


    //将高度存入dataNodeHeight中
    dataNodeHeigth.append(node.height);

}

void MainWindow::AppendDataNode(QList<qint16> &listRow, qreal height)
{
    dataNodeListRaw.append(listRow);

    //将要显示成灰度的数据存入listPro中.
    QList<qint16> listPro;
    qint16 temp;
    int count = listRow.count();
    for (int i = 0; i < count; i += 2)
    {
        temp = abs(listRow.at(i));
        if (temp <= thresholdLow)
            temp = 255;
        else if (temp >= thresholdHeight)
            temp = 0;
        else
        {
            temp -= thresholdLow;
            temp = abs((qreal)temp * (qreal)255 / (qreal)(thresholdHeight - thresholdLow) - 255);
        }
        listPro.append(temp);
    }

    dataNodeListPro.append(listPro);

    //将高度存入dataNodeHeight中
    dataNodeHeigth.append(height);
}

void MainWindow::UpdateThreshold()
{
    QTime time;
    time.start();
    if (listThreholdHeight != thresholdHeight || listThresholdLow != thresholdLow)
    {
        //timerForProgress->start(500);

        dataNodeListPro.clear();
        listThreholdHeight = thresholdHeight;
        listThresholdLow = thresholdLow;

        qint32 rawNodeCount = dataNodeListRaw.count();
        qint16 temp = 0;
        QList<qint16> listPro;
        for (int i = 0; i < rawNodeCount; i++)
        {
            for (int j = 0; j < SECTIONCOUNT * POINTCOUNT; j += 2)
            {
                temp = abs(dataNodeListRaw.at(i).at(j));
                if (temp <= thresholdLow)
                    temp = 255;
                else if (temp >= thresholdHeight)
                    temp = 0;
                else
                {
                    temp -= thresholdLow;
                    temp = abs((qreal)temp * (qreal)255 / (qreal)(thresholdHeight - thresholdLow) - 255);
                }
                listPro.append(temp);
            }
            dataNodeListPro.append(listPro);
            listPro.clear();
        }

        //int currentPage = finishShow->grayShow->GetCurrentPage();
        finishShow->grayShow->loadData(&dataNodeHeigth, &dataNodeListPro);
        //finishShow->grayShow->showPage(currentPage);
    }
#ifdef MY_DEBUG
    qDebug() << "Threshold elapse : " << time.elapsed();
#endif
}


void MainWindow::WhenConfUi()
{
    //widget控件
    confShow->show();
    finishShow->hide();
    infoShow->hide();
    testShow->hide();
    waveShow->hide();
    widgetSpace->hide();
    //按钮控件
    btnConf->setEnabled(true);
    btnTest->setEnabled(false);
    btnStart->setEnabled(false);
    btnStop->setEnabled(false);
    btnShowConf->setEnabled(false);
    btnBrowseMode->setEnabled(true);
    //功能可用
    showConf->gBGrayShow->setEnabled(false);
    showConf->gBShowMode->setEnabled(false);
    infoShow->comboShowMode->setEnabled(false);
}
void MainWindow::WhenConfFinishUi()
{
    //widget控件
    confShow->hide();
    finishShow->hide();
    infoShow->show();
    testShow->hide();
    waveShow->hide();
    widgetSpace->show();
    //按钮控件
    btnConf->setEnabled(true);
    btnTest->setEnabled(true);
    btnStart->setEnabled(false);
    btnStop->setEnabled(false);
    btnShowConf->setEnabled(false);
    btnBrowseMode->setEnabled(true);
    //功能可用
    showConf->gBGrayShow->setEnabled(false);
    showConf->gBShowMode->setEnabled(false);
    infoShow->comboShowMode->setEnabled(false);

    //信息更新
    infoShow->AppendHistoryInfo("配置设置成功！");
}

void MainWindow::WhenFinishUi()
{
    //widget控件
    confShow->hide();
    finishShow->show();
    finishShow->ShowMode(2);
    infoShow->show();
    testShow->hide();
    waveShow->hide();
    widgetSpace->hide();
    //按钮控件
    btnConf->setEnabled(true);
    btnTest->setEnabled(true);
    btnStart->setEnabled(true);
    btnStop->setEnabled(true);
    btnShowConf->setEnabled(true);
    btnBrowseMode->setEnabled(true);
    //功能可用
    showConf->gBGrayShow->setEnabled(true);
    showConf->gBShowMode->setEnabled(true);
    infoShow->comboShowMode->setEnabled(true);

    if (isStartSampling)
    {
        //信息更新
        infoShow->SetState("结束数据接收");
        infoShow->AppendHistoryInfo("结束数据接收!");
    }

}
void MainWindow::WhenStartUi()
{
    //widget控件
    confShow->hide();
    finishShow->hide();
    infoShow->show();
    testShow->hide();
    waveShow->show();
    widgetSpace->hide();
    //按钮控件
    btnConf->setEnabled(false);
    btnTest->setEnabled(false);
    btnStart->setEnabled(false);
    btnStop->setEnabled(true);
    btnShowConf->setEnabled(true);
    btnBrowseMode->setEnabled(false);
    //功能可用
    showConf->gBGrayShow->setEnabled(false);
    showConf->gBShowMode->setEnabled(false);
    infoShow->comboShowMode->setEnabled(false);

    //信息更新
    infoShow->SetState("正在接收数据...");
    infoShow->AppendHistoryInfo("开始数据接收!");
}
void MainWindow::WhenTestUi()
{
    //widget控件
    confShow->hide();
    finishShow->hide();
    infoShow->show();
    testShow->show();
    waveShow->hide();
    widgetSpace->hide();
    //按钮控件
    btnConf->setEnabled(false);
    btnTest->setEnabled(true);
    btnStart->setEnabled(false);
    btnStop->setEnabled(false);
    btnShowConf->setEnabled(true);
    btnTest->setText("结束标定模式");
    btnBrowseMode->setEnabled(false);

    //功能可用
    showConf->gBGrayShow->setEnabled(false);
    showConf->gBShowMode->setEnabled(false);
    infoShow->comboShowMode->setEnabled(false);

    //信息更新
    infoShow->comboProfile->setCurrentIndex(testShow->startDlg->GetProfile());
    infoShow->SetState("正在进行标定模式...");
    infoShow->AppendHistoryInfo("开始标定模式！");
}

void MainWindow::WhenTestFinishUi()
{

    //widget控件
    confShow->hide();
    finishShow->hide();
    infoShow->show();
    testShow->hide();
    waveShow->hide();
    widgetSpace->show();
    //按钮控件
    btnConf->setEnabled(true);
    btnTest->setEnabled(true);
    btnStart->setEnabled(true);
    btnStop->setEnabled(false);
    btnShowConf->setEnabled(true);
    btnTest->setText("标定模式");
    btnBrowseMode->setEnabled(false);

    //功能可用
    showConf->gBGrayShow->setEnabled(false);
    showConf->gBShowMode->setEnabled(false);
    infoShow->comboShowMode->setEnabled(false);

    testShow->finishDlg->SetSpeed(infoShow->GetSpeed());
    infoShow->SetState("标定模式结束！");
    infoShow->AppendHistoryInfo("标定模式已结束！\n获取的声速为：" + testShow->finishDlg->GetSpeed());
    infoShow->SetSpeed(testShow->finishDlg->GetSpeed().toDouble());
}


bool MainWindow::OpenDataPort()
{
    //打开数据串口：
    port->ClostPort();
    port->SetBaudPart(confShow->GetDataBaud());
    if (port->OpenPort(confShow->GetDataPortName()))
    {
        infoShow->SetDataPort(confShow->GetDataPortName());
#ifdef MY_DEBUG
        qDebug() << "打开串口" + confShow->GetDataPortName() + "成功!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + confShow->GetDataPortName() + "成功!");
        return true;
    }
    else
    {
        infoShow->SetDataPort("");
#ifdef MY_DEBUG
        qDebug() << "打开串口" + confShow->GetDataPortName() + "失败!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + confShow->GetDataPortName() + "失败!");
        return false;
    }


}

bool MainWindow::OpenHeightPort()
{
    //打开高度串口：
    if (port->OpenHeightPort(confShow->GetHeightPortName()))
    {
        infoShow->SetHeightPort(confShow->GetHeightPortName());
#ifdef MY_DEBUG
        qDebug() << "打开串口" + confShow->GetHeightPortName() + "成功!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + confShow->GetHeightPortName() + "成功!");
        return true;
    }
    else
    {
        infoShow->SetHeightPort("");
#ifdef MY_DEBUG
        qDebug() << "打开串口" + confShow->GetHeightPortName() + "失败!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + confShow->GetHeightPortName() + "失败!");
        return false;
    }
}

void MainWindow::FinishDataRecv(MyDataNode &node)
{
    //将原始数据存入listRow中;
        QList<qint16> listRow;

        for(int i = 0; i < SECTIONCOUNT; i++)
        {
            for(int j = 0; j < POINTCOUNT; j++)
                listRow.append(node.data[i][j]);
        }

        AppendDataNode(listRow, node.height);
        currentHeight = node.height;
        recvDataCount++;
        infoShow->SetPointCount(QString::number(recvDataCount));
        infoShow->SetHeight(currentHeight);

        if (testShow->isVisible())
        {
            testShow->updataData(listRow);
        }
        if (waveShow->isVisible())
        {
            waveShow->waveWidget->appendData(listRow, currentHeight);
            //将数据存入filedata中：
            this->fileData->WriteData(node);
            //更新当前高度：
            infoShow->SetHeight(node.height);
            //更新当前采集点数：
            infoShow->SetPointCount(QString::number(recvDataCount));
        }
}

void MainWindow::on_btnConf_clicked()
{
    WhenConfUi();
}

void MainWindow::on_btnTest_clicked()
{
    if (!port->IsOpen())
    {
        if (!OpenDataPort())
        {
            QMessageBox::warning(this, "错误！", "数据串口未打开！");
            //widget控件
            confShow->hide();
            finishShow->hide();
            infoShow->show();
            testShow->hide();
            waveShow->hide();
            widgetSpace->show();
            //按钮控件
            btnConf->setEnabled(true);
            btnTest->setEnabled(true);
            btnStart->setEnabled(false);
            btnStop->setEnabled(false);
            btnShowConf->setEnabled(false);
            btnBrowseMode->setEnabled(true);

            //功能可用
            showConf->gBGrayShow->setEnabled(false);
            showConf->gBShowMode->setEnabled(false);
            infoShow->comboShowMode->setEnabled(false);
            return;
        }
    }

    if (btnTest->text().compare("标定模式") == 0)
    {
        if (QDialog::Accepted == testShow->startDlg->exec())
        {
            WhenTestUi();
        }
        else
        {
            //widget控件
            confShow->hide();
            finishShow->hide();
            infoShow->show();
            testShow->hide();
            waveShow->hide();
            widgetSpace->show();
            //按钮控件
            btnConf->setEnabled(true);
            btnTest->setEnabled(true);
            btnStart->setEnabled(false);
            btnStop->setEnabled(false);
            btnShowConf->setEnabled(false);
            btnBrowseMode->setEnabled(true);

            //功能可用
            showConf->gBGrayShow->setEnabled(false);
            showConf->gBShowMode->setEnabled(false);
            infoShow->comboShowMode->setEnabled(false);
        }
    }
    else
    {
        if (QDialog::Accepted == testShow->finishDlg->exec())
        {
            WhenTestFinishUi();

        }
    }
}

void MainWindow::on_btnStart_clicked()
{
    if (!port->IsOpen())
    {

        if (!OpenDataPort())
        {
            QMessageBox::warning(this, "错误！", "数据串口未打开！");
            //widget控件
            confShow->hide();
            finishShow->hide();
            infoShow->show();
            testShow->hide();
            waveShow->hide();
            widgetSpace->show();
            //按钮控件
            btnConf->setEnabled(true);
            btnTest->setEnabled(true);
            btnStart->setEnabled(true);
            btnStop->setEnabled(false);
            btnShowConf->setEnabled(false);
            btnBrowseMode->setEnabled(true);

            //功能可用
            showConf->gBGrayShow->setEnabled(false);
            showConf->gBShowMode->setEnabled(false);
            infoShow->comboShowMode->setEnabled(false);
            return;
        }

    }
    if (!port->heightPort->IsOpen())
    {
        if (!OpenHeightPort())
        {
            QMessageBox::warning(this, "错误！", "高度串口未打开！");
            //widget控件
            confShow->hide();
            finishShow->hide();
            infoShow->show();
            testShow->hide();
            waveShow->hide();
            widgetSpace->show();
            //按钮控件
            btnConf->setEnabled(true);
            btnTest->setEnabled(true);
            btnStart->setEnabled(true);
            btnStop->setEnabled(false);
            btnShowConf->setEnabled(false);
            btnBrowseMode->setEnabled(true);

            //功能可用
            showConf->gBGrayShow->setEnabled(false);
            showConf->gBShowMode->setEnabled(false);
            infoShow->comboShowMode->setEnabled(false);
            return;
        }
    }

    //打开保存文件
    fileData->Open(confShow->GetFileName());

    WhenStartUi();

    isStartSampling = true;
}



void MainWindow::on_btnBrowseMode_clicked()
{
    //widget控件
    confShow->hide();
    finishShow->show();
    finishShow->ShowMode(2);
    infoShow->show();
    testShow->hide();
    waveShow->hide();
    widgetSpace->hide();
    //按钮控件
    btnConf->setEnabled(true);
    btnTest->setEnabled(false);
    btnStart->setEnabled(false);
    btnStop->setEnabled(false);
    btnShowConf->setEnabled(true);
    btnBrowseMode->setEnabled(true);
    //功能可用
    showConf->gBGrayShow->setEnabled(true);
    showConf->gBShowMode->setEnabled(true);
    infoShow->comboShowMode->setEnabled(true);


    port->ClostPort();
    port->CloseHeightPort();

}


void MainWindow::on_btnStop_clicked()
{
    if (QMessageBox::Yes == QMessageBox::information(this, "是否结束数据采集？", "是否结束数据采集？", QMessageBox::Yes, QMessageBox::No))
    {
        QMessageBox::information(this, "提示", "当前采集数据已存放到" + confShow->GetFileName() + "文件中");
        WhenFinishUi();

        this->fileData->Close();
        isStartSampling = false;
    }

    port->ClostPort();
    port->CloseHeightPort();

}

void MainWindow::on_ScaleChange(int scale)
{
    float tempScale = (float)scale * 0.004;
    testShow->setScale(tempScale);
    waveShow->waveWidget->setScale(tempScale);
    finishShow->waveShow->setScale(tempScale);
    showConf->SetScale(scale);
}

void MainWindow::on_WaveCountChange(int waveCount)
{
    waveShow->waveWidget->setRowCount(waveCount);
    finishShow->waveShow->setRowCount(waveCount);
    showConf->SetWaveCount(waveCount);
}
void MainWindow::on_ShowModeChange(int showMode)
{
    finishShow->ShowMode(showMode);
    showConf->SetShowMode(showMode);
}
void MainWindow::on_ProfileChange(int profile)
{
    testShow->SetProfile(profile);
    waveShow->waveWidget->SetProfile(profile);
    finishShow->waveShow->SetProfile(profile);
    showConf->SetProfile(profile);
}

void MainWindow::on_ConfSave()
{

    WhenConfFinishUi();

//    OpenDataPort();
//    OpenHeightPort();
}

void MainWindow::on_BrowseModeSelected(int mode)
{
    if (mode == 0)//从文件中浏览
    {
        QString fileName = QFileDialog::getOpenFileName(this, "打开文件","",
            "uds文件 (*.uds)");
        if (fileName.isEmpty())
        {
            return;
        }
        this->fileLoad->LoadDataFromFile(fileName);
        MyDataNode node;
        dataNodeHeigth.clear();
        dataNodeListPro.clear();
        dataNodeListRaw.clear();

        while (this->fileLoad->GetNextDataNode(node))
        {
            AppendDataNode(node);
        }

        this->fileLoad->Close();
    }
    else//从当前数据中浏览：
    {

    }

    //将数据对象的指针传个finishhow对象。
    finishShow->grayShow->loadData(&dataNodeHeigth, &dataNodeListPro);
    finishShow->InitData(&dataNodeListRaw, &dataNodeHeigth);

    finishShow->ShowMode(infoShow->GetShowMode());
}

//debug部分**************************************************************************************************************


void MainWindow::on_btnSendCommand_clicked()
{
    switch (comboBoxCommand->currentIndex()) {
    case 0:
        port->Command(1, 0);
        break;
    case 1:
        port->Command(2, 0);
        break;

    case 2:
        port->Command(3, 1);
        break;
    case 3:
        port->Command(3, 2);
        break;
    case 4:
        port->Command(3, 3);
        break;

    case 5:
        port->Command(4, 1);
        break;
    case 6:
        port->Command(4, 2);
        break;

    case 7:
        port->Command(5, 0x88);
        break;
    default:
        break;
    }
}

void MainWindow::on_btnLoadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件","",
        "uds文件 (*.uds)");
    if (fileName.isEmpty())
    {
        return;
    }
    this->fileLoad->LoadDataFromFile(fileName);
    MyDataNode node;
    dataNodeHeigth.clear();
    dataNodeListPro.clear();
    dataNodeListRaw.clear();

    while (this->fileLoad->GetNextDataNode(node))
    {
        AppendDataNode(node);
    }

    this->fileLoad->Close();

    this->fileLoad->Open(TEMPFILENAME);
//    //将原始数据存入listRow中;
//    QList<qint16> listRow;
//    int heihgtindex = 0;
//    while (this->file->GetNextDataNode(node))
//    {
//        for(int i=0;i<4;i++)
//        {
//            for(int j=0;j<400;j++)
//                listRow.append(node.data[i][j]);
//        }

//        dataNodeListRaw.append(listRow);
//        dataNodeHeigth.append(heihgtindex++);
//        listRow.clear();
//    }
//    this->file->SeekToNodeIndexBegin(340);
//    while (this->file->GetNextDataNode(node))
//    {
//        for(int i=0;i<4;i++)
//        {
//            for(int j=0;j<400;j++)
//                listRow.append(node.data[i][j]);
//        }
//        dataNodeListRaw.append(listRow);
//        dataNodeHeigth.append(heihgtindex++);
//        listRow.clear();
//    }
//    this->file->SeekToNodeIndexBegin(440);
//    while (this->file->GetNextDataNode(node))
//    {
//        for(int i=0;i<4;i++)
//        {
//            for(int j=0;j<400;j++)
//                listRow.append(node.data[i][j]);
//        }
//        dataNodeListRaw.append(listRow);
//        dataNodeHeigth.append(heihgtindex++);
//        listRow.clear();
//    }
//    this->file->SeekToNodeIndexBegin(540);
//    while (this->file->GetNextDataNode(node))
//    {
//        for(int i=0;i<4;i++)
//        {
//            for(int j=0;j<400;j++)
//                listRow.append(node.data[i][j]);
//        }
//        dataNodeListRaw.append(listRow);
//        dataNodeHeigth.append(heihgtindex++);
//        listRow.clear();
//    }

//    this->file->SaveDataToFile(QString("test2.uds"), dataNodeListRaw, dataNodeHeigth);




}

void MainWindow::on_btnShowNext_clicked()
{
    if (currentDataNodeIndex >= dataNodeListRaw.count())
        return;
    if (port->serialPort->isOpen())
    {
        quint8 *sendBuff;
        int byteCount = SECTIONCOUNT * POINTCOUNT * 2;
        sendBuff = new quint8[byteCount + 16];
        int index = 0;
        qint16 temp;
        for (index = 0; index < 8; index++)
        {
            sendBuff[index] = 0xAA;
        }

        for (int i = 0; i < POINTCOUNT; i++)
        {
            for (int j = 0; j < SECTIONCOUNT; j++)
            {
                temp = dataNodeListRaw.at(currentDataNodeIndex).at(i + j * 400);
                sendBuff[index++] = temp >> 8;
                sendBuff[index++] = temp;
            }
        }

        for (index = byteCount + 8; index < byteCount + 16; index++)
        {
            sendBuff[index] = 0xAA;
        }
        port->serialPort->write((char*)sendBuff, byteCount + 16);
        delete sendBuff;
    }

    if (testShow->isVisible())
    {
        testShow->updataData(dataNodeListRaw.at(currentDataNodeIndex));
    }
    if (waveShow->isVisible())
    {
        waveShow->waveWidget->appendData(dataNodeListRaw.at(currentDataNodeIndex), dataNodeHeigth.at(currentDataNodeIndex));
    }
    if (finishShow->isVisible())
    {
        finishShow->waveShow->appendData(dataNodeListRaw.at(currentDataNodeIndex), dataNodeHeigth.at(currentDataNodeIndex));
    }

    if (this->fileLoad->IsOpen())
    {
        MyDataNode node;
        for (int i = 0; i < SECTIONCOUNT; i++)
        {
            for (int j = 0; j < POINTCOUNT; j++)
            {
                node.data[i][j] = dataNodeListRaw.at(currentDataNodeIndex).at(i * POINTCOUNT + j);
            }
        }
        this->fileLoad->WriteData(node);
    }

    currentDataNodeIndex++;
}

void MainWindow::on_btnAutoShow_clicked()
{
    if (autoShowTimer->isActive())
    {
        btnAutoShow->setText("开始自动显示");
        autoShowTimer->stop();
    }
    else
    {
        btnAutoShow->setText("停止自动显示");
        autoShowTimer->start(lEditAutoShow->text().toInt());
    }
}

void MainWindow::on_AutoShowTimer()
{
    on_btnShowNext_clicked();
}


void MainWindow::on_btnOpenPort_clicked()
{
    switch (comboBaudRate->currentIndex()) {
    case 0:
        port->SetBaudPart(BAUD115200);
        break;
#if defined(Q_OS_UNIX) || defined(qdoc)
    case 1:
        port->SetBaudPart(BAUD230400);
        break;
#endif
#if defined(Q_OS_WIN) || defined(qdoc)
    case 2:
        port->SetBaudPart(BAUD256000);
        break;
#endif
    default:
        port->SetBaudPart(BAUD115200);
        break;
    }

    //port->SetBaudPart(BAUD9600);
    port->ClostPort();
    if (port->OpenPort(comboPort->currentText()))
    {
        infoShow->SetDataPort(comboPort->currentText());
#ifdef MY_DEBUG
        qDebug() << "打开串口" + comboPort->currentText() + "成功!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + comboPort->currentText() + "成功!");
    }
    else
    {
        infoShow->SetDataPort("");
#ifdef MY_DEBUG
        qDebug() << "打开串口" + comboPort->currentText() + "失败!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + comboPort->currentText() + "失败!");
    }
}

void MainWindow::on_btnOpenPortHeight_clicked()
{
    if (port->heightPort->openPort(comboPortHeight->currentText()))
    {
        infoShow->SetHeightPort(comboPortHeight->currentText());
#ifdef MY_DEBUG
        qDebug() << "打开串口" + comboPortHeight->currentText() + "成功!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + comboPortHeight->currentText() + "成功!");
    }
    else
    {
        infoShow->SetHeightPort("");
#ifdef MY_DEBUG
        qDebug() << "打开串口" + comboPortHeight->currentText() + "失败!";
#endif
        infoShow->AppendHistoryInfo("打开串口" + comboPortHeight->currentText() + "失败!");
    }
}

void MainWindow::on_btnSeekToNodeIndex_clicked()
{
    currentDataNodeIndex = lEditNodeIndex->text().toInt();
}

void MainWindow::on_btnTempFileSaveAs_clicked()
{
//    QString fileName = QFileDialog::getSaveFileName(this, "保存文件","",
//        "uds文件 (*.uds)");
//    if (fileName.isEmpty())
//    {
//        return;
//    }
//    QFile::copy(this->file->fileName, fileName);

    if (this->fileLoad->IsOpen())
        this->fileLoad->Close();

    this->fileLoad->Open(TEMPFILENAME);
    this->fileLoad->WriteFinishState();
    this->fileLoad->Close();
    if (MyFile::IsWriteFinishState(TEMPFILENAME))
        qDebug() << "finish";
}


void MainWindow::on_pushButton_clicked()
{

//    if (this->file->IsOpen())
//        this->file->Close();

//    FILE*myFile = fopen("1", "r+");
//    fseek(myFile, -(sizeof("finish")), SEEK_END);
//    qDebug() <<  sizeof("finish");
//    char *c = new char[sizeof("finish")];
//    fread(c, sizeof("finish"), 1, myFile);
//    qDebug() << c;
//    QString str(c);
//    qDebug() << str;

}


//debug部分**************************************************************************************************************

void MainWindow::on_btnShowConf_clicked()
{
    if (QDialog::Accepted == showConf->exec())
    {
        infoShow->spinScale->setValue(showConf->GetScale());
        infoShow->spinBoxWaveCount->setValue(showConf->GetWaveCount());
        infoShow->comboProfile->setCurrentIndex(showConf->GetProfile());
        infoShow->comboShowMode->setCurrentIndex(showConf->GetShowMode());
        thresholdLow = showConf->GetThresholdLow();
        UpdateThreshold();
    }
}


