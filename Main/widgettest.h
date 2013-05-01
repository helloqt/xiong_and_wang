#ifndef WIDGETTEST_H
#define WIDGETTEST_H

#include <QWidget>
#include "ui_widgettest.h"
#include "testwidget/samplingwidget.h"
#include "UserDef.h"
#include "testviewstartdlg.h"
#include "testviewfinishdlg.h"
class widgetTest : public QWidget, public Ui::widgetTest
{
    Q_OBJECT
    

public:
    explicit widgetTest(QWidget *parent = 0);
    ~widgetTest();

    void setScale(qreal value);
    //data.size=400
    void updataXData(QList<qint16> node);
    void updataYData(QList<qint16> node);
    void updataXxData(QList<qint16> node);
    void updataYyData(QList<qint16> node);

    //data.size=1600
    void updataData(MyDataNode &node);
    void updataData(QList<qint16> listData);
    int getXValue();
    int getYValue();
    int getXxValue();
    int getYyValue();

    void SetProfile(int profileIndex);

    SamplingWidget *XWidget;
    SamplingWidget *XxWidget;

    SamplingWidget *YWidget;
    SamplingWidget *YyWidget;

    testViewStartDlg *startDlg;
    testViewFinishDlg *finishDlg;
private slots:
    void XValueChange(int, int);
    void XxValueChange(int, int);
    void YValueChange(int, int);
    void YyValueChange(int, int);
    void on_btnPreX_clicked();
    void on_btnNextX_clicked();
    void on_btnPreXx_clicked();
    void on_btnNextXx_clicked();
    void on_btnPreY_clicked();
    void on_btnNextY_clicked();
    void on_btnPreYy_clicked();
    void on_btnNextYy_clicked();
};

#endif // WIDGETTEST_H
