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
            }
            if (FD_ISSET(*it, &m_write_set))
            {
                evt |= kWriteEvent;
            }
        }
        if (evt & kEventMask)
        {
            events->insert(std::make_pair(*it, evt));
        }
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
/// ��ȡ���¼����������о���Լ����������¼�
/// @param  events  ��ȡ���¼�
/// @param  timeout ��ʱʱ��
/// @retval = 0     û�з����¼��ľ��(��ʱ)
/// @retval > 0     �����¼��ľ������
/// @retval < 0     ��������
int EpollDemultiplexer::WaitEvents(std::map<handle_t, event_t> * events,
                                   int timeout)
{
    return 0;
}

/// ���þ��handle��עevt�¼�
/// @retval = 0 ���óɹ�
/// @retval < 0 ���ó���
int EpollDemultiplexer::RequestEvent(handle_t handle, event_t evt)
{
    return 0;
}

/// �������handle���¼�evt�Ĺ�ע
/// @retval = 0 �����ɹ�
/// @retval < 0 ��������
int EpollDemultiplexer::UnrequestEvent(handle_t handle, event_t evt)
{
    return 0;
}
#else
#error "Ŀǰ����֧�ָ�ƽ̨"
#endif // _WIN32
} // namespace reactor