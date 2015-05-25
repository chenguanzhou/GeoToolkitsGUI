#include "ecwthread.h"
#include <QtCore>
#include <QProgressBar>
#include "gdal_priv.h"

#if defined(GDAL_VERSION_NUM) && GDAL_VERSION_NUM >= 1800
#define TO8F(x) (x).toUtf8().constData()
#else
#define TO8F(x) QFile::encodeName( x ).constData()
#endif


ECWThread::ECWThread(QString input, QString output, QObject *parent)
    :inputPath(input),
      outputPath(output),
      QThread(parent)
{
    GDALAllRegister();
}

ECWThread::~ECWThread()
{

}

int __stdcall progress(double dfComplete, const char *pszMessage, void *pProgressArg)
{
    ECWThread *bar = qobject_cast<ECWThread*>((QObject*)pProgressArg);
    if (bar)
        bar->updateProgressBar(dfComplete*100);
    return 1;
}

void ECWThread::run()
{
    try{
        GDALDataset *poSrcDS = (GDALDataset *)GDALOpen(TO8F(inputPath),GA_ReadOnly);
        if (poSrcDS==NULL)
            throw tr("Open input file failed!");

        if (poSrcDS->GetRasterBand(1)->GetRasterDataType()!=GDT_Byte)
            throw tr("Data type of input file is not byte!");

        GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("JP2ECW");
        if (poDriver == NULL)
            throw tr("Data driver of ECW not found!");

        char **papszOptions = NULL;
        papszOptions = CSLSetNameValue( papszOptions, "LARGE_OK", "YES" );
//        papszOptions = CSLSetNameValue( papszOptions, "TARGET", "0" );



        GDALDataset *poDstDS = poDriver->CreateCopy(TO8F(outputPath),poSrcDS,NULL,papszOptions,progress,this);
        if (poDstDS==NULL)
            throw tr("Generate ecw file failed!");
        GDALClose(poSrcDS);
        GDALClose(poDstDS);

        updateProgressBar(100);
    }
    catch (const QString &msg)
    {
        qDebug()<<msg;
    }
}

void ECWThread::updateProgressBar(int val)
{
    emit progressChanged(val);
}

