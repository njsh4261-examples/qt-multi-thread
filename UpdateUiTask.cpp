#include "UpdateUiTask.h"

UpdateUiTask::UpdateUiTask(int threadNo, int countMax, QWidget* parent)
    : _threadNo(threadNo), _countMax(countMax) {}

void UpdateUiTask::Cancel()
{
    _cancelRequested = true;
}

void UpdateUiTask::run()
{
    while ((_count <= _countMax)&& !_cancelRequested)
    {
        if (_count > _threshold && _threadNo == 2) // ���� �ִ� ������ ����
        {
            emit SendResult(_threadNo, false);
            return;
        }

        emit UpdateUI(_threadNo, _count++);
        msleep(100); // ���� ������ �̺�Ʈ ������ ����� �� �ִ� ������ �ð� ����
    }
    emit SendResult(_threadNo, !_cancelRequested);
}
