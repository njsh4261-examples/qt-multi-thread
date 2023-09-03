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
        if (_count > _threshold && _threadNo == 2) // 문제 있는 쓰레드 가정
        {
            emit SendResult(_threadNo, false);
            return;
        }

        emit UpdateUI(_threadNo, _count++);
        msleep(100); // 유저 반응이 이벤트 루프에 끼어들 수 있는 적절한 시간 설정
    }
    emit SendResult(_threadNo, !_cancelRequested);
}
