
#include "wavewidget.h"
#include "UserDef.h"
#ifdef MY_DEBUG
#include <QDebug>
#endif
#include <QTime>
WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent)
{
    resize(1024,480);
    m_row = 6;
    m_miniNumber = 0;
    m_maxNumber = 400;
    m_scale = 0.03;
    m_showMode = all;
}

WaveWidget::~WaveWidget()
{

}


void WaveWidget::appendData(const QList<qint16> &listData, const qreal height)
{

    if(m_dataList.size()>=m_row)
    {
        m_dataList.clear();
        m_height.clear();
    }
    m_dataList.append(listData);
    m_height.append(height);
    update();
}

/*****************************************************
  *类型：保护成员函数
  *功能：实现波形的绘制
        通过m_dataList的控制改变绘制的数据
        通过m_height的控制改变绘制的深度
  ***************************************************/
void WaveWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    if(m_showMode == all)
    {
        //画刻度
        painter.drawLine(QPointF(0,36),QPointF(width(),36));

        painter.drawText(QRectF((width()/19.0),0,4*(width()/19.0),36),Qt::AlignHCenter|Qt::AlignTop,"X");
        painter.drawText(QRectF(5*(width()/19.0),0,4*(width()/19.0),36),Qt::AlignHCenter|Qt::AlignTop,"X'");
        painter.drawText(QRectF(10*(width()/19.0),0,4*(width()/19.0),36),Qt::AlignHCenter|Qt::AlignTop,"Y");
        painter.drawText(QRectF(14*(width()/19.0),0,4*(width()/19.0),36),Qt::AlignHCenter|Qt::AlignTop,"Y'");

        for(int i=0;i<=5;i++)
        {
            painter.drawText(0,10,2*i*(4*width()/19.0/5.0)+10*(width()/19.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
            painter.drawText(0,10,-2*i*(4*width()/19.0/5.0)+10*(width()/19.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
        }

        for(int i=0;i<=5;i++)
        {
            painter.drawText(0,10,2*i*(4*width()/19.0/5.0)+28*(width()/19.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
            painter.drawText(0,10,-2*i*(4*width()/19.0/5.0)+28*(width()/19.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF((width()/19.0)+4*(width()/19.0)*(i/5),0),QPointF((width()/19.0)+4*(width()/19.0)*(i/5),12));
                painter.drawLine(QPointF((width()/19.0)+4*(width()/19.0)*(i/5),30),QPointF((width()/19.0)+4*(width()/19.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF((width()/19.0)+4*(width()/19.0)*(i/5.0),30),QPointF((width()/19.0)+4*(width()/19.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5),0),QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5),12));
                painter.drawLine(QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5),30),QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5.0),30),QPointF(5*(width()/19.0)+4*(width()/19.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5),0),QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5),12));
                painter.drawLine(QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5),30),QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5.0),30),QPointF(10*(width()/19.0)+4*(width()/19.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5),0),QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5),12));
                painter.drawLine(QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5),30),QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5.0),30),QPointF(14*(width()/19.0)+4*(width()/19.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<m_dataList.size();i++)
        {
            //标注深度
            painter.drawText(0,i*((qreal)(height()-48)/m_row)+48,(width()/19.0),((qreal)height()/m_row),Qt::AlignLeft|Qt::AlignCenter,QString::number(m_height.value(i)));
            painter.drawText((width()/19.0)+2*((width()/19.0)*4),i*((qreal)(height()-48)/m_row)+48,(width()/19.0),((qreal)height()/m_row),Qt::AlignLeft|Qt::AlignCenter,QString::number(m_height.value(i)));
            painter.drawText(2*(width()/19.0)+4*((width()/19.0)*4),i*((qreal)(height()-48)/m_row)+48,(width()/19.0),((qreal)height()/m_row),Qt::AlignLeft|Qt::AlignCenter,QString::number(m_height.value(i)));

            //画曲线
            if(m_dataList.value(i).size()!=0)
            {
                QPointF points[4*WAVE_LENGHT_VALUE];
                for(int j=0;j<WAVE_LENGHT_VALUE;j++)
                {
                    points[j] = QPointF(-1*((qreal)j/WAVE_LENGHT_VALUE)*((width()/19.0)*4)+5*(width()/19.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*2;j++)
                {
                    points[j] = QPointF(-1*((qreal)(j-WAVE_LENGHT_VALUE)/WAVE_LENGHT_VALUE)*((width()/19.0)*4)+14*(width()/19.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=2*WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*3;j++)
                {
                    points[j] = QPointF(((qreal)(j-WAVE_LENGHT_VALUE*2)/WAVE_LENGHT_VALUE)*((width()/19.0)*4)+5*(width()/19.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=3*WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*4;j++)
                {
                    points[j] = QPointF(((qreal)(j-WAVE_LENGHT_VALUE*3)/WAVE_LENGHT_VALUE)*((width()/19.0)*4)+14*(width()/19.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }

                //0--400 X
                painter.drawPolyline(points,WAVE_LENGHT_VALUE);
                //400--800 Y
                painter.drawPolyline(points+WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
                //800--1200 X'
                painter.drawPolyline(points+2*WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
                //800--1200 Y'
                painter.drawPolyline(points+3*WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
            }
        }
    }else if(m_showMode == left || m_showMode == right)
    {
        painter.drawLine(QPointF(0,36),QPointF(width(),36));
        if(m_showMode==left)
        {
            painter.drawText(QRectF((width()/10.0),0,4*(width()/10.0),36),Qt::AlignHCenter|Qt::AlignTop,"X");
            painter.drawText(QRectF(5*(width()/10.0),0,4*(width()/10.0),36),Qt::AlignHCenter|Qt::AlignTop,"X'");
        }else
        {
            painter.drawText(QRectF((width()/10.0),0,4*(width()/10.0),36),Qt::AlignHCenter|Qt::AlignTop,"Y");
            painter.drawText(QRectF(5*(width()/10.0),0,4*(width()/10.0),36),Qt::AlignHCenter|Qt::AlignTop,"Y'");
        }

        for(int i=0;i<=5;i++)
        {
            painter.drawText(0,10,2*i*(4*width()/10.0/5.0)+10*(width()/10.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
            painter.drawText(0,10,-2*i*(4*width()/10.0/5.0)+10*(width()/10.0),28,Qt::AlignCenter,QString::number(i*(m_maxNumber-m_miniNumber)/5.0+m_miniNumber));
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF((width()/10.0)+4*(width()/10.0)*(i/5),0),QPointF((width()/10.0)+4*(width()/10.0)*(i/5),12));
                painter.drawLine(QPointF((width()/10.0)+4*(width()/10.0)*(i/5),30),QPointF((width()/10.0)+4*(width()/10.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF((width()/10.0)+4*(width()/10.0)*(i/5.0),30),QPointF((width()/10.0)+4*(width()/10.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<=5;i++)
        {
            if(i%5==0)
            {
                painter.drawLine(QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5),0),QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5),12));
                painter.drawLine(QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5),30),QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5),36));
            }else
            {
                painter.drawLine(QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5.0),30),QPointF(5*(width()/10.0)+4*(width()/10.0)*(i/5.0),36));
            }
        }

        for(int i=0;i<m_dataList.size();i++)
        {
            //标注深度
            painter.drawText(0,i*((qreal)(height()-48)/m_row)+48,(width()/10.0),((qreal)(height()-48)/m_row),Qt::AlignLeft|Qt::AlignCenter,QString::number(m_height.value(i)));
            painter.drawText(9*(width()/10.0),i*((qreal)(height()-48)/m_row)+48,(width()/10.0),((qreal)(height()-48)/m_row),Qt::AlignLeft|Qt::AlignCenter,QString::number(m_height.value(i)));

            //画曲线
            if(m_dataList.value(i).size()!=0)
            {
                QPointF points[4*WAVE_LENGHT_VALUE];
                for(int j=0;j<WAVE_LENGHT_VALUE;j++)
                {
                    points[j] = QPointF(-1*((qreal)j/WAVE_LENGHT_VALUE)*((width()/10.0)*4)+5*(width()/10.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*2;j++)
                {
                    points[j] = QPointF(-1*((qreal)(j-WAVE_LENGHT_VALUE)/WAVE_LENGHT_VALUE)*((width()/10.0)*4)+5*(width()/10.0),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=2*WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*3;j++)
                {
                    points[j] = QPointF(((qreal)(j-WAVE_LENGHT_VALUE*2)/WAVE_LENGHT_VALUE)*((width()/10.0)*4)+((width()/10.0)*5),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }
                for(int j=3*WAVE_LENGHT_VALUE;j<WAVE_LENGHT_VALUE*4;j++)
                {
                    points[j] = QPointF(((qreal)(j-WAVE_LENGHT_VALUE*3)/WAVE_LENGHT_VALUE)*((width()/10.0)*4)+((width()/10.0)*5),-1*m_dataList.value(i).value(j)*m_scale+((qreal)(height()-48)/m_row)*(0.5+i)+48);
                }

                if(m_showMode == left)
                {
                    painter.drawPolyline(points,WAVE_LENGHT_VALUE);
                    painter.drawPolyline(points+2*WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
                }else
                {
                    painter.drawPolyline(points+WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
                    painter.drawPolyline(points+3*WAVE_LENGHT_VALUE,WAVE_LENGHT_VALUE);
                }
            }
        }
    }
}

void WaveWidget::setMiniNumber(int mini)
{
    m_miniNumber = mini;
    update();
}

void WaveWidget::setMaxNumber(int max)
{
    m_maxNumber = max;
    update();
}

void WaveWidget::setRowCount(int row)
{
    m_row = row;
    update();
}

int WaveWidget::getRowCount()
{
    return m_row;
}

void WaveWidget::ShowData(const QList<QList<qint16> > &dataList, const QList<qreal> &height)
{
    if(dataList.size() > m_row)
    {
        return;
    }
    m_dataList.clear();
    m_height.clear();
    m_dataList.append(dataList);
    m_height.append(height);
    update();
}

void WaveWidget::setScale(qreal value)
{
    m_scale = value;
    update();
}

void WaveWidget::SetProfile(int profile)
{
    switch(profile)
    {
    case 0:
        m_showMode = left;
        break;
    case 1:
        m_showMode = right;
        break;
    case 2:
        m_showMode = all;
        break;
    }
    update();
}

void WaveWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    static int i=0;
    if(i%2==0)
    {
        if(event->pos().x()>width()/2)
            SetProfile(right);
        else
            SetProfile(left);
    }else
        SetProfile(all);
    i++;
}
