#ifndef TESTVIEWDLG_H
#define TESTVIEWDLG_H

#include "ui_testviewstartdlg.h"
#include <QString>
class testViewStartDlg : public QDialog, private Ui::testViewStartDlg
{
    Q_OBJECT
    
public:
    explicit testViewStartDlg(QWidget *parent = 0);
    int GetProfile();
    QString GetXDistance();
    QString GetYDistance();
};

#endif // TESTVIEWDLG_H
