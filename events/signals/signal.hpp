#ifndef SIGNAL_HPP
#define SIGNAL_HPP


#include <utility>
#include "signalobject.hpp"


template<class... Args>
class Signal : public SignalObject
{
    using SigObjFunc = void (SignalObject::*)(Args...);

public:
//-------------------------------------------------------------
    Signal()
    {}

    ~Signal()
    {}
    
    Signal(const Signal &signal)             = delete;
    Signal &operator =(const Signal &signal) = delete;
//-------------------------------------------------------------
    void emit(Args... args)
    {
        for (int i = 0; i < slots_.size(); ++i)
        {
            std::pair cur_pair = slots_[i];
            if (cur_pair.first != nullptr)
            {
                (cur_pair.first->*cur_pair.second)(args...);
            }
        }
    }

    template<class T>
    void connect(T *signal_object, void (T::*function)(Args...))
    {
        slots_.push_back(std::pair{static_cast<SignalObject *> (signal_object), static_cast<SigObjFunc> (function)});
    }

    template<class T>
    void disconnect(T *signal_object, void (T::*function)(Args...))
    {
        for (int i = 0; i < slots_.size(); ++i)
        {
            std::pair cur_pair = slots_[i];
            if ((cur_pair.first == signal_object) && (cur_pair.second == function))
            {
                slots_[i].first = nullptr;
            }
        }
    }

    template<class T>
    void add_transmitter(T *signal_object, Signal<Args...> T::*transmitter)
    {
        connect(&(signal_object->*transmitter), &Signal<Args...>::emit);
    }

private:
//---------------------------------Variables------------------------------------------
    std::vector<std::pair<SignalObject*, SigObjFunc>> slots_{};
//------------------------------------------------------------------------------------
};


template<class T, class U, class... Args>
void connect(T *emitter, Signal<Args...> T::*signal, U *receiver, void (U::*slot)(Args...))
{
    assert(emitter  != nullptr);
    assert(receiver != nullptr);

    (emitter->*signal).connect(receiver, slot);
}

template<class T, class U, class... Args>
void disconnect(T *emitter, Signal<Args...> T::*signal, U *receiver, void (U::*slot)(Args...))
{
    assert(emitter  != nullptr);
    assert(receiver != nullptr);

    (emitter->*signal).disconnect(receiver, slot);
}

template<class T, class U, class... Args>
void add_transmitter(T *emitter, Signal<Args...> T::*signal, U *receiver, Signal<Args...> U::*transmitter)
{
    assert(emitter  != nullptr);
    assert(receiver != nullptr);

    (emitter->*signal).add_transmitter(receiver, transmitter);
}

#endif
