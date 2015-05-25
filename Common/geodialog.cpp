#include "geodialog.h"
#include "ui_geodialog.h"

#include <QtCore>
#include <QStringListModel>
#include <QFileDialog>

GeoDialog::GeoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeoDialog),
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

GeoDialog::~GeoDialog()
{
    delete ui;
}

QProgressBar *GeoDialog::progressBar()
{
    return ui->progressBar;
}

QLabel *GeoDialog::progressLabel()
{
    return ui->labelProgress;
}

void GeoDialog::onPushButtonInputBrowseClicked()
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

void GeoDialog::onPushButtonOutputBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Directort of Output Images"),
                ui->lineEditOutput->text());

    if (dir.isEmpty())
        return;

    ui->lineEditOutput->setText(dir);
}

void GeoDialog::updateListViewInput()
{
    QStringList files = ui->lineEditInput->text().split("|");
    modelInput->setStringList(files);
}

void GeoDialog::updateListViewOutput()
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
