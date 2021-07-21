#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <thread>

template <typename T>
class MonitorThread: public std::thread{
public:
    MonitorThread(const T &value, bool &ended, const T maximum = 0);

private:
    MonitorThread() = delete;

    static void run(const T &value, bool &ended, const T maximum = 0);
};

template <typename T>
MonitorThread<T>::MonitorThread(const T &value, bool &ended, const T maximum) : std::thread(run, std::ref(value), std::ref(ended), maximum)
{
}

template <typename T>
void MonitorThread<T>::run(const T &value, bool &ended, const T maximum)
{
    std::cout << "starting monitor" << std::endl;
    while (!ended)
    {
        std::stringstream ss;
        if (maximum > 0){
            ss<< std::fixed << std::setprecision(0)<<(value + 0.1) / maximum * 100 << "%";
        }else{
            ss<<value;
            ss.clear();
        }
        std::cout << "\r" << ss.str();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << std::endl;
    std::cout << "ending monitoring" << std::endl;
}

#endif /* MONITORTHREAD_H */