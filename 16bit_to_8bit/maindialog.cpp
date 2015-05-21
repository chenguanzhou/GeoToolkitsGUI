#include "maindialog.h"
#include "ui_maindialog.h"

#include <QtCore>
#include <QStringListModel>
#include <QFileDialog>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    modelOutput(new QStringListModel(this))
{
    ui->setupUi(this);
    ui->listViewOutput->setModel(modelOutput);

    connect(ui->pushButtonInputBrowse,SIGNAL(clicked()),SLOT(onPushButtonInputBrowseClicked()));
    connect(ui->pushButtonOutputBrowse,SIGNAL(clicked()),SLOT(onPushButtonOutputBrowseClicked()));

    connect(ui->lineEditInput,SIGNAL(textChanged(QString)),SLOT(updateListViewInput()));
    connect(ui->lineEditInput,SIGNAL(textChanged(QString)),SLOT(updateListViewOutput()));
    connect(ui->lineEditOutput,SIGNAL(textChanged(QString)),SLOT(updateListViewOutput()));

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
                QString::null,
                tr("Supported formats(*.tif *.img)"));

    if (files.isEmpty())
        return;

    ui->lineEditInput->setText(files.join("|"));
}

void MainDialog::onPushButtonOutputBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Directort of Output Images"));

    if (dir.isEmpty())
        return;

    ui->lineEditOutput->setText(dir);
}

void MainDialog::updateListViewInput()
{

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
        while(outputFiles.contains(p)||QFileInfo(p).exists())
        {
            QFileInfo info(p);
            p = QFileInfo(info.absolutePath()+"/"+info.baseName()+"_new."+info.completeSuffix()).absoluteFilePath();
        }
        outputFiles.push_back(p);
    }

    modelOutput->setStringList(outputFiles);
}
