#ifndef ECWTHREAD_H
#define ECWTHREAD_H

#include <QThread>

class ECWThread : public QThread
{
    Q_OBJECT
public:
    explicit ECWThread(QString input,QString output,QObject *parent = 0);
    ~ECWThread();

    void run();
signals:

public slots:

private:
    QString inputPath;
    QString outputPath;
};

#endif // ECWTHREAD_H
