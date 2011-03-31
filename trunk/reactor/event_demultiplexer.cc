#include <errno.h>
#include <assert.h>
#include <vector>
#include "event_demultiplexer.h"

namespace reactor
{
#if defined(_WIN32)
#pragma comment(lib, "Ws2_32.lib")
/// ���캯��
SelectDemultiplexer::SelectDemultiplexer()
{
    this->Clear();
}

/// ��ȡ���¼����������о���Լ����������¼�
/// @param  events  ��ȡ���¼�
/// @param  timeout ��ʱʱ��
/// @retval = 0     û�з����¼��ľ��(��ʱ)
/// @retval > 0     �����¼��ľ������
/// @retval < 0     ��������
int SelectDemultiplexer::WaitEvents(std::map<handle_t, event_t> * events,
                                    int timeout)
{
    /// ��select��ȡ�����¼���fd����
    m_timeout.tv_sec = timeout / 1000;
    m_timeout.tv_usec = timeout % 1000 * 1000;
    int ret = select(FD_SETSIZE, &m_read_set, &m_write_set,
                     &m_except_set, &m_timeout);
    if (ret <= 0)
    {
        return ret;
    }
    ///ת��select�Ľ��
    std::set<handle_t>::iterator it = m_fd_set.begin();
    while (it != m_fd_set.end())
    {
        event_t evt = 0;
        if (FD_ISSET(*it, &m_except_set))
        {
            evt |= kErrorEvent;
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
        if (evt & kEventMask)
        {
            events->insert(std::make_pair(*it, evt));
        }
        ++it;
    }
    return ret;
}

/// ���þ��handle��עevt�¼�
/// @retval = 0 ���óɹ�
/// @retval < 0 ���ó���
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

/// �������handle���¼�evt�Ĺ�ע
/// @retval = 0 �����ɹ�
/// @retval < 0 ��������
int SelectDemultiplexer::UnrequestEvent(handle_t handle, event_t evt)
{
    if ((evt ^ kEventMask) == 0)
    {
        m_fd_set.erase(handle);
    }

    if (evt & kReadEvent)
    {
        FD_CLR(handle, &m_read_set);
    }
    if (evt & kWriteEvent)
    {
        FD_CLR(handle, &m_write_set);
    }
    if (evt & kErrorEvent)
    {
        FD_CLR(handle, &m_except_set);
    }
    return 0;
}

/// ������е�fd����
void SelectDemultiplexer::Clear()
{
    FD_ZERO(&m_read_set);
    FD_ZERO(&m_write_set);
    FD_ZERO(&m_except_set);
    m_fd_set.clear();
}
#elif defined(__linux__)
/// ���캯��
EpollDemultiplexer::EpollDemultiplexer()
{
    m_epoll_fd = ::epoll_create(FD_SETSIZE);
    assert(m_epoll_fd != -1);
    m_fd_num = 0;
}

/// ��������
EpollDemultiplexer::~EpollDemultiplexer()
{
    ::close(m_epoll_fd);
}

/// ��ȡ���¼����������о���Լ����������¼�
/// @param  events  ��ȡ���¼�
/// @param  timeout ��ʱʱ��
/// @retval = 0     û�з����¼��ľ��(��ʱ)
/// @retval > 0     �����¼��ľ������
/// @retval < 0     ��������
int EpollDemultiplexer::WaitEvents(std::map<handle_t, event_t> * events,
                                   int timeout)
{
    std::vector<epoll_event> ep_evts(m_fd_num);
    int num = epoll_wait(m_epoll_fd, &ep_evts[0], ep_evts.size(), timeout);
    if (num > 0)
    {
        for (int idx = 0; idx < num; ++idx)
        {
            event_t evt;
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

/// ���þ��handle��עevt�¼�
/// @retval = 0 ���óɹ�
/// @retval < 0 ���ó���
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

/// �������handle���¼�evt�Ĺ�ע
/// @retval = 0 �����ɹ�
/// @retval < 0 ��������
int EpollDemultiplexer::UnrequestEvent(handle_t handle, event_t evt)
{
    if ((evt ^ kEventMask) == 0)
    {
        epoll_event ep_evt;
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, handle, &ep_evt) != 0)
        {
            return -errno;
        }
        --m_fd_num;
        return 0;
    }
    assert(0);
    return -0xffff; /// fatal error
}
#else
#error "Ŀǰ����֧�ָ�ƽ̨"
#endif // _WIN32
} // namespace reactor
