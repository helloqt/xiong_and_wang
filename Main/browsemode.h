#ifndef BROWSEMODE_H
#define BROWSEMODE_H

#include "ui_browsemode.h"

class BrowseMode : public QWidget, private Ui::BrowseMode
{
    Q_OBJECT
    
public:
    explicit BrowseMode(QWidget *parent = 0);
signals:
    void showMode(int mode);
private slots:
    void on_btnBrowseFile_clicked();
    void on_btnBrowseData_clicked();
};

#endif // BROWSEMODE_H
