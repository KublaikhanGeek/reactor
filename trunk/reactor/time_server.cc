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

    /// ��ȡ��handler����Ӧ�ľ��
    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

    /// ������¼��Ļص�����
    virtual void HandleRead()
    {
    }

    /// ����д�¼��Ļص�����
    virtual void HandleWrite()
    {
    }

    /// ��������¼��Ļص�����
    virtual void HandleError()
    {
    }

private:

    reactor::handle_t m_handle;
};

#endif // REACTOR_TIME_SERVER_H_