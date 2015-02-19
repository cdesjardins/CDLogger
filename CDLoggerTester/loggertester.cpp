#include "Logger.h"

#include <thread>
#include <list>
#include <memory>
#include <iostream>


void threadFunc(int id)
{
    for (int i = 0; i < 100; i++)
    {
        cdLog(LogLevel::Info, "thread") << "Thread " << id << ", Message " << i;
    }
}

int main()
{
    std::list<std::shared_ptr<std::thread> > oThreads;
    Logger::getLogger().addStream("test.log");
    Logger::getLogger().addStream(&std::cout);
    for (int i = 0; i < 100; i++)
    {
        oThreads.push_back(std::shared_ptr<std::thread>(new std::thread(threadFunc, i)));
    }

    // also do logging from the main thread
    for (int i = 0; i < 100; i++)
    {
        cdLog(LogLevel::Debug, "main") << "Main thread, Message " << i;
    }

    // wait for all the threads to finish
    for (std::shared_ptr<std::thread> oThread : oThreads)
    {
        oThread->join();
    }

    return 0;
}

