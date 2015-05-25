#include "ecwdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ECWDialog w;
    w.setWindowTitle(ECWDialog::tr("ECW Generator"));
    w.show();

    return a.exec();
}
