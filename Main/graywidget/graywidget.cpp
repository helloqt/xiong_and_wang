#include "graywidget.h"

GrayWidget::GrayWidget(QWidget *parent) :
    QWidget(parent)
{
    m_mainLayout = new QHBoxLayout(this);

    m_leftWidget = new GrayWidgetindex;
    QStringList leftTitile;
    leftTitile<<"X"<<"X'";
    m_leftWidget->setTitleText(leftTitile);
    m_leftWidget->setType(true);

    m_rightWidget = new GrayWidgetindex;
    QStringList rightTitile;
    rightTitile<<"Y"<<"Y'";
    m_rightWidget->setTitleText(rightTitile);
    m_rightWidget->setType(false);

    m_mainLayout->addWidget(m_leftWidget);
    m_mainLayout->addWidget(m_rightWidget);
    m_mainLayout->setSpacing(2);
    showSolo = false;
    connect(m_leftWidget,SIGNAL(DoubleClicked()),this,SLOT(showLeft()));
    connect(m_rightWidget,SIGNAL(DoubleClicked()),this,SLOT(showRight()));

    isAtBegin = false;
    isAtEnd = false;
    //m_heightPerPage = m_leftWidget->height()-80;
    m_heightPerPage = 400;

    m_currentPage = 0;

}

void GrayWidget::loadData(QList<qreal> *height, QList<QList<qint16> > *gray)
{
    m_pHeight = height;
    m_pGrayData = gray;
    showPage(0);
}

void GrayWidget::showPage(int page)
{
    if(page == 0)
        isAtBegin = true;
    else
        isAtBegin = false;

    int numberPerPage = m_heightPerPage;
    if((page+1)*numberPerPage<m_pHeight->size())
        isAtEnd = false;
    else
        isAtEnd = true;

    m_leftWidget->setData(m_pGrayData->mid(page*numberPerPage,numberPerPage),m_pHeight->mid(page*numberPerPage,numberPerPage));
    m_rightWidget->setData(m_pGrayData->mid(page*numberPerPage,numberPerPage),m_pHeight->mid(page*numberPerPage,numberPerPage));

    m_currentPage = page;
}

bool GrayWidget::PageUp()
{
    if (canUp())
    {
        showPage(--m_currentPage);
        return true;
    }
    return false;
}

bool GrayWidget::PageDown()
{
    if (canDown())
    {
        showPage(++m_currentPage);
        return true;
    }
    return false;
}

void GrayWidget::showLeft()
{
    if(showSolo)
    {
        m_rightWidget->show();
        m_mainLayout->addWidget(m_rightWidget);
        showSolo = false;
    }
    else
    {
        m_rightWidget->hide();
        m_mainLayout->removeWidget(m_rightWidget);
        showSolo =true;
    }
}

void GrayWidget::showRight()
{
    if(showSolo)
    {
        m_leftWidget->show();
        m_mainLayout->addWidget(m_leftWidget);
        showSolo = false;
    }
    else
    {
        m_leftWidget->hide();
        m_mainLayout->removeWidget(m_leftWidget);
        showSolo =true;
    }
}

bool GrayWidget::canUp()
{
    return !isAtBegin;
}

bool GrayWidget::canDown()
{
    return !isAtEnd;
}

void GrayWidget::setGraduation(int mini, int max)
{
    m_leftWidget->setMiniNumber(mini);
    m_rightWidget->setMiniNumber(mini);
    m_leftWidget->setMaxNumber(max);
    m_rightWidget->setMaxNumber(max);
}

void GrayWidget::setHeight(int height)
{
    m_heightPerPage = height;
    showPage(m_currentPage);
}

void GrayWidget::ShowCurrentPage()
{
    showPage(m_currentPage);
}

int GrayWidget::GetCurrentPage()
{
    return m_currentPage;
}
