#ifndef REACTOR_EVENT_DEMULTIPLEXER_H_
#define REACTOR_EVENT_DEMULTIPLEXER_H_

#include <set>
#include <map>
#include "reactor.h"

/// @file   event_demultiplexer.h
/// @brief  �¼���·���ַ�����
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-27

namespace reactor
{
class EventDemultiplexer
{
public:

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

    /// �������handle���¼�evt�Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle, event_t evt) = 0;
};

///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
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

    /// �������handle���¼�evt�Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle, event_t evt);

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
#elif defined(__linux__)
/// ��epollʵ�ֵĶ�·�¼��ַ���
class EpollDemultiplexer : public EventDemultiplexer
{
public:

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

    /// �������handle���¼�evt�Ĺ�ע
    /// @retval = 0 �����ɹ�
    /// @retval < 0 ��������
    virtual int UnrequestEvent(handle_t handle, event_t evt);
};
#else
#error "Ŀǰ��֧�ָ�ƽ̨"
#endif
} // namespace reactor

#endif // REACTOR_EVENT_DEMULTIPLEXER_H_
