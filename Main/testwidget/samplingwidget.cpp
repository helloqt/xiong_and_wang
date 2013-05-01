#include "samplingwidget.h"
#include "ui_samplingwidget.h"
#include "UserDef.h"
#ifdef MY_DEBUG
#include <QDebug>
#endif
SamplingWidget::SamplingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SamplingWidget)
{
    ui->setupUi(this);
    showButton = true;
    update();
    showButton =false;
    valueIndex = 1;
    leftMousePress = false;

    //设置可以抓取鼠标移动时间
    setMouseTracking(true);

    //增益默认初始值为0.03
    m_scale = 0.3;
}

SamplingWidget::~SamplingWidget()
{
    delete ui;
}

/**************************************************************
  *类型：保护成员函数、虚函数
  *功能：实现波形的绘制、用户操作button的绘制
        button的位置由value决定
        1<=value<=400
        *****************************************************/
void SamplingWidget::paintEvent(QPaintEvent */* event*/)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawLine(QPointF(((qreal)valueIndex/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH)+BUTTONWIDTH/2,0),QPointF(((qreal)valueIndex/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH)+BUTTONWIDTH/2,height()));

    if(showButton)
    {
        painter.drawLine(QPointF(BUTTONWIDTH/2,height()/2),QPointF(width()-BUTTONWIDTH/2,height()/2));
        painter.drawPixmap(((qreal)valueIndex/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH),height()/2-BUTTONWIDTH/2,BUTTONWIDTH,BUTTONWIDTH,QPixmap("./img/spliterButton.png"));
    }

    painter.setPen(Qt::black);
    if(m_data.size()!=0)
    {
        QPointF points[WAVE_LENGHT_VALUE];
        for(int i=0;i<m_data.size();i++)
        {
            points[i] = QPointF(((qreal)i/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH)+BUTTONWIDTH/2,m_data.value(i)*m_scale+height()/2);
        }
        painter.drawPolyline(points,m_data.size());
    }
}

/*********************************************************
  *类型：保护成员函数、虚函数
  *功能：鼠标按下button并移动时，改变value值并重绘
        实现button的拖动
        鼠标从button上移动开后，通过控制showButton的值使button消失
        **************************************************/
void SamplingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftMousePress==false)
    {
        if(isPointInRect(event->pos(),QRect(((qreal)valueIndex/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH),height()/2-BUTTONWIDTH/2,BUTTONWIDTH,BUTTONWIDTH)))
        {
            showButton = true;
        }
        else
            showButton=false;
    }else
    {
        int temp  = (qreal)(event->pos().x()-BUTTONWIDTH/2)/(width()-BUTTONWIDTH)*WAVE_LENGHT_VALUE;
        if(temp<=0)
            temp = 1;
        if(temp>WAVE_LENGHT_VALUE)
            temp = WAVE_LENGHT_VALUE;
        valueIndex = temp;
        if (m_data.count())
        {
           emit valueChanged(valueIndex, m_data.at(valueIndex - 1));
        }
        else
            emit valueChanged(valueIndex, 0);

    }
    update();
}

//通过leftMousePress标记鼠标按下
void SamplingWidget::mousePressEvent(QMouseEvent *event)
{
    if(leftMousePress==false)
    {
        if(isPointInRect(event->pos(),QRect(((qreal)valueIndex/WAVE_LENGHT_VALUE)*(width()-BUTTONWIDTH),height()/2-BUTTONWIDTH/2,BUTTONWIDTH,BUTTONWIDTH)))
            leftMousePress = true;
    }
}
//通过leftMousePress标记鼠标弹起
void SamplingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    leftMousePress = false;
}

/*****************************************************
  *类型：私有函数
  *功能：判断点p是否在矩形r内
        是返回true,否返回flase
        **********************************************/
bool SamplingWidget::isPointInRect(QPoint p, QRect r)
{
    if(p.x()>r.left()&&p.x()<r.right()&&p.y()<r.bottom()&&p.y()>r.top())
        return true;
    else
        return false;
}

/*********************************************************
  *类型：公有成员函数
  *功能：刷新数据，重画数据显示曲线
  ******************************************************/
void SamplingWidget::updateDate(QList<qint16> data)
{
    emit valueChanged(valueIndex, data.at(valueIndex));
    m_data = data;
    update();
}

/*********************************************************
  *类型：公有成员函数
  *功能：返回用户选择的数据
         返回value:1--400
  ******************************************************/
int SamplingWidget::getValue()
{
    return valueIndex;
}

/*********************************************************
  *类型：公有成员函数
  *功能：设置增益
  ******************************************************/
void SamplingWidget::setScale(qreal value)
{
    m_scale = value;
    update();
}
