#ifndef EVENT_FRAME_TASK_H_
#define EVENT_FRAME_TASK_H_

/// @file   task.h
/// @brief  �������Ͷ���
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

namespace event_frame
{
/// �¼���
class Event;

/// �������
class Task
{
public:

    /// ���캯��
    Task() {}

    /// ��������
    virtual ~Task() {}

    /// ���������¼�
    /// @param  event ���������¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    virtual int ProcessStartEvent(Event * event) = 0;

    /// ����ʱ�¼�
    /// @param  event ��ʱ�¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    virtual int ProcessTimeoutEvent(Event * event) = 0;

    /// ����һ����ʱ�¼�
    /// @return �����ĳ�ʱ�¼���ָ��
    virtual Event * CreateTimeoutEvent() = 0;
};
} // namespace event_frame

#endif // EVENT_FRAME_TASK_H_
