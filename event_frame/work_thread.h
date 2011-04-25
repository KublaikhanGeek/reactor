#ifndef EVENT_FRAME_WORK_THREAD_H_
#define EVENT_FRAME_WORK_THREAD_H_

#include <priority_queue>
#include "event.h"
#include "task.h"

/// @file   work_thread.h
/// @brief  工作线程定义
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

/// @{
/// WorkThread的定义中用到了下面两个类型，这里加以说明:
/// 1.BaseThread: 线程封装类，线程启动入口为Entry函数
/// 2.CondQueue: 条件队列封装类,参考pthread_cond相关内容
/// @}

namespace event_frame
{
/// 工作线程
class WorkThread : public BaseThread
{
public:

    /// 工作线程入口函数
    virtual int Entry()
    {
        while (!IsStopRequested())
        {
            /// 优先处理超时事件
            if (!m_timeout_heap.empty())
            {
                int64_t time_now = TimeUtils::Microseconds();
                const TimeoutItem & top_item = m_timeout_heap.top();
                if (top_item.m_timeout <= time_now)
                {
                    ProcessEvent(top_item.m_event);
                    delete top_item.m_event;
                    m_wait_head.pop();
                    continue;
                }
            }

            /// 然后处理新到的事件
            Event * event = NULL;
            if (TimedDeQueue(&event, 5))
            {
                assert(event != NULL);
                ProcessEvent(event);
                delete event;
                event = NULL;
            }
        }
    }

    /// 新事件入队
    /// @param event  要插入队列的事件
    void EnQueue(Event * event)
    {
        m_cond_queue.PushBack(event);
    }

private:

    /// 从队头取一个事件
    /// @param  event 取到的事件
    /// @param  time  队列为空时的阻塞时间
    /// @retval true  从队列中获取事件成功
    /// @retval false 从队列中获取事件失败
    bool TimedDeQueue(Event ** event, int64_t time)
    {
        return m_cond_queue.TimedPopFront(event, time);
    }

    /// 处理给定的事件
    void ProcessEvent(Event * event)
    {
        int ret = event->Apply();
        if (ret > 0)
        {
            /// 等待超时后重试，创建超时事件加入最小堆
            int64_t time_now = TimeUtils::Microseconds();
            Task * task = event->GetTask();
            Event * timeout_event = task->CreateTimeoutEvent(task);
            m_wait_head.push(TimeoutItem(time_now + ret, timeout_event));
        }
        else if (ret < 0)
        {
            /// 任务结束，销毁相关资源
            Task * task = event->GetTask();
            delete task;
        }
        else
        {
            /// 等待下一次事件触发
            /// do nothing
        }
    }

private:

    /// 超时事件最小堆元素
    struct TimeoutItem
    {
        TimeoutItem(int64_t timeout, Event * event) :
            m_timeout(timeout),
            m_event(event)
        {}

        bool operator>(const TimeoutItem & item)
        {
            return m_timeout > item.m_timeout;
        }

        int64_t  m_timeout;   ///< 超时时间
        Event *  m_event;     ///< 超时事件
    };
    typedef std::priority_queue<TimeoutItem, std::vector<TimeoutItem>,
        std::greator<TimeoutItem> >  TimeoutHeap;

private:

    CondQueue<Event *>  m_cond_queue;   ///< 事件队列
    TimeoutHeap         m_timeout_heap; ///< 超时事件最小堆
};
} // namespace event_frame

#endif // EVENT_FRAME_WORK_THREAD_H_
