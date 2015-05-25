#include "ecwthread.h"
#include <QtCore>
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
//    GDALAllRegister();
}

ECWThread::~ECWThread()
{

}

void ECWThread::run()
{
    try{
        GDALDataset *poSrcDS = (GDALDataset *)GDALOpen(TO8F(inputPath),GA_ReadOnly);
        if (poSrcDS==NULL)
            throw tr("Open input file failed!");

        if (poSrcDS->GetRasterBand(1)->GetRasterDataType()!=GDT_Byte)
            throw tr("Data type of input file is not byte!");

        GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("ECW");
        if (poDriver == NULL)
            throw tr("Data driver of ECW not found!");

        qDebug()<<"you";
    }
    catch (const QString &msg)
    {
        qDebug()<<msg;
    }
}

