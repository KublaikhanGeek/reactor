#ifndef EVENT_FRAME_EVENT_TASK_H_
#define EVENT_FRAME_EVENT_TASK_H_

#include<assert.h>

/// @file    event_task.h
/// @brief   �¼�����������Ͷ���
/// @author  zeshengwu<wuzesheng@vip.qq.com>
/// @date    2011-05-16

namespace event_frame
{
class Task;

template<typename T> struct EventTraits;

class Event
{
public:

    /// ���캯��
    Event(uint64_t disp_id, Task * task) :
        m_dispatch_id(disp_id),
        m_task(task),
        m_cookie(NULL)
    {}

    /// ��������
    virtual ~Event() {}

    /// ����dispatch id
    void SetDispatchID(uint64_t id)
    {
        m_dispatch_id = id;
    }

    /// ��ȡdispatch id
    uint64_t GetDispatchID() const
    {
        return m_dispatch_id;
    }

    /// ����cookie
    void SetCookie(void * cookie)
    {
        m_cookie = cookie;
    }

    /// ��ȡcookie
    const void * GetCookie() const
    {
        return m_cookie;
    }

    /// �����̴߳����¼�ʱ���õĽӿ�
    virtual void Apply() = 0;

protected:

    uint64_t m_dispatch_id; ///< �����ַ��¼���id
    Task *   m_task;        ///< ����¼��������task
    void *   m_cookie;      ///< �¼�������cookie
};

#define EVENT_FRAME_DEFINE_EVENT(event, task, callback) \
class event; \
 \
template<> \
struct EventTraits<event> \
{ \
    typedef task BaseTask; \
}; \
 \
class event : public Event \
{ \
public: \
     \
    typedef EventTraits<event>::BaseTask TaskType; \
     \
    event(Task * t) : \
        Event(t) \
    {} \
     \
    virtual void Apply() \
    { \
        TaskType * t = dynamic_cast<TaskType *>(m_task); \
        assert(t != NULL); \
        t->callback(this); \
    } \
};

///////////////////////////////////////////////////////////////////////////////

EVENT_FRAME_DEFINE_EVENT(StartEvent, Task, OnStart)
EVENT_FRAME_DEFINE_EVENT(TimeoutEvent, Task, OnTimeout)
EVENT_FRAME_DEFINE_EVENT(FinishEvent, Task, OnFinish)

class Task
{
public:

    /// ���캯��
    Task(id = 0) :
        m_task_id(id)
    {}

    /// ��������
    virtual ~Task() {};

    /// ����task id
    void SetTaskID(uint32_t id)
    {
        m_task_id = id;
    }

    /// ��ȡtask id
    uint32_t GetTaskID() const
    {
        return m_task_id;
    }

    /// ��������������¼�
    /// @param  event ���������¼�
    /// @retval >0 ָ��ʱ�������
    /// @retval =0 ����/�ȴ���һ�¼�����
    /// @retval <0 ��������ֵΪ������
    virtual int OnStart(StartEvent * event) = 0;

    /// ��������ʱ���¼�
    /// @param  event ��ʱ�¼�
    /// @retval >0 ָ��ʱ�������
    /// @retval =0 ����/�ȴ���һ�¼�����
    /// @retval <0 ��������ֵΪ������
    virtual int OnTimeout(TimeoutEvent * event) = 0;

    /// �������������¼�
    /// @param  event ���������¼�
    /// @retval >0 ָ��ʱ�������
    /// @retval =0 ����/�ȴ���һ�¼�����
    /// @retval <0 ��������ֵΪ������
    virtual int OnFinish(FinishEvent * event) = 0;

protected:

    uint32_t  m_task_id; ///< task id
};
}
#endif // EVENT_FRAME_EVENT_TASK_H_

