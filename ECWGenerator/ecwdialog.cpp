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
    connect(tFirst,SIGNAL(progressChanged(int)),progressBar(),SLOT(setValue(int)));
    connect(tFirst,SIGNAL(progressLabelChanged(QString)),progressLabel(),SLOT(setText(QString)));

    ECWThread *tLast = tFirst;
    for (int i=1;i<count;++i)
    {
        ECWThread *tNew = new ECWThread(inputList[i],outputList[i],this);
        connect(tNew,SIGNAL(progressChanged(int)),progressBar(),SLOT(setValue(int)));
        connect(tNew,SIGNAL(progressLabelChanged(QString)),progressLabel(),SLOT(setText(QString)));
        connect(tLast,SIGNAL(finished()),tNew,SLOT(start()));
        tLast = tNew;
    }

    tFirst->start();
}

void ECWDialog::updateListViewOutput()
{
    QStringList files = getOutputPathsFromLineEdit();
    QString dir = getDirFromLineEdit();

    if (dir.isEmpty())
        dir = QDir::currentPath();

    QStringList outputFiles;

    foreach (QString filePath, files) {
        QString p = dir+ "/" + QFileInfo(filePath).baseName()+".jp2";
        p = QFileInfo(p).absoluteFilePath();
        while(outputFiles.contains(p)||QFile(p).exists())
        {
            QFileInfo info(p);
            p = QFileInfo(info.absolutePath()+"/"+info.baseName()+"_new.jp2").absoluteFilePath();
        }
        outputFiles.push_back(p);
    }

    modelOutput->setStringList(outputFiles);
}
