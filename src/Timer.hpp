#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <thread>

namespace swegui
{

    class Timer
    {
        public:

            Timer(std::chrono::milliseconds period, MainWindow* parent);
            void start();
            void stop();
            bool isRunning();

        private:
            std::chrono::milliseconds period;
            std::thread runner_thread;
            bool running = false;
            swegui::MainWindow* parent = nullptr;

            void wait_execute();
            void run();
    };
    
}
#endif