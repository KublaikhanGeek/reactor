#ifndef REACTOR_EVENT_DEMULTIPLEXER_H_
#define REACTOR_EVENT_DEMULTIPLEXER_H_

#include <set>
#include <map>
#include "reactor.h"

/// @file   event_demultiplexer.h
/// @brief  事件多路发分发器类
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-20

namespace reactor
{
class EventDemultiplexer
{
public:

    /// 析构函数
    virtual ~EventDemultiplexer() {}

    /// 获取有事件发生的所有句柄以及所发生的事件
    /// @param  events  获取的事件
    /// @param  timeout 超时时间
    /// @retval = 0     没有发生事件的句柄(超时)
    /// @retval > 0     发生事件的句柄个数
    /// @retval < 0     发生错误
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0) = 0;

    /// 设置句柄handle关注evt事件
    /// @retval = 0 设置成功
    /// @retval < 0 设置出错
    virtual int RequestEvent(handle_t handle, event_t evt) = 0;

    /// 撤销句柄handle对事件的关注
    /// @retval = 0 撤销成功
    /// @retval < 0 撤销出错
    virtual int UnrequestEvent(handle_t handle) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/// 由select实现的多路事件分发器
class SelectDemultiplexer : public EventDemultiplexer
{
public:

    /// 构造函数
    SelectDemultiplexer();

    /// 获取有事件发生的所有句柄以及所发生的事件
    /// @param  events  获取的事件
    /// @param  timeout 超时时间
    /// @retval = 0     没有发生事件的句柄(超时)
    /// @retval > 0     发生事件的句柄个数
    /// @retval < 0     发生错误
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0);

    /// 设置句柄handle关注evt事件
    /// @retval = 0 设置成功
    /// @retval < 0 设置出错
    virtual int RequestEvent(handle_t handle, event_t evt);

    /// 撤销句柄handle对事件的关注
    /// @retval = 0 撤销成功
    /// @retval < 0 撤销出错
    virtual int UnrequestEvent(handle_t handle);

private:

    /// 清空所有的fd集合
    void Clear();

private:

    fd_set              m_read_set;   ///< 读事件fd集合
    fd_set              m_write_set;  ///< 写事件fd集合
    fd_set              m_except_set; ///< 异常事件fd集合
    timeval             m_timeout;    ///< 超时
    std::set<handle_t>  m_fd_set;     ///< 所有的fd集合
};

///////////////////////////////////////////////////////////////////////////////

/// 由epoll实现的多路事件分发器
class EpollDemultiplexer : public EventDemultiplexer
{
public:

    /// 构造函数
    EpollDemultiplexer();

    /// 析构函数
    ~EpollDemultiplexer();

    /// 获取有事件发生的所有句柄以及所发生的事件
    /// @param  events  获取的事件
    /// @param  timeout 超时时间
    /// @retval = 0     没有发生事件的句柄(超时)
    /// @retval > 0     发生事件的句柄个数
    /// @retval < 0     发生错误
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0);

    /// 设置句柄handle关注evt事件
    /// @retval = 0 设置成功
    /// @retval < 0 设置出错
    virtual int RequestEvent(handle_t handle, event_t evt);

    /// 撤销句柄handle对事件的关注
    /// @retval = 0 撤销成功
    /// @retval < 0 撤销出错
    virtual int UnrequestEvent(handle_t handle);

private:

    int  m_epoll_fd; ///< epoll自身的fd
    int  m_fd_num;   ///< 所有fd的个数
};
} // namespace reactor

#endif // REACTOR_EVENT_DEMULTIPLEXER_H_
