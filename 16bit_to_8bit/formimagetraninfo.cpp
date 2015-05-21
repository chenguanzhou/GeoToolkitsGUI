#include "formimagetraninfo.h"
#include "ui_formimagetraninfo.h"
#include <QtCore>

FormImageTranInfo::FormImageTranInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImageTranInfo)
{
    ui->setupUi(this);
}

FormImageTranInfo::~FormImageTranInfo()
{
    delete ui;
}
