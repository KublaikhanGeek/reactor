#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "task.h"

/// 用户自定义A事件
template<typename T>
class UserDefinedAEvent: public event_frame::Event
{
public:

    /// 构造函数
    UserDefinedAEvent(event_frame::Task * task) :
        event_frame::Event(task)
    {}

    /// 处理用记自定义A事件
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessUserDefinedAEvent(this);
    };
};

/// 用户自定义B事件
template<typename T>
class UserDefinedBEvent: public event_frame::Event
{
public:

    /// 构造函数
    UserDefinedBEvent(event_frame::Task * task) :
        event_frame::Event(task)
    {}

    /// 处理用记自定义B事件
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessUserDefinedBEvent(this);
    };
};

/// 用户自定义的任务
class UserDefinedTask : public event_frame::Task
{
public:

    /// 处理启动事件
    /// @param  event 启动任务事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    virtual int ProcessStartEvent(event_frame::Event * event)
    {
        event_frame::StartEvent<UserDefinedTask> * evt =
            dynamic_cast<event_frame::StartEvent<UserDefinedTask> *>(event);
        assert(evt != NULL);
        /// TODO 处理启动事件
        return 0;
    }

    /// 处理超时事件
    /// @param  event 超时事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    virtual int ProcessTimeoutEvent(event_frame::Event * event)
    {
        event_frame::TimeoutEvent<UserDefinedTask> * evt =
            dynamic_cast<event_frame::TimeoutEvent<UserDefinedTask> *>(event);
        assert(evt != NULL);
        /// TODO 处理超时事件
        return 0;
    }

    /// 创建一个超时事件
    /// @return 创建的超时事件的指针
    virtual event_frame::Event * CreateTimeoutEvent()
    {
        return new event_frame::TimeoutEvent<UserDefinedTask>(this);
    }
    
    /// 处理用户自定义A事件
    /// @param  event 要处理的事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    int ProcessUserDefinedAEvent(UserDefinedAEvent<UserDefinedTask> * event)
    {
        /// TODO 处理用户自定义事件
        fprintf(stderr, "%s\n", "ProcessUserDefinedAEvent");
        return 0;
    }

    /// 处理用户自定义B事件
    /// @param  event 要处理的事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    int ProcessUserDefinedBEvent(UserDefinedBEvent<UserDefinedTask> * event)
    {
        /// TODO 处理用户自定义事件
        fprintf(stderr, "%s\n", "ProcessUserDefinedBEvent");
        return 0;
    }
};

int main()
{
    event_frame::Task * task = new UserDefinedTask();
    event_frame::Event * evt_a = new UserDefinedAEvent<UserDefinedTask>(task);
    event_frame::Event * evt_b = new UserDefinedBEvent<UserDefinedTask>(task);
    evt_a->Apply();
    evt_b->Apply();
    delete evt_b;
    delete evt_a;
    delete task;
}
