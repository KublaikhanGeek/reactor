#include <assert.h>
#include "reactor.h"
#include "event_demultiplexer.h"

/// @file   reactor.cc
/// @brief  Reactor类实现
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-20

namespace reactor
{
/// reactor的实现类
class ReactorImplementation
{
public:

    /// 构造函数
    ReactorImplementation();

    /// 析构函数
    ~ReactorImplementation();

    /// 向reactor中注册关注事件evt的handler
    /// @param  handler 要注册的事件处理器
    /// @param  evt     要关注的事件
    /// @retval =0      注册成功
    /// @retval <0      注册出错
    int RegisterHandler(EventHandler * handler, event_t evt);

    /// 从reactor中移除handler
    /// @param  handler 要移除的事件处理器
    /// @retval =0      移除成功
    /// @retval <0      移除出错
    int RemoveHandler(EventHandler * handler);

    /// 处理事件,回调注册的handler中相应的事件处理函数
    /// @param  timeout 超时时间
    void HandleEvents(int timeout);

private:

    EventDemultiplexer *                m_demultiplexer; ///< 事件多路分发器
    std::map<handle_t, EventHandler *>  m_handlers;      ///< 所有handler集合  
};

///////////////////////////////////////////////////////////////////////////////

/// 构造函数
Reactor::Reactor()
{
    m_reactor_impl = new ReactorImplementation();
}

/// 析构函数
Reactor::~Reactor()
{
    delete m_reactor_impl;
}

/// 向reactor中注册关注事件evt的handler
/// @param  handler 要注册的事件处理器
/// @param  evt     要关注的事件
/// @retval =0      注册成功
/// @retval <0      注册出错
int Reactor::RegisterHandler(EventHandler * handler, event_t evt)
{
    return m_reactor_impl->RegisterHandler(handler, evt);
}

/// 从reactor中移除handler
/// @param  handler 要移除的事件处理器
/// @retval =0      移除成功
/// @retval <0      移除出错
int Reactor::RemoveHandler(EventHandler * handler)
{
    return m_reactor_impl->RemoveHandler(handler);
}

/// 处理事件,回调注册的handler中相应的事件处理函数
/// @param  timeout 超时时间
void Reactor::HandleEvents(int timeout)
{
    m_reactor_impl->HandleEvents(timeout);
}

///////////////////////////////////////////////////////////////////////////////

/// 构造函数
ReactorImplementation::ReactorImplementation()
{
#if defined(_WIN32)
    m_demultiplexer = new SelectDemultiplexer();
#elif defined(__linux__)
    m_demultiplexer = new EpollDemultiplexer();
#else
#error "目前还不支持该平台"
#endif // _WIN32
}

/// 析构函数
ReactorImplementation::~ReactorImplementation()
{
    delete m_demultiplexer;
}

/// 向reactor中注册关注事件evt的handler
/// @param  handler 要注册的事件处理器
/// @param  evt     要关注的事件
/// @retval =0      注册成功
/// @retval <0      注册出错
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

/// 从reactor中移除handler
/// @param  handler 要移除的事件处理器
/// @retval =0      移除成功
/// @retval <0      移除出错
int ReactorImplementation::RemoveHandler(EventHandler * handler)
{
    handle_t handle = handler->GetHandle();
    m_handlers.erase(handle);
    return m_demultiplexer->UnrequestEvent(handle, kEventMask);
}

/// 处理事件,回调注册的handler中相应的事件处理函数
/// @param  timeout 超时时间
void ReactorImplementation::HandleEvents(int timeout)
{
    std::map<handle_t, event_t> events;
    /// 获取事件
    int events_num = m_demultiplexer->WaitEvents(&events);
    if (events_num > 0)
    {
        /// 处理事件
        std::map<handle_t, event_t>::iterator it = events.begin();
        while (it != events.end())
        {
            assert(m_handlers.find(it->first) != m_handlers.end());
            EventHandler * handler = m_handlers[it->first];
            if (it->second & kErrorEvent) ///< 出错
            {
                handler->HandleError();
            }
            else ///< 可读 || 可写
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
