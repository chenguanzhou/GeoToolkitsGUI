#ifndef QIMAGEINFOMODEL_H
#define QIMAGEINFOMODEL_H

#include <QAbstractListModel>

class QImageInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum{
        PATH_ROLE = Qt::UserRole+1,
        BAND_ROLE = Qt::UserRole+2,
        TYPE_ROLE = Qt::UserRole+3,
    };
    explicit QImageInfoModel(QObject *parent = NULL);
    ~QImageInfoModel();

    void setFiles(QStringList filesPath);
};

#endif // QIMAGEINFOMODEL_H
