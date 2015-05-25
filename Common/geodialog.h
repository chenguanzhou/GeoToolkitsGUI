#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace Ui {
class GeoDialog;
}

class QStringListModel;
class QProgressBar;
class QLabel;

class GeoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeoDialog(QWidget *parent = 0);
    ~GeoDialog();

protected:
    QProgressBar *progressBar();
    QLabel *progressLabel();

public slots:
    void onPushButtonInputBrowseClicked();
    void onPushButtonOutputBrowseClicked();

    void updateListViewInput();
    void updateListViewOutput();

    virtual void start() = 0;

private:
    Ui::GeoDialog *ui;

protected:
    QStringListModel *modelInput;
    QStringListModel *modelOutput;
};

#endif // MAINDIALOG_H
