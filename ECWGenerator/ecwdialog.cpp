#include "ecwdialog.h"
#include <QtCore>
#include <QStringListModel>
#include <QProgressBar>
#include <QLabel>

#include "ecwthread.h"

ECWDialog::ECWDialog(QWidget *parent)
    : GeoDialog(parent)
{
}

ECWDialog::~ECWDialog()
{

}

void ECWDialog::start()
{
    QStringList inputList = modelInput->stringList();
    QStringList outputList = modelOutput->stringList();

    int count = inputList.count();
    if (count == 0 || count != outputList.count())
        return;

    ECWThread *tFirst = new ECWThread(inputList[0],outputList[0],this);
    tFirst->start();
}
