#ifndef REACTOR_TIME_SERVER_H_
#define REACTOR_TIME_SERVER_H_

#include "reactor.h"

/// @file   time_server.cc
/// @brief  
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-28

class TimeServer : public reactor::EventHandler
{
public:

    /// 获取该handler所对应的句柄
    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

    /// 处理读事件的回调函数
    virtual void HandleRead()
    {
    }

    /// 处理写事件的回调函数
    virtual void HandleWrite()
    {
    }

    /// 处理出错事件的回调函数
    virtual void HandleError()
    {
    }

private:

    reactor::handle_t m_handle;
};

#endif // REACTOR_TIME_SERVER_H_