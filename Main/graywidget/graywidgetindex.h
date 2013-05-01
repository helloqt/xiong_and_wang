#ifndef GRAYWIDGETINDEX_H
#define GRAYWIDGETINDEX_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QPainter>
#include <math.h>
#include "UserDef.h"
const static int TEXT_WIDTH = 60;
const static int TEXT_HEIGHT = 20;
const static int DATA_NUMBER = 200;

class GrayWidgetindex : public QWidget
{
    Q_OBJECT
public:
    explicit GrayWidgetindex(QWidget *parent = 0);
    void setTitleText(QStringList str);
    void setData(QList<QList<qint16 > > dataList,QList<qreal > heightList);
    void setMaxNumber(qreal num);
    void setMiniNumber(qreal num);
    void setType(bool t);
signals:
    void DoubleClicked();
public slots:

private:
    QStringList m_titile;
    QList<QList<qint16 > > m_dataList;
    QList<qreal > m_heightList;
    void paintEvent(QPaintEvent *event);
    qreal m_maxNumber;
    qreal m_miniNumber;
    void mouseDoubleClickEvent(QMouseEvent *);
    //XX'Ϊtrue;YY'Ϊfalse;
    bool type;
};

#endif // GRAYWIDGETINDEX_H
