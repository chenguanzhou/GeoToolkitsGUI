#ifndef TRANSTHREAD_H
#define TRANSTHREAD_H

#include <QThread>

class TransThread : public QThread
{
    Q_OBJECT
public:
    explicit TransThread(QString input,QString output,QObject *parent = NULL);
    ~TransThread();

    void run() ;

signals:
    void progressChanged(int);
    void progressLabelChanged(QString);
private:
    QString inputPath;
    QString outputPath;
};

#endif // TRANSTHREAD_H
