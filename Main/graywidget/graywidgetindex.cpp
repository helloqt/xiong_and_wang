
#include "graywidgetindex.h"
#include <QDebug>
#include <QTime>
GrayWidgetindex::GrayWidgetindex(QWidget *parent) :
    QWidget(parent)
{
    m_maxNumber = 400;
    m_miniNumber =0;
    QStringList list;
    list<<"X"<<"X'";
    setTitleText(list);
    type = true;
}

//设置标题 X X' 或者Y Y'
void GrayWidgetindex::setTitleText(QStringList str)
{
    m_titile = str;
    update();
}

//设置数据
void GrayWidgetindex::setData(QList<QList<qint16> > dataList, QList<qreal> heightList)
{
    m_dataList = dataList;
    m_heightList = heightList;
    update();
}

//设置最大刻度
void GrayWidgetindex::setMaxNumber(qreal num)
{
    m_maxNumber = num;
    update();
}

//设置最小刻度
void GrayWidgetindex::setMiniNumber(qreal num)
{
    m_miniNumber = num;
    update();
}

void GrayWidgetindex::paintEvent(QPaintEvent *event)
{
#ifdef MY_DEBUG
    QTime time;
    time.start();
#endif
//    QString strMessage;
//    QString temp;
    QPainter painter(this);
    painter.setPen(Qt::black);

    static int count = 0;
#ifdef MY_DEBUG
    qDebug() << "update grayWidget" << count++;
#endif
    //画眉头的刻度
    for(int i=0;i<7;i++)
    {
        painter.drawLine(QPointF((i/6.0)*(width()-TEXT_WIDTH)+TEXT_WIDTH-1,36),QPointF((i/6.0)*(width()-TEXT_WIDTH)-1+TEXT_WIDTH,40));
        if(i%3==0)
        {
            painter.drawLine(QPointF((i/6.0)*(width()-TEXT_WIDTH)+TEXT_WIDTH-1,0),QPointF((i/6.0)*(width()-TEXT_WIDTH)-1+TEXT_WIDTH,10));
        }
        if(abs(i-3)/3!=1)
        painter.drawText(TEXT_WIDTH-(width()-TEXT_WIDTH)/6.0,10,(2*i+2)*(width()-TEXT_WIDTH)/6.0,40,Qt::AlignCenter,QString::number((int)(m_miniNumber+((m_maxNumber-m_miniNumber)/3.0)*abs(i-3))));
    }

    //画文字（XX'YY')
    painter.drawText(TEXT_WIDTH,0,(width()-TEXT_WIDTH)/2.0,26,Qt::AlignCenter,m_titile.value(0));
    painter.drawText(TEXT_WIDTH+(width()-TEXT_WIDTH)/2.0,0,(width()-TEXT_WIDTH)/2.0,26,Qt::AlignCenter,m_titile.value(1));

    //画数据
    if(type){
        for(int i=0;i<m_dataList.size();i++)
        {
            for(int j=0;j<DATA_NUMBER;j++)
            {
                qint16 number = m_dataList.value(i).value(j);
                if (number >= 245)
                {
                    continue;
                }
                painter.setPen(QColor(number,number,number));
                painter.drawPoint((DATA_NUMBER-j)/((qreal)DATA_NUMBER)*(width()-TEXT_WIDTH)/2.0+TEXT_WIDTH,i+40);
            }
            for(int j=2*DATA_NUMBER;j<3*DATA_NUMBER;j++)
            {
                qint16 number = m_dataList.value(i).value(j);
                if (number >= 245)
                {
                    continue;
                }
                painter.setPen(QColor(number,number,number));
                painter.drawPoint((j-2*DATA_NUMBER)/((qreal)DATA_NUMBER)*(width()-TEXT_WIDTH)/2.0+TEXT_WIDTH+(width()-TEXT_WIDTH)/2.0,i+40);
            }

//            temp.sprintf("%d", i * 100 / m_dataList.size());
//            strMessage = "已更新X-X'剖面:" + temp + "%";

        }
    }else
    {
        for(int i=0;i<m_dataList.size();i++)
        {
            for(int j=DATA_NUMBER;j<2*DATA_NUMBER;j++)
            {
                qint16 number = m_dataList.value(i).value(j);
                if (number >= 245)
                {
                    continue;
                }
                painter.setPen(QColor(number,number,number));
                painter.drawPoint((2*DATA_NUMBER-j)/((qreal)DATA_NUMBER)*(width()-TEXT_WIDTH)/2.0+TEXT_WIDTH,i+40);
            }
            for(int j=3*DATA_NUMBER;j<4*DATA_NUMBER;j++)
            {
                qint16 number = m_dataList.value(i).value(j);
                if (number >= 245)
                {
                    continue;
                }
                painter.setPen(QColor(number,number,number));
                painter.drawPoint((j-3*DATA_NUMBER)/((qreal)DATA_NUMBER)*(width()-TEXT_WIDTH)/2.0+TEXT_WIDTH+(width()-TEXT_WIDTH)/2.0,i+40);

            }

//            temp.sprintf("%d", i * 100 / m_dataList.size());
//            strMessage = "已更新Y-Y'剖面:" + temp + "%";
        }

    }

    //画深度刻度
    painter.setPen(Qt::black);
    for(int i=0;i<m_heightList.size();i++)
    {
        if(i%TEXT_HEIGHT==0)
        {
            painter.drawText(0,30+i,TEXT_WIDTH,TEXT_HEIGHT,Qt::AlignCenter,QString::number(m_heightList.value(i)));
        }
    }
#ifdef MY_DEBUG
    qDebug() << "update grayWidget elapse is " << time.elapsed();
#endif
}

void GrayWidgetindex::mouseDoubleClickEvent(QMouseEvent *)
{
    emit DoubleClicked();
}

void GrayWidgetindex::setType(bool t)
{
    type = t;
}
