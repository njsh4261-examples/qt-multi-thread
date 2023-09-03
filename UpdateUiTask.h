#pragma once

#include <QThread>
#include <QString>

class UpdateUiTask : public QThread
{
    Q_OBJECT

public:
    explicit UpdateUiTask(int threadNo, int countMax = 10, QWidget* parent = nullptr);

    int GetThreadNo() const { return _threadNo; }

    void Cancel();

protected:
    void run() override;

private:
    bool _cancelRequested = false;

    int _threadNo;
    int _countMax;
    int _count = 1;

    int _threshold = 5;

signals:
    void UpdateUI(int threadNo, int value);
    void SendResult(int threadNo, bool success);
};
