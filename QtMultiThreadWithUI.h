#pragma once
#include "ui_QtMultiThreadWithUI.h"

#include <vector>

#include <QtWidgets/QMainWindow>
#include <QMutex>
#include <QString>

#include "UpdateUiTask.h"

class QtMultiThreadWithUI : public QMainWindow
{
    Q_OBJECT

public:
    QtMultiThreadWithUI(QWidget *parent = nullptr);

protected slots:
    void on_pushButton_clicked();
    void on_stopButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::QtMultiThreadWithUIClass _ui;

    std::vector<std::shared_ptr<UpdateUiTask>> _threads;
    std::shared_ptr<UpdateUiTask> _threadOnFinished;
    int _threadCounts = 3;

    int _finishedThreadCount = 0;
    QMutex _mutex;

    void RunThreadOnFinished();
    void StopAllThreads();

private slots:
    void PrintCount(int threadNo, int val);
    void HandleResult(int threadNo, bool success);
};
