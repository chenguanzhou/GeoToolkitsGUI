#include "translatedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TranslateDialog w;
    w.setWindowTitle(TranslateDialog::tr("16bit to 8bit convertor"));
    w.show();

    return a.exec();
}
