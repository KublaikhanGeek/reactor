#ifndef EVENT_FRAME_EVENT_TASK_H_
#define EVENT_FRAME_EVENT_TASK_H_

#include<assert.h>

/// @file    event_task.h
/// @brief   事件和任务基类型定义
/// @author  zeshengwu<wuzesheng@vip.qq.com>
/// @date    2011-05-16

namespace event_frame
{
class Task;

template<typename T> struct EventTraits;

class Event
{
public:

    /// 构造函数
    Event(uint64_t disp_id, Task * task) :
        m_dispatch_id(disp_id),
        m_task(task),
        m_cookie(NULL)
    {}

    /// 析构函数
    virtual ~Event() {}

    /// 设置dispatch id
    void SetDispatchID(uint64_t id)
    {
        m_dispatch_id = id;
    }

    /// 获取dispatch id
    uint64_t GetDispatchID() const
    {
        return m_dispatch_id;
    }

    /// 设置cookie
    void SetCookie(void * cookie)
    {
        m_cookie = cookie;
    }

    /// 获取cookie
    const void * GetCookie() const
    {
        return m_cookie;
    }

    /// 工作线程处理事件时调用的接口
    virtual void Apply() = 0;

protected:

    uint64_t m_dispatch_id; ///< 用来分发事件的id
    Task *   m_task;        ///< 与该事件相关联的task
    void *   m_cookie;      ///< 事件所带的cookie
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

    /// 构造函数
    Task(id = 0) :
        m_task_id(id)
    {}

    /// 析构函数
    virtual ~Task() {};

    /// 设置task id
    void SetTaskID(uint32_t id)
    {
        m_task_id = id;
    }

    /// 获取task id
    uint32_t GetTaskID() const
    {
        return m_task_id;
    }

    /// 处理启动任务的事件
    /// @param  event 启动任务事件
    /// @retval >0 指定时间后重试
    /// @retval =0 结束/等待下一事件触发
    /// @retval <0 出错，返回值为出错码
    virtual int OnStart(StartEvent * event) = 0;

    /// 处理任务超时的事件
    /// @param  event 超时事件
    /// @retval >0 指定时间后重试
    /// @retval =0 结束/等待下一事件触发
    /// @retval <0 出错，返回值为出错码
    virtual int OnTimeout(TimeoutEvent * event) = 0;

    /// 处理结束任务的事件
    /// @param  event 结束任务事件
    /// @retval >0 指定时间后重试
    /// @retval =0 结束/等待下一事件触发
    /// @retval <0 出错，返回值为出错码
    virtual int OnFinish(FinishEvent * event) = 0;

protected:

    uint32_t  m_task_id; ///< task id
};
}
#endif // EVENT_FRAME_EVENT_TASK_H_

