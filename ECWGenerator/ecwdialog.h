#ifndef ECWDIALOG_H
#define ECWDIALOG_H

#include "../Common/geodialog.h"

class ECWDialog : public GeoDialog
{
    Q_OBJECT

public:
    ECWDialog(QWidget *parent = 0);
    ~ECWDialog();

    void start();

    void updateListViewOutput();
};

#endif // ECWDIALOG_H
