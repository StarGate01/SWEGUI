/**
 * @file Timer.cpp
 * 
 * @brief Timer for the play functionality
*/

#include "Timer.hpp"

using namespace swegui;

Timer::Timer(std::chrono::milliseconds period)
{
    this->period = period;
    dispatcher.connect(sigc::mem_fun(this, &Timer::on_thread_tick));
}

Timer::type_signal_tick Timer::signal_tick()
{
    return m_signal_tick;
}

void Timer::start()
{
    if(running) return;
    should_stop = false;
    running = true;
    t_timer = new std::thread([this] 
    {
        while(!should_stop)
        {
            std::this_thread::sleep_for(period);
            dispatcher.emit();
        }
        dispatcher.emit();
    });
}

void Timer::stop()
{
   should_stop = true;
}

bool Timer::is_running()
{
    return running;
}

void Timer::on_thread_tick()
{
    if(t_timer && should_stop)
    {
        if (t_timer->joinable()) t_timer->join();
        delete t_timer;
        t_timer = nullptr;
        running = false;
    }
    if(!should_stop) m_signal_tick.emit();
}
