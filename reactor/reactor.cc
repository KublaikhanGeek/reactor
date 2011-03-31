#include <assert.h>
#include "reactor.h"
#include "event_demultiplexer.h"

/// @file   reactor.cc
/// @brief  Reactor��ʵ��
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-20

namespace reactor
{
/// reactor��ʵ����
class ReactorImplementation
{
public:

    /// ���캯��
    ReactorImplementation();

    /// ��������
    ~ReactorImplementation();

    /// ��reactor��ע���ע�¼�evt��handler
    /// @param  handler Ҫע����¼�������
    /// @param  evt     Ҫ��ע���¼�
    /// @retval =0      ע��ɹ�
    /// @retval <0      ע�����
    int RegisterHandler(EventHandler * handler, event_t evt);

    /// ��reactor���Ƴ�handler
    /// @param  handler Ҫ�Ƴ����¼�������
    /// @retval =0      �Ƴ��ɹ�
    /// @retval <0      �Ƴ�����
    int RemoveHandler(EventHandler * handler);

    /// �����¼�,�ص�ע���handler����Ӧ���¼�������
    /// @param  timeout ��ʱʱ��
    void HandleEvents(int timeout);

private:

    EventDemultiplexer *                m_demultiplexer; ///< �¼���·�ַ���
    std::map<handle_t, EventHandler *>  m_handlers;      ///< ����handler����  
};

///////////////////////////////////////////////////////////////////////////////

/// ���캯��
Reactor::Reactor()
{
    m_reactor_impl = new ReactorImplementation();
}

/// ��������
Reactor::~Reactor()
{
    delete m_reactor_impl;
}

/// ��reactor��ע���ע�¼�evt��handler
/// @param  handler Ҫע����¼�������
/// @param  evt     Ҫ��ע���¼�
/// @retval =0      ע��ɹ�
/// @retval <0      ע�����
int Reactor::RegisterHandler(EventHandler * handler, event_t evt)
{
    return m_reactor_impl->RegisterHandler(handler, evt);
}

/// ��reactor���Ƴ�handler
/// @param  handler Ҫ�Ƴ����¼�������
/// @retval =0      �Ƴ��ɹ�
/// @retval <0      �Ƴ�����
int Reactor::RemoveHandler(EventHandler * handler)
{
    return m_reactor_impl->RemoveHandler(handler);
}

/// �����¼�,�ص�ע���handler����Ӧ���¼�������
/// @param  timeout ��ʱʱ��
void Reactor::HandleEvents(int timeout)
{
    m_reactor_impl->HandleEvents(timeout);
}

///////////////////////////////////////////////////////////////////////////////

/// ���캯��
ReactorImplementation::ReactorImplementation()
{
#if defined(_WIN32)
    m_demultiplexer = new SelectDemultiplexer();
#elif defined(__linux__)
    m_demultiplexer = new EpollDemultiplexer();
#else
#error "Ŀǰ����֧�ָ�ƽ̨"
#endif // _WIN32
}

/// ��������
ReactorImplementation::~ReactorImplementation()
{
    delete m_demultiplexer;
}

/// ��reactor��ע���ע�¼�evt��handler
/// @param  handler Ҫע����¼�������
/// @param  evt     Ҫ��ע���¼�
/// @retval =0      ע��ɹ�
/// @retval <0      ע�����
int ReactorImplementation::RegisterHandler(EventHandler * handler, event_t evt)
{
    handle_t handle = handler->GetHandle();
    std::map<handle_t, EventHandler *>::iterator it = m_handlers.find(handle);
    if (it == m_handlers.end())
    {
        m_handlers[handle] = handler;
    }
    return m_demultiplexer->RequestEvent(handle, evt);
}

/// ��reactor���Ƴ�handler
/// @param  handler Ҫ�Ƴ����¼�������
/// @retval =0      �Ƴ��ɹ�
/// @retval <0      �Ƴ�����
int ReactorImplementation::RemoveHandler(EventHandler * handler)
{
    handle_t handle = handler->GetHandle();
    m_handlers.erase(handle);
    return m_demultiplexer->UnrequestEvent(handle, kEventMask);
}

/// �����¼�,�ص�ע���handler����Ӧ���¼�������
/// @param  timeout ��ʱʱ��
void ReactorImplementation::HandleEvents(int timeout)
{
    std::map<handle_t, event_t> events;
    /// ��ȡ�¼�
    int events_num = m_demultiplexer->WaitEvents(&events);
    if (events_num > 0)
    {
        /// �����¼�
        std::map<handle_t, event_t>::iterator it = events.begin();
        while (it != events.end())
        {
            assert(m_handlers.find(it->first) != m_handlers.end());
            EventHandler * handler = m_handlers[it->first];
            if (it->second & kErrorEvent) ///< ����
            {
                handler->HandleError();
            }
            else ///< �ɶ� || ��д
            {
                if (it->second & kReadEvent)
                {
                    handler->HandleRead();
                }
                if (it->second & kWriteEvent)
                {
                    handler->HandleWrite();
                }
            }
            ++it;
        }
    }
}
} // namespace reactor
