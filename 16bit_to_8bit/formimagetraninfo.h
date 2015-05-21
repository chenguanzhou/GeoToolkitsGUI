#ifndef FORMIMAGETRANINFO_H
#define FORMIMAGETRANINFO_H

#include <QWidget>

namespace Ui {
class FormImageTranInfo;
}

class FormImageTranInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FormImageTranInfo(QWidget *parent = 0);
    ~FormImageTranInfo();

private:
    Ui::FormImageTranInfo *ui;
};

#endif // FORMIMAGETRANINFO_H
