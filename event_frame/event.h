#ifndef EVENT_FRAME_EVENT_H_
#define EVENT_FRAME_EVENT_H_

#include <assert.h>

/// @file   event.h
/// @brief  事件类型定义
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

namespace event_frame
{
/// 任务类
class Task;

/// 事件基类
class Event
{
public:

    /// 构造函数
    Event(Task * task) :
        m_task(task),
        m_dispatch_id(0)
    {}

    /// 析构函数
    virtual ~Event() {}

    /// 获取事件的分发id
    uint64_t GetDispatchID() const
    {
        return m_dispatch_id;
    }

    /// 设置事件分发id
    void SetDispatchID(uint64_t id)
    {
        m_dispatch_id = id;
    }

    /// 获取与该事件相关联的task
    Task * GetTask()
    {
        return m_task;
    }

    /// 处理事件
    virtual int Apply() = 0;

protected:

    Task *    m_task;         ///< 与此event相关联的task
    uint64_t  m_dispatch_id;  ///< 通过此id将事件分到不同的workthread
};

/// 启动指定任务的事件
template<typename T>
class StartEvent : public Event
{
public:

    /// 构造函数
    StartEvent(Task * task) :
        Event(task)
    {}

    ///  处理启动事件
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessStartEvent(this);
    }
};

/// 超时事件
template<typename T>
class TimeoutEvent: public Event
{
public:

    /// 构造函数
    TimeoutEvent(Task * task) :
        Event(task)
    {}

    /// 处理超时事件
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessTimeoutEvent(this);
    }
};

/// 任务完成事件
template<typename T>
class FinishEvent: public Event
{
public:

    /// 构造函数
    FinishEvent(Task * task) :
        Event(task)
    {}

    /// 处理超时事件
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessFinishEvent(this);
    }
};
} // namespace event_frame

#endif // EVENT_FRAME_EVENT_H_
