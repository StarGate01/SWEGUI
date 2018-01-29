#include "Timer.hpp"

using namespace swegui;

Timer::Timer(std::chrono::milliseconds period, MainWindow* parent)
{
    this->period = period;
    this->parent = parent;
}

void Timer::start()
{
    if(running)
        return;

    running = true;
    //Spawn new thread
    runner_thread = std::thread(&Timer::run);
}

void Timer::stop()
{
    running = false;
    runner_thread.join();
}

bool Timer::isRunning()
{
    return running;
}

void Timer::run()
{
    while(running)
        wait_execute();
}

void Timer::wait_execute()
{
    std::this_thread::sleep_for(period);
    parent->on_action_timer_tick();
}