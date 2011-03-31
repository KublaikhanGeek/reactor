#include <errno.h>
#include <assert.h>
#include <vector>
#include "event_demultiplexer.h"

/// @file   event_demultiplexer.cc
/// @brief  事件多路分发器类实现
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-20

namespace reactor
{
#if defined(_WIN32)
#pragma comment(lib, "Ws2_32.lib")
/// 构造函数
SelectDemultiplexer::SelectDemultiplexer()
{
    this->Clear();
}

/// 获取有事件发生的所有句柄以及所发生的事件
/// @param  events  获取的事件
/// @param  timeout 超时时间
/// @retval = 0     没有发生事件的句柄(超时)
/// @retval > 0     发生事件的句柄个数
/// @retval < 0     发生错误
int SelectDemultiplexer::WaitEvents(std::map<handle_t, event_t> * events,
                                    int timeout)
{
    /// 用select获取发生事件的fd集合
    m_timeout.tv_sec = timeout / 1000;
    m_timeout.tv_usec = timeout % 1000 * 1000;
    int max_fd = *m_fd_set.rbegin();
    int ret = select(max_fd + 1, &m_read_set, &m_write_set,
                     &m_except_set, &m_timeout);
    if (ret <= 0)
    {
        return ret;
    }
    ///转化select的结果
    std::set<handle_t>::iterator it = m_fd_set.begin();
    while (it != m_fd_set.end())
    {
        event_t evt = 0;
        if (FD_ISSET(*it, &m_except_set))
        {
            evt |= kErrorEvent;
            FD_CLR(*it, &m_read_set);
            FD_CLR(*it, &m_write_set);
        }
        else
        {
            if (FD_ISSET(*it, &m_read_set))
            {
                evt |= kReadEvent;
                FD_CLR(*it, &m_read_set);
            }
            if (FD_ISSET(*it, &m_write_set))
            {
                evt |= kWriteEvent;
                FD_CLR(*it, &m_write_set);
            }
        }
        FD_CLR(*it, &m_except_set);
        if (evt & kEventMask)
        {
            events->insert(std::make_pair(*it, evt));
        }
        ++it;
    }
    return ret;
}

/// 设置句柄handle关注evt事件
/// @retval = 0 设置成功
/// @retval < 0 设置出错
int SelectDemultiplexer::RequestEvent(handle_t handle, event_t evt)
{
    if (m_fd_set.find(handle) == m_fd_set.end())
    {
        m_fd_set.insert(handle);
    }

    if (evt & kReadEvent)
    {
        FD_SET(handle, &m_read_set);
    }
    if (evt & kWriteEvent)
    {
        FD_SET(handle, &m_write_set);
    }
    FD_SET(handle, &m_except_set);
    return 0;
}

/// 撤销句柄handle对事件的关注
/// @retval = 0 撤销成功
/// @retval < 0 撤销出错
int SelectDemultiplexer::UnrequestEvent(handle_t handle)
{
    m_fd_set.erase(handle);
    FD_CLR(handle, &m_read_set);
    FD_CLR(handle, &m_write_set);
    FD_CLR(handle, &m_except_set);
    return 0;
}

/// 清空所有的fd集合
void SelectDemultiplexer::Clear()
{
    FD_ZERO(&m_read_set);
    FD_ZERO(&m_write_set);
    FD_ZERO(&m_except_set);
    m_fd_set.clear();
}
#elif defined(__linux__)
/// 构造函数
EpollDemultiplexer::EpollDemultiplexer()
{
    m_epoll_fd = ::epoll_create(FD_SETSIZE);
    assert(m_epoll_fd != -1);
    m_fd_num = 0;
}

/// 析构函数
EpollDemultiplexer::~EpollDemultiplexer()
{
    ::close(m_epoll_fd);
}

/// 获取有事件发生的所有句柄以及所发生的事件
/// @param  events  获取的事件
/// @param  timeout 超时时间
/// @retval = 0     没有发生事件的句柄(超时)
/// @retval > 0     发生事件的句柄个数
/// @retval < 0     发生错误
int EpollDemultiplexer::WaitEvents(std::map<handle_t, event_t> * events,
                                   int timeout)
{
    std::vector<epoll_event> ep_evts(m_fd_num);
    int num = epoll_wait(m_epoll_fd, &ep_evts[0], ep_evts.size(), timeout);
    if (num > 0)
    {
        for (int idx = 0; idx < num; ++idx)
        {
            event_t evt = 0;
            if ((ep_evts[idx].events & EPOLLERR) ||
                    (ep_evts[idx].events & EPOLLHUP))
            {
                evt |= kErrorEvent;
            }
            else
            {
                if (ep_evts[idx].events & EPOLLIN)
                {
                    evt |= kReadEvent;
                }
                if (ep_evts[idx].events & EPOLLOUT)
                {
                    evt |= kWriteEvent;
                }
            }
            events->insert(std::make_pair(ep_evts[idx].data.fd, evt));
        }
    }
    return num;
}

/// 设置句柄handle关注evt事件
/// @retval = 0 设置成功
/// @retval < 0 设置出错
int EpollDemultiplexer::RequestEvent(handle_t handle, event_t evt)
{
    epoll_event ep_evt;
    ep_evt.data.fd = handle;
    ep_evt.events = 0;

    if (evt & kReadEvent)
    {
        ep_evt.events |= EPOLLIN;
    }
    if (evt & kWriteEvent)
    {
        ep_evt.events |= EPOLLOUT;
    }
    ep_evt.events |= EPOLLONESHOT;

    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, handle, &ep_evt) != 0)
    {
        if (errno == ENOENT)
        {
            if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, handle, &ep_evt) != 0)
            {
                return -errno;
            }
            ++m_fd_num;
        }
    }
    return 0;
}

/// 撤销句柄handle对事件的关注
/// @retval = 0 撤销成功
/// @retval < 0 撤销出错
int EpollDemultiplexer::UnrequestEvent(handle_t handle)
{
    epoll_event ep_evt;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, handle, &ep_evt) != 0)
    {
        return -errno;
    }
    --m_fd_num;
    return 0;
}
#else
#error "目前还不支持该平台"
#endif // _WIN32
} // namespace reactor
