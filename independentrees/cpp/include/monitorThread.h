#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <iostream>
#include <thread>

template <typename T>
class MonitorThread: public std::thread{
public:
    MonitorThread(const T &value, bool &ended);

private:
    MonitorThread() = delete;

    static void run(const T &value, bool &ended);
};

template <typename T>
MonitorThread<T>::MonitorThread(const T &value, bool &ended) : std::thread(run, std::ref(value), std::ref(ended))
{
}

template <typename T>
void MonitorThread<T>::run(const T &value, bool &ended)
{
    std::cout << "starting monitor" << std::endl;
    while (!ended)
    {
        std::cout << "\r" << value;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << std::endl;
    std::cout << "ending monitoring" << std::endl;
}

#endif /* MONITORTHREAD_H */