#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include "UserDef.h"

#ifndef WAVE_LENGHT_VALUE
#define WAVE_LENGHT_VALUE 400
#endif

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    WaveWidget(QWidget *parent = 0);
    ~WaveWidget();
    void appendData(const QList<qint16> &dataList, const qreal height);
    void setRowCount(int row);
    int getRowCount();
    void ShowData(const QList<QList<qint16> > &dataList, const QList<qreal> &height);
    void setMiniNumber(int mini);
    void setMaxNumber(int max);
    void setScale(qreal value);
    enum Mode{all,left,right};

    void SetProfile(int profile);
private:
    QList<QList<qint16> > m_dataList;
    QList<qreal > m_height;
    int m_row;
    int m_miniNumber;
    int m_maxNumber;
    qreal m_scale;
    Mode m_showMode;
protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
private slots:
};

#endif // WIDGET_H
