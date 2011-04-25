#ifndef EVENT_FRAME_WORK_THREAD_H_
#define EVENT_FRAME_WORK_THREAD_H_

#include <priority_queue>
#include "event.h"
#include "task.h"

/// @file   work_thread.h
/// @brief  �����̶߳���
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

/// @{
/// WorkThread�Ķ������õ��������������ͣ��������˵��:
/// 1.BaseThread: �̷߳�װ�࣬�߳��������ΪEntry����
/// 2.CondQueue: �������з�װ��,�ο�pthread_cond�������
/// @}

namespace event_frame
{
/// �����߳�
class WorkThread : public BaseThread
{
public:

    /// �����߳���ں���
    virtual int Entry()
    {
        while (!IsStopRequested())
        {
            /// ���ȴ���ʱ�¼�
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

            /// Ȼ�����µ����¼�
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

    /// ���¼����
    /// @param event  Ҫ������е��¼�
    void EnQueue(Event * event)
    {
        m_cond_queue.PushBack(event);
    }

private:

    /// �Ӷ�ͷȡһ���¼�
    /// @param  event ȡ�����¼�
    /// @param  time  ����Ϊ��ʱ������ʱ��
    /// @retval true  �Ӷ����л�ȡ�¼��ɹ�
    /// @retval false �Ӷ����л�ȡ�¼�ʧ��
    bool TimedDeQueue(Event ** event, int64_t time)
    {
        return m_cond_queue.TimedPopFront(event, time);
    }

    /// ����������¼�
    void ProcessEvent(Event * event)
    {
        int ret = event->Apply();
        if (ret > 0)
        {
            /// �ȴ���ʱ�����ԣ�������ʱ�¼�������С��
            int64_t time_now = TimeUtils::Microseconds();
            Task * task = event->GetTask();
            Event * timeout_event = task->CreateTimeoutEvent(task);
            m_wait_head.push(TimeoutItem(time_now + ret, timeout_event));
        }
        else if (ret < 0)
        {
            /// ������������������Դ
            Task * task = event->GetTask();
            delete task;
        }
        else
        {
            /// �ȴ���һ���¼�����
            /// do nothing
        }
    }

private:

    /// ��ʱ�¼���С��Ԫ��
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

        int64_t  m_timeout;   ///< ��ʱʱ��
        Event *  m_event;     ///< ��ʱ�¼�
    };
    typedef std::priority_queue<TimeoutItem, std::vector<TimeoutItem>,
        std::greator<TimeoutItem> >  TimeoutHeap;

private:

    CondQueue<Event *>  m_cond_queue;   ///< �¼�����
    TimeoutHeap         m_timeout_heap; ///< ��ʱ�¼���С��
};
} // namespace event_frame

#endif // EVENT_FRAME_WORK_THREAD_H_
