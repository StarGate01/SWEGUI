/**
 * @file Timer.hpp
 * @brief Multi-threaded timer implementation for periodic events
*/

#ifndef TIMER_HPP
#define TIMER_HPP

#include <gtkmm.h>
#include <chrono>
#include <thread>

namespace swegui
{

    /**
     * @brief Multi-threaded timer implementation for periodic events
    */
    class Timer
    {
        public:

            /**
             * @brief Constructor
             * 
             * @param period Interval, after which the timer emits a Timer::type_signal_tick
            */
            Timer(std::chrono::milliseconds period);
            void start();                                               ///<Start the timer
            void stop();                                                ///<Stop the timer
            bool is_running();                                          ///<True if the timer is running, otherwise false

            typedef sigc::signal<void> type_signal_tick;                ///<Definition for signal that is emitted, when an interval is passed
            type_signal_tick signal_tick();                             ///<Signal that is emitted, when an interval is passed

        protected:

            type_signal_tick m_signal_tick;                             ///<Internal tick signal

        private:

            std::chrono::milliseconds period;                           ///<Timer period
            std::thread* t_timer = nullptr;                             ///<Timer thread
            bool should_stop = false;                                   ///<Set to true if the timer should stop as soon as possible. Otherwise false
            bool running = false;                                       ///<True if the timer is running. Otherwise false

            Glib::Dispatcher dispatcher;                                ///<Dispatcher for inter-thread communication
            void on_thread_tick();                                      ///<Internal event handler for timer tick event

    };
    
}
#endif