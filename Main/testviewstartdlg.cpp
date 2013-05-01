#include "testviewstartdlg.h"

testViewStartDlg::testViewStartDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}


int testViewStartDlg::GetProfile()
{
    return comboTestStartDlgProfile->currentIndex();
}

QString testViewStartDlg::GetXDistance()
{
    return lEditTestStartDlgXDistance->text();
}

QString testViewStartDlg::GetYDistance()
{
    return lEditTestStartDlgYDistance->text();
}
