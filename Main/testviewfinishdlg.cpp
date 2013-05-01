#include "testviewfinishdlg.h"

testViewFinishDlg::testViewFinishDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

QString testViewFinishDlg::GetSpeed()
{
    return labelTestFinishDlgSpeed->text();
}


void testViewFinishDlg::SetSpeed(qreal speed)
{
    labelTestFinishDlgSpeed->setText(QString::number(speed));
}
