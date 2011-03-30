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
/// @brief  Reactor��,�û�ͨ�������ṩ�Ľӿ�ʹ��Reactor�Ĺ���
/// @author zeshengwu<wuzesheng@vip.qq.com>
/// @date   2011-03-27

namespace reactor
{
/// �¼�����
typedef unsigned int event_t;
enum
{
    kReadEvent    = 0x01, ///< ���¼�
    kWriteEvent   = 0x02, ///< д�¼�
    kErrorEvent   = 0x04, ///< �����¼�
    kEventMask    = 0xff  ///< �¼�����
};

/// �������
#if defined(_WIN32)
    typedef ::SOCKET handle_t;
#elif defined(__linux__)
    typedef int32_t handle_t;
#else
#error "Ŀǰ����֧�ָ�ƽ̨"
#endif // _WIN32

/// �¼�������
class EventHandler
{
public:

	/// ���캯��
	EventHandler(bool is_own_memory = false) :
		m_is_reactor_own_memory(is_own_memory)
	{}

    /// ��ȡ��handler����Ӧ�ľ��
    virtual handle_t GetHandle() const = 0;

    /// ������¼��Ļص�����
    virtual void HandleRead() {}

    /// ����д�¼��Ļص�����
    virtual void HandleWrite() {}

    /// ��������¼��Ļص�����
    virtual void HandleError() {}

protected:

    /// ���캯��,ֻ�������
    EventHandler() {}

    /// ��������,ֻ�������
    virtual ~EventHandler() {}

protected:

	bool m_is_reactor_own_memory; ///< reactor�Ƿ����handler���ڴ�
};

/// reactor��ʵ����
/// TODO (zeshengwu) ʵ�ֶ��̰߳�ȫ�汾
class ReactorImplementation;

/// reactor�ӿ���
class Reactor
{
public:

    /// ���캯��
    Reactor();

    /// ��������
    ~Reactor();

    /// ��reactor��ע���ע�¼�evt��handler(������)
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
    /// @param  timeout ��ʱʱ��(����)
    void HandleEvents(int timeout = 0);

private:

    ReactorImplementation * m_reactor_impl; ///< reactor��ʵ����
};
} // namespace reactor
#endif // REACTOR_REACTOR_H_
