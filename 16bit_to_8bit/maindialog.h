#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace Ui {
class MainDialog;
}

class QStringListModel;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

public slots:
    void onPushButtonInputBrowseClicked();
    void onPushButtonOutputBrowseClicked();

    void updateListViewInput();
    void updateListViewOutput();

    void start();

private:
    Ui::MainDialog *ui;
    QStringListModel *modelInput;
    QStringListModel *modelOutput;
};

#endif // MAINDIALOG_H
