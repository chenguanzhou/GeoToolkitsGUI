#include "maindialog.h"
#include "ui_maindialog.h"

#include <QtCore>
#include <QStringListModel>
#include <QFileDialog>
#include "transthread.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    modelInput(new QStringListModel(this)),
    modelOutput(new QStringListModel(this))
{
    ui->setupUi(this);

    ui->listViewInput->setModel(modelInput);
    ui->listViewOutput->setModel(modelOutput);


    connect(ui->pushButtonInputBrowse,SIGNAL(clicked()),SLOT(onPushButtonInputBrowseClicked()));
    connect(ui->pushButtonOutputBrowse,SIGNAL(clicked()),SLOT(onPushButtonOutputBrowseClicked()));

    connect(ui->lineEditInput,SIGNAL(textChanged(QString)),SLOT(updateListViewInput()));
    connect(ui->lineEditInput,SIGNAL(textChanged(QString)),SLOT(updateListViewOutput()));
    connect(ui->lineEditOutput,SIGNAL(textChanged(QString)),SLOT(updateListViewOutput()));

    connect(ui->pushButtonStart,SIGNAL(clicked()),SLOT(start()));
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::onPushButtonInputBrowseClicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                tr("Open Images"),
                ui->lineEditInput->text(),
                tr("Supported formats(*.tif *.img)"));

    if (files.isEmpty())
        return;

    ui->lineEditInput->setText(files.join("|"));
}

void MainDialog::onPushButtonOutputBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Directort of Output Images"),
                ui->lineEditOutput->text());

    if (dir.isEmpty())
        return;

    ui->lineEditOutput->setText(dir);
}

void MainDialog::updateListViewInput()
{
    QStringList files = ui->lineEditInput->text().split("|");
    modelInput->setStringList(files);
}

void MainDialog::updateListViewOutput()
{
    QStringList files = ui->lineEditInput->text().split("|");
    QString dir = ui->lineEditOutput->text();

    if (dir.isEmpty())
        dir = QDir::currentPath();

    QStringList outputFiles;

    foreach (QString filePath, files) {
        QString p = dir+ "/" + QFileInfo(filePath).fileName();
        p = QFileInfo(p).absoluteFilePath();
        while(outputFiles.contains(p)||QFile(p).exists())
        {
            QFileInfo info(p);
            p = QFileInfo(info.absolutePath()+"/"+info.baseName()+"_new."+info.completeSuffix()).absoluteFilePath();
        }
        outputFiles.push_back(p);
    }

    modelOutput->setStringList(outputFiles);
}

void MainDialog::start()
{
    QStringList inputList = modelInput->stringList();
    QStringList outputList = modelOutput->stringList();

    int count = inputList.count();
    if (count == 0 || count != outputList.count())
        return;

    TransThread *tFirst = new TransThread(inputList[0],outputList[0],this);
    connect(tFirst,SIGNAL(progressChanged(int)),ui->progressBar,SLOT(setValue(int)));
    connect(tFirst,SIGNAL(progressLabelChanged(QString)),ui->labelProgress,SLOT(setText(QString)));

    TransThread *tLast = tFirst;
    for (int i=1;i<count;++i)
    {
        TransThread *tNew = new TransThread(inputList[i],outputList[i],this);
        connect(tNew,SIGNAL(progressChanged(int)),ui->progressBar,SLOT(setValue(int)));
        connect(tNew,SIGNAL(progressLabelChanged(QString)),ui->labelProgress,SLOT(setText(QString)));
        connect(tLast,SIGNAL(finished()),tNew,SLOT(start()));
        tLast = tNew;
    }

    tFirst->start();
}
