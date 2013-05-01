#ifndef GRAYWIDGET_H
#define GRAYWIDGET_H

#include "graywidgetindex.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

class GrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrayWidget(QWidget *parent = 0);
    void loadData(QList<qreal >height,QList<QList<qint16 > > gray);
    void showPage(int page);
    bool canUp();
    bool canDown();
    void setGraduation(int mini,int max);
signals:

public slots:
    void showLeft();
    void showRight();
private:
    QString m_fileName;
    GrayWidgetindex *m_leftWidget;
    GrayWidgetindex *m_rightWidget;
    QHBoxLayout *m_mainLayout;
    int m_currentPage;
    bool isAtBegin;
    bool isAtEnd;
    bool showSolo;
    QList<qreal > m_height;
    QList<QList<qint16 > > m_grayData;
};

#endif // GRAYWIDGET_H
