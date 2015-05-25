#include "translatedialog.h"
#include <QtCore>
#include <QStringListModel>
#include <QProgressBar>
#include <QLabel>

#include "transthread.h"

TranslateDialog::TranslateDialog(QWidget *parent)
    :GeoDialog(parent)
{

}

TranslateDialog::~TranslateDialog()
{

}

void TranslateDialog::start()
{
    QStringList inputList = modelInput->stringList();
    QStringList outputList = modelOutput->stringList();

    int count = inputList.count();
    if (count == 0 || count != outputList.count())
        return;

    TransThread *tFirst = new TransThread(inputList[0],outputList[0],this);
    connect(tFirst,SIGNAL(progressChanged(int)),progressBar(),SLOT(setValue(int)));
    connect(tFirst,SIGNAL(progressLabelChanged(QString)),progressLabel(),SLOT(setText(QString)));

    TransThread *tLast = tFirst;
    for (int i=1;i<count;++i)
    {
        TransThread *tNew = new TransThread(inputList[i],outputList[i],this);
        connect(tNew,SIGNAL(progressChanged(int)),progressBar(),SLOT(setValue(int)));
        connect(tNew,SIGNAL(progressLabelChanged(QString)),progressLabel(),SLOT(setText(QString)));
        connect(tLast,SIGNAL(finished()),tNew,SLOT(start()));
        tLast = tNew;
    }

    tFirst->start();
}

