#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "task.h"

/// �û��Զ���A�¼�
template<typename T>
class UserDefinedAEvent: public event_frame::Event
{
public:

    /// ���캯��
    UserDefinedAEvent(event_frame::Task * task) :
        event_frame::Event(task)
    {}

    /// �����ü��Զ���A�¼�
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessUserDefinedAEvent(this);
    };
};

/// �û��Զ���B�¼�
template<typename T>
class UserDefinedBEvent: public event_frame::Event
{
public:

    /// ���캯��
    UserDefinedBEvent(event_frame::Task * task) :
        event_frame::Event(task)
    {}

    /// �����ü��Զ���B�¼�
    virtual int Apply()
    {
        T * task = dynamic_cast<T *>(m_task);
        assert(task != NULL);
        return task->ProcessUserDefinedBEvent(this);
    };
};

/// �û��Զ��������
class UserDefinedTask : public event_frame::Task
{
public:

    /// ���������¼�
    /// @param  event ���������¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    virtual int ProcessStartEvent(event_frame::Event * event)
    {
        event_frame::StartEvent<UserDefinedTask> * evt =
            dynamic_cast<event_frame::StartEvent<UserDefinedTask> *>(event);
        assert(evt != NULL);
        /// TODO ���������¼�
        return 0;
    }

    /// ����ʱ�¼�
    /// @param  event ��ʱ�¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    virtual int ProcessTimeoutEvent(event_frame::Event * event)
    {
        event_frame::TimeoutEvent<UserDefinedTask> * evt =
            dynamic_cast<event_frame::TimeoutEvent<UserDefinedTask> *>(event);
        assert(evt != NULL);
        /// TODO ����ʱ�¼�
        return 0;
    }

    /// ����һ����ʱ�¼�
    /// @return �����ĳ�ʱ�¼���ָ��
    virtual event_frame::Event * CreateTimeoutEvent()
    {
        return new event_frame::TimeoutEvent<UserDefinedTask>(this);
    }
    
    /// �����û��Զ���A�¼�
    /// @param  event Ҫ������¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    int ProcessUserDefinedAEvent(UserDefinedAEvent<UserDefinedTask> * event)
    {
        /// TODO �����û��Զ����¼�
        fprintf(stderr, "%s\n", "ProcessUserDefinedAEvent");
        return 0;
    }

    /// �����û��Զ���B�¼�
    /// @param  event Ҫ������¼�
    /// @retval =0    �ȴ��´��¼����� 
    /// @retval >0    �ȴ���ʱ����
    /// @retval <0    �������
    int ProcessUserDefinedBEvent(UserDefinedBEvent<UserDefinedTask> * event)
    {
        /// TODO �����û��Զ����¼�
        fprintf(stderr, "%s\n", "ProcessUserDefinedBEvent");
        return 0;
    }
};

int main()
{
    event_frame::Task * task = new UserDefinedTask();
    event_frame::Event * evt_a = new UserDefinedAEvent<UserDefinedTask>(task);
    event_frame::Event * evt_b = new UserDefinedBEvent<UserDefinedTask>(task);
    evt_a->Apply();
    evt_b->Apply();
    delete evt_b;
    delete evt_a;
    delete task;
}
