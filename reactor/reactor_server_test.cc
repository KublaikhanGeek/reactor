#ifndef REACTOR_TIME_SERVER_H_
#define REACTOR_TIME_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "reactor.h"

/// @file   time_server.cc
/// @brief  用reactor实现的时间server,内部用telnet协议
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-28

/// reactor对象
reactor::Reactor g_reactor;

/// 读/写数据用到的buffer
const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];

class RequestHandler : public reactor::EventHandler
{
public:

	/// 构造函数
	RequestHandler(reactor::handle_t handle) :
		EventHandler(true),
		m_handle(handle)
	{}

    /// 获取该handler所对应的句柄
    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

	/// 向客户端回应答
	virtual void HandleWrite()
	{
		::memset(g_write_buffer, 0, sizeof(g_write_buffer));
		int len = sprintf(g_write_buffer, "current time: %d\n", (int)time(NULL));
		len = ::send(m_handle, g_write_buffer, len, 0);
		if (len > 0)
		{
			fprintf(stderr, "send response to client, fd=%d\n", (int)m_handle);
			::close(m_handle);
			g_reactor.RemoveHandler(this);
		}
		else
		{
			assert(0);
		}
	}

	/// 读客户端发过来的数据
	virtual void HandleRead()
	{
		::memset(g_read_buffer, 0, sizeof(g_read_buffer));
		int len = ::recv(m_handle, g_read_buffer, kBufferSize, 0);
		if (len > 0)
		{
			if (::strncasecmp("gettime", g_read_buffer, 4) == 0)
			{
				g_reactor.RegisterHandler(this, reactor::kWriteEvent);
			}
			else
			{
				fprintf(stderr, "Invalid request: %s", g_read_buffer);
				::close(m_handle);
				g_reactor.RemoveHandler(this);
			}
		}
		else
		{
			assert(0);
		}
	}

private:

	reactor::handle_t m_handle; ///< handler句柄
};

class TimeServer : public reactor::EventHandler
{
public:

	/// 构造函数
	TimeServer(const char * ip, unsigned short port) :
		EventHandler(false),
		m_ip(ip),
		m_port(port)
	{}

	/// 启动server,开始工作
	bool Start()
	{
		/// 初始化handle
		m_handle = ::socket(AF_INET, SOCK_STREAM, 0);
		if (m_handle < 0)
		{
			fprintf(stderr, "error: %s\n", strerror(errno));
			return false;
		}

		/// 绑定ip/port
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_port);
		addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
		if (::bind(m_handle, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			fprintf(stderr, "error: %s\n", strerror(errno));
			return false;
		}

		/// 监听
		if (::listen(m_handle, 10) < 0)
		{
			fprintf(stderr, "error: %s\n", strerror(errno));
			return false;
		}
		return true;
	}

    /// 获取该handler所对应的句柄
    virtual reactor::handle_t GetHandle() const
    {
        return m_handle;
    }

    /// 处理读事件的回调函数: accept到新请求
    virtual void HandleRead()
    {
		struct sockaddr addr;
		socklen_t addrlen = 0;
		reactor::handle_t handle = ::accept(m_handle, &addr, &addrlen);
		if (handle < 0)
		{
			fprintf(stderr, "error: %s\n", ::strerror(errno));
		}
		else
		{
			RequestHandler * handler = new RequestHandler(handle);
			if (g_reactor.RegisterHandler(handler, reactor::kReadEvent) != 0)
			{
				fprintf(stderr, "error: register handler failed\n");
				delete handler;
			}
		}
    }

private:

    reactor::handle_t     m_handle; ///< 服务器句柄
	std::string           m_ip;     ///< 服务器ip
	unsigned short        m_port;   ///< 服务器监听端口
};

int main(int argc, char ** argv)
{
	if (argc < 3)
	{
		fprintf(stderr, "usage: %s ip port\n", argv[0]);
		return EXIT_FAILURE;
	}

	TimeServer server(argv[1], atoi(argv[2]));
	if (!server.Start())
	{
		fprintf(stderr, "start server failed\n");
		return EXIT_FAILURE;
	}
	//printf("server started\n");
	fprintf(stderr, "server started!\n");

	while (1)
	{
		g_reactor.RegisterHandler(&server, reactor::kReadEvent);
		g_reactor.HandleEvents(100);
	}
	return EXIT_SUCCESS;
}

#endif // REACTOR_TIME_SERVER_H_
