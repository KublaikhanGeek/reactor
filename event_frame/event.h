#ifndef EVENT_FRAME_EVENT_H_
#define EVENT_FRAME_EVENT_H_

#include <assert.h>

/// @file   event.h
/// @brief  �¼����Ͷ���
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

namespace event_frame
{
/// ������
class Task;

/// �¼�����
class Event
{
public:

    /// ���캯��
    Event(Task * task) :
        m_task(task),
        m_dispatch_id(0)
    {}

    /// ��������
    virtual ~Event() {}

    /// ��ȡ�¼��ķַ�id
    uint64_t GetDispatchID() const
    {
        return m_dispatch_id;
    }

    /// �����¼��ַ�id
    void SetDispatchID(uint64_t id)
    {
        m_dispatch_id = id;
    }

    /// ��ȡ����¼��������task
    Task * GetTask()
    {
        return m_task;
    }

    /// �����¼�
    virtual int Apply() = 0;

protected:

    Task *    m_task;         ///< ���event�������task
    uint64_t  m_dispatch_id;  ///< ͨ����id���¼��ֵ���ͬ��workthread
};

/// ����ָ��������¼�
template<typename T>
class StartEvent : public Event
{
public:

    /// ���캯��
    StartEvent(Task * task) :
        Event(task)
    {}

    ///  ���������¼�
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessStartEvent(this);
    }
};

/// ��ʱ�¼�
template<typename T>
class TimeoutEvent: public Event
{
public:

    /// ���캯��
    TimeoutEvent(Task * task) :
        Event(task)
    {}

    /// ����ʱ�¼�
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessTimeoutEvent(this);
    }
};

/// ��������¼�
template<typename T>
class FinishEvent: public Event
{
public:

    /// ���캯��
    FinishEvent(Task * task) :
        Event(task)
    {}

    /// ����ʱ�¼�
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessFinishEvent(this);
    }
};
} // namespace event_frame

#endif // EVENT_FRAME_EVENT_H_
