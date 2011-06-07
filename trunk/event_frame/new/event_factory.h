#ifndef EVENT_FRAME_EVENT_FACTORY_H_
#define EVENT_FRAME_EVENT_FACTORY_H_

template<typename T> class NewAlloc;

template<typename T, typename Alloc = NewAlloc<T> >
class EventFactory
{
public:

    static T * Create(Task * task)
    {
        return NewAlloc::Allocate<T>(task);
    }

    static void Destroy(T * t)
    {
        NewAlloc::Deallocate<T>(t);
    }
};

template<typename T>
class NewAlloc
{
public:

    static T * Allocate(Task * task)
    {
        return new T(task);
    }

    static void Deallocate(T * t)
    {
        return delete t;
    }
};

#endif // EVENT_FRAME_EVENT_FACTORY_H_

