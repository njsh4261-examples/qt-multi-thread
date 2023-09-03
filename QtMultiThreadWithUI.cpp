#include "QtMultiThreadWithUI.h"

QtMultiThreadWithUI::QtMultiThreadWithUI(QWidget *parent)
    : QMainWindow(parent)
{
    _ui.setupUi(this);
}

void QtMultiThreadWithUI::on_pushButton_clicked()
{
    _finishedThreadCount = 0;
    for (int i = 1; i <= _threadCounts; i++)
    {
        auto thread = std::make_shared<UpdateUiTask>(i, 100, this);

        connect(thread.get(), &UpdateUiTask::UpdateUI, this, &QtMultiThreadWithUI::PrintCount);
        connect(thread.get(), &UpdateUiTask::SendResult, this, &QtMultiThreadWithUI::HandleResult);
        connect(
            thread.get(), &UpdateUiTask::finished, this,
            [=]()
            {
                _ui.textEdit->append(QString("Thread %1 finished.").arg(QString::number(thread->GetThreadNo())));

                disconnect(thread.get(), &UpdateUiTask::UpdateUI, this, &QtMultiThreadWithUI::PrintCount);
                disconnect(thread.get(), &UpdateUiTask::SendResult, this, &QtMultiThreadWithUI::HandleResult);

                _mutex.lock();
                
                _finishedThreadCount++;
                if (_finishedThreadCount == _threadCounts)
                {
                    _threads.clear();
                    RunThreadOnFinished();
                }

                _mutex.unlock();
            }
        );

        _threads.emplace_back(thread);
    }

    for (auto& thread : _threads)
    {
        thread->start();
    }
}

void QtMultiThreadWithUI::on_stopButton_clicked()
{
    _ui.textEdit->append("User stopped all threads.");
    StopAllThreads();
}

void QtMultiThreadWithUI::on_clearButton_clicked()
{
    _ui.textEdit->clear();
}

void QtMultiThreadWithUI::RunThreadOnFinished()
{
    _ui.textEdit->append("\nRun after every other threads are finished.");

    _threadOnFinished = std::make_shared<UpdateUiTask>(4, 10, this);
    connect(_threadOnFinished.get(), &UpdateUiTask::UpdateUI, this, &QtMultiThreadWithUI::PrintCount);
    connect(_threadOnFinished.get(), &UpdateUiTask::SendResult, this, &QtMultiThreadWithUI::HandleResult);
    connect(
        _threadOnFinished.get(), &UpdateUiTask::finished, this,
        [=]()
        {
            disconnect(_threadOnFinished.get(), &UpdateUiTask::UpdateUI, this, &QtMultiThreadWithUI::PrintCount);
            disconnect(_threadOnFinished.get(), &UpdateUiTask::SendResult, this, &QtMultiThreadWithUI::HandleResult);

            _threadOnFinished = nullptr;
        }
    );

    _threadOnFinished->start();
}

void QtMultiThreadWithUI::StopAllThreads()
{
    for (auto& thread : _threads)
    {
        if (thread != nullptr)
        {
            thread->Cancel();
        }
    }
}

void QtMultiThreadWithUI::PrintCount(int threadNo, int val)
{
    _ui.textEdit->append(
        QString("[Thread %1] Count: %2")
            .arg(QString::number(threadNo))
            .arg(QString::number(val))
    );
}

void QtMultiThreadWithUI::HandleResult(int threadNo, bool success)
{
    if (success)
    {
        _ui.textEdit->append(QString("Thread %1 end successfully.").arg(threadNo));
    }
    else
    {
        _ui.textEdit->append(QString("Thread %1 failed; Stop all threads.").arg(threadNo));
        StopAllThreads();
    }
}
