#ifndef TIMER_HPP
#define TIMER_HPP

#include <gtkmm.h>
#include <chrono>
#include <thread>

namespace swegui
{

    class Timer
    {
        public:

            Timer(std::chrono::milliseconds period);
            void start();
            void stop();
            bool is_running();

            typedef sigc::signal<void> type_signal_tick;
            type_signal_tick signal_tick();

        protected:

            type_signal_tick m_signal_tick;

        private:

            std::chrono::milliseconds period;
            std::thread* t_timer = nullptr;
            bool should_stop = false;
            bool running = false;

            Glib::Dispatcher dispatcher;
            void on_thread_tick();   


    };
    
}
#endif