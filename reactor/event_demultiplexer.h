#ifndef REACTOR_EVENT_DEMULTIPLEXER_H_
#define REACTOR_EVENT_DEMULTIPLEXER_H_

#include <set>
#include <map>
#include "reactor.h"

/// @file   event_demultiplexer.h
/// @brief  �¼���·���ַ�����
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-20

namespace reactor
{
class EventDemultiplexer
{
public:

    /// ��������
    virtual ~EventDemultiplexer() {}

    /// ��ȡ���¼����������о���Լ����������¼�
    /// @param  events  ��ȡ���¼�
    /// @param  timeout ��ʱʱ��
    /// @retval = 0     û�з����¼��ľ��(��ʱ)
    /// @retval > 0     �����¼��ľ������
    /// @retval < 0     ��������
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0) = 0;

    /// ���þ��handle��עevt�¼�
    /// @retval = 0 ���óɹ�
    /// @retval < 0 ���ó���
    virtual int RequestEvent(handle_t handle, event_t evt) = 0;

    /// �������handle���¼��Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/// ��selectʵ�ֵĶ�·�¼��ַ���
class SelectDemultiplexer : public EventDemultiplexer
{
public:

    /// ���캯��
    SelectDemultiplexer();

    /// ��ȡ���¼����������о���Լ����������¼�
    /// @param  events  ��ȡ���¼�
    /// @param  timeout ��ʱʱ��
    /// @retval = 0     û�з����¼��ľ��(��ʱ)
    /// @retval > 0     �����¼��ľ������
    /// @retval < 0     ��������
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0);

    /// ���þ��handle��עevt�¼�
    /// @retval = 0 ���óɹ�
    /// @retval < 0 ���ó���
    virtual int RequestEvent(handle_t handle, event_t evt);

    /// �������handle���¼��Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle);

private:

    /// ������е�fd����
    void Clear();

private:

    fd_set              m_read_set;   ///< ���¼�fd����
    fd_set              m_write_set;  ///< д�¼�fd����
    fd_set              m_except_set; ///< �쳣�¼�fd����
    timeval             m_timeout;    ///< ��ʱ
    std::set<handle_t>  m_fd_set;     ///< ���е�fd����
};

///////////////////////////////////////////////////////////////////////////////

/// ��epollʵ�ֵĶ�·�¼��ַ���
class EpollDemultiplexer : public EventDemultiplexer
{
public:

    /// ���캯��
    EpollDemultiplexer();

    /// ��������
    ~EpollDemultiplexer();

    /// ��ȡ���¼����������о���Լ����������¼�
    /// @param  events  ��ȡ���¼�
    /// @param  timeout ��ʱʱ��
    /// @retval = 0     û�з����¼��ľ��(��ʱ)
    /// @retval > 0     �����¼��ľ������
    /// @retval < 0     ��������
    virtual int WaitEvents(std::map<handle_t, event_t> * events,
                           int timeout = 0);

    /// ���þ��handle��עevt�¼�
    /// @retval = 0 ���óɹ�
    /// @retval < 0 ���ó���
    virtual int RequestEvent(handle_t handle, event_t evt);

    /// �������handle���¼��Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle);

private:

    int  m_epoll_fd; ///< epoll�����fd
    int  m_fd_num;   ///< ����fd�ĸ���
};
} // namespace reactor

#endif // REACTOR_EVENT_DEMULTIPLEXER_H_
