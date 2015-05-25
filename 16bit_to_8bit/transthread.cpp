#include "transthread.h"
#include <QtCore>
#include <gdal_priv.h>
#include "ui_geodialog.h"

#if defined(GDAL_VERSION_NUM) && GDAL_VERSION_NUM >= 1800
#define TO8F(x) (x).toUtf8().constData()
#else
#define TO8F(x) QFile::encodeName( x ).constData()
#endif

TransThread::TransThread(QString input, QString output, QObject *parent)
    :inputPath(input),outputPath(output),QThread(parent)
{
    GDALAllRegister();
}

TransThread::~TransThread()
{

}

void TransThread::run()
{
    try
    {
        GDALDataset *poSrcDS = (GDALDataset *)GDALOpen(TO8F(inputPath),GA_ReadOnly);
        if (poSrcDS==NULL)
            throw tr("Open input file failed!");

        int width = poSrcDS->GetRasterXSize();
        int height = poSrcDS->GetRasterYSize();
        int bandCount = poSrcDS->GetRasterCount();
        GDALDataType dataType = poSrcDS->GetRasterBand(1)->GetRasterDataType();
        if (dataType!=GDT_Int16 && dataType!=GDT_UInt16)
            throw tr("Datatype of input file wrong!");

        GDALDataset *poDstDS;

        poDstDS = poSrcDS->GetDriver()->Create( TO8F(outputPath), width, height, bandCount, GDT_Byte,NULL );
        if (poDstDS==NULL)
            throw tr("Create output file failed!");

        poDstDS->SetProjection(poSrcDS->GetProjectionRef());
        QVector<double> geoTransform(6);
        poSrcDS->GetGeoTransform(&geoTransform[0]);
        poDstDS->SetGeoTransform(&geoTransform[0]);

        for(int k=0;k<bandCount;++k)
        {
            emit progressLabelChanged(tr("Band%1").arg(k+1));
            GDALRasterBand *bandIn = poSrcDS->GetRasterBand(k+1);
            GDALRasterBand *bandOut = poDstDS->GetRasterBand(k+1);

            bandOut->SetNoDataValue(0);

            double dMinMax[2] = {0.0,255.0};
            bandIn->ComputeRasterMinMax(FALSE,dMinMax);
            double dMin = dMinMax[0];
            double dMax = dMinMax[1];
//            double dDiff = dMax-dMin;
            double ratio = 255./(dMax-dMin);

            //            QVector<ushort> dataIn(width);
            //            QVector<uchar> dataOut(width);
            //            for (int i=0;i<height;++i)
            //            {
            //                bandIn->RasterIO(GF_Read,0,i,width,1,&dataIn[0],width,1,GDT_UInt16,0,0);
            //                for(int j=0;j<width;++j)
            //                {
            //                    if (dataIn[j]==0)
            //                         dataOut[j] = 0;
            //                    else
            //                        dataOut[j] = (dataIn[j] - dMin)*255/dDiff;
            //                }
            //                bandOut->RasterIO(GF_Write,0,i,width,1,&dataOut[0],width,1,GDT_Byte,0,0);
            //                if (i%100==0)
            //                    emit progressChanged(i*100./height);
            //            }

            int nXBlocks, nYBlocks, nXBlockSize, nYBlockSize;
            int iXBlock, iYBlock;
            bandIn->GetBlockSize( &nXBlockSize, &nYBlockSize );
            nXBlocks = (width + nXBlockSize - 1) / nXBlockSize;
            nYBlocks = (height + nYBlockSize - 1) / nYBlockSize;
            int blocksCount = nXBlocks*nYBlocks;
            QVector<ushort> dataIn(nXBlockSize * nYBlockSize);
            QVector<uchar> dataOut(nXBlockSize * nYBlockSize);
            for( iYBlock = 0; iYBlock < nYBlocks; iYBlock++ )
            {
                for( iXBlock = 0; iXBlock < nXBlocks; iXBlock++ )
                {
                    int nXValid, nYValid;
                    bandIn->ReadBlock( iXBlock, iYBlock, &dataIn[0] );
                    if( (iXBlock+1) * nXBlockSize > width )
                        nXValid = width - iXBlock * nXBlockSize;
                    else
                        nXValid = nXBlockSize;
                    if( (iYBlock+1) * nYBlockSize > height )
                        nYValid = height - iYBlock * nYBlockSize;
                    else
                        nYValid = nYBlockSize;

                    // Collect the histogram counts.
                    auto iterIn = dataIn.begin();
                    auto iterOut = dataOut.begin();
                    for( int iY = 0; iY < nYValid; iY++,iterIn+=nXValid,iterOut+=nXValid)
                    {
//                        for( int iX = 0; iX < nXValid; iX++ )
//                        {
//                            ushort val = dataIn[iX + iY * nXBlockSize];
//                            dataOut[iX + iY * nXBlockSize] = val==0?0:val*ratio;
//                        }
                        std::transform(iterIn,iterIn+nXValid,iterOut,[=](const ushort &val)->uchar{
                            return val==0?0:val*ratio;
                        });
                    }

                    bandOut->WriteBlock(iXBlock, iYBlock, &dataOut[0]);
                }
                emit progressChanged((iXBlock+iYBlock*nXBlocks)* 100./blocksCount);
            }
        }

        emit progressChanged(100);
        emit progressLabelChanged(tr("Completed!"));
        GDALClose(poSrcDS);
        GDALClose(poDstDS);
    }
    catch (const QString &msg)
    {
        qDebug()<<msg;
    }
    qDebug()<<"Done!";
}

