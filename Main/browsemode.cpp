#include "browsemode.h"

BrowseMode::BrowseMode(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void BrowseMode::on_btnBrowseFile_clicked()
{
    emit showMode(0);
}

void BrowseMode::on_btnBrowseData_clicked()
{
    emit showMode(1);
}
