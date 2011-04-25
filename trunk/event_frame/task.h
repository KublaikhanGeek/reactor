#ifndef EVENT_FRAME_TASK_H_
#define EVENT_FRAME_TASK_H_

/// @file   task.h
/// @brief  任务类型定义
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-04-25

namespace event_frame
{
/// 事件类
class Event;

/// 任务基类
class Task
{
public:

    /// 构造函数
    Task() {}

    /// 析构函数
    virtual ~Task() {}

    /// 处理启动事件
    /// @param  event 启动任务事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    virtual int ProcessStartEvent(Event * event) = 0;

    /// 处理超时事件
    /// @param  event 超时事件
    /// @retval =0    等待下次事件触发 
    /// @retval >0    等待超时触发
    /// @retval <0    任务结束
    virtual int ProcessTimeoutEvent(Event * event) = 0;

    /// 创建一个超时事件
    /// @return 创建的超时事件的指针
    virtual Event * CreateTimeoutEvent() = 0;
};
} // namespace event_frame

#endif // EVENT_FRAME_TASK_H_
