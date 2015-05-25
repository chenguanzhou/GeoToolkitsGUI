#ifndef TRANSLATEDIALOG_H
#define TRANSLATEDIALOG_H

#include "../Common/geodialog.h"

class TranslateDialog : public GeoDialog
{
    Q_OBJECT
public:
    explicit TranslateDialog(QWidget *parent = 0);
    ~TranslateDialog();

    void start();
};

#endif // TRANSLATEDIALOG_H
