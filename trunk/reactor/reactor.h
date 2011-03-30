#ifndef REACTOR_REACTOR_H_
#define REACTOR_REACTOR_H_

#ifdef _WIN32
#define FD_SETSIZE 8192
#include <Winsock2.h>
#elif defined(__linux__)
#include <stdint.h>
#include <unistd.h>
#include <sys/epoll.h>
#endif

/// @file   reactor.h
/// @brief  Reactor类,用户通过此类提供的接口使用Reactor的功能
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-27

namespace reactor
{
/// 事件类型
typedef unsigned int event_t;
enum
{
    kReadEvent    = 0x01, ///< 读事件
    kWriteEvent   = 0x02, ///< 写事件
    kErrorEvent   = 0x04, ///< 出错事件
    kEventMask    = 0xff  ///< 事件掩码
};

/// 句柄类型
#if defined(_WIN32)
    typedef ::SOCKET handle_t;
#elif defined(__linux__)
    typedef int32_t handle_t;
#else
#error "目前还不支持该平台"
#endif // _WIN32

/// 事件处理器
class EventHandler
{
public:

	/// 构造函数
	EventHandler(bool is_own_memory = false) :
		m_is_reactor_own_memory(is_own_memory)
	{}

    /// 获取该handler所对应的句柄
    virtual handle_t GetHandle() const = 0;

    /// 处理读事件的回调函数
    virtual void HandleRead() {}

    /// 处理写事件的回调函数
    virtual void HandleWrite() {}

    /// 处理出错事件的回调函数
    virtual void HandleError() {}

protected:

    /// 构造函数,只能子类调
    EventHandler() {}

    /// 析构函数,只能子类调
    virtual ~EventHandler() {}

protected:

	bool m_is_reactor_own_memory; ///< reactor是否持有handler的内存
};

/// reactor的实现类
/// TODO (zeshengwu) 实现多线程安全版本
class ReactorImplementation;

/// reactor接口类
class Reactor
{
public:

    /// 构造函数
    Reactor();

    /// 析构函数
    ~Reactor();

    /// 向reactor中注册关注事件evt的handler(可重入)
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
    /// @param  timeout 超时时间(毫秒)
    void HandleEvents(int timeout = 0);

private:

    ReactorImplementation * m_reactor_impl; ///< reactor的实现类
};
} // namespace reactor
#endif // REACTOR_REACTOR_H_
