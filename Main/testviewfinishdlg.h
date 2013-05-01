#ifndef TESTVIEWFINISHDLG_H
#define TESTVIEWFINISHDLG_H

#include "ui_testviewfinishdlg.h"
#include <QString>
class testViewFinishDlg : public QDialog, private Ui::testViewFinishDlg
{
    Q_OBJECT
    
public:
    explicit testViewFinishDlg(QWidget *parent = 0);
    QString GetSpeed();
    void SetSpeed(qreal speed);
};

#endif // TESTVIEWFINISHDLG_H
