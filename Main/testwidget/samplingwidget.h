#ifndef SAMPLINGWIDGET_H
#define SAMPLINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QMouseEvent>
#define BUTTONWIDTH 48

#ifndef WAVE_LENGHT_VALUE
#define WAVE_LENGHT_VALUE 400
#endif

namespace Ui {
class SamplingWidget;
}

class SamplingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SamplingWidget(QWidget *parent = 0);
    ~SamplingWidget();
    void updateDate(QList<qint16> node);
    int getValue();
    void setScale(qreal valueIndex);

    Ui::SamplingWidget *ui;
    bool showButton;
    int valueIndex;
    bool leftMousePress;
    bool isPointInRect(QPoint p,QRect r);
    QList<qint16> m_data;
    qreal m_scale;
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *);
signals:
    void valueChanged(int valueIndex, int value);
};

#endif // SAMPLINGWIDGET_H
