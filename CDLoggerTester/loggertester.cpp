#include "Logger.h"
#include <thread>
#include <list>
#include <memory>
#include <iostream>

void threadFunc(int id)
{
    try
    {
        for (int i = 0; i < 100; i++)
        {
            cdLog(LogLevel::Info) << "Thread " << id << ", Message " << i;
        }
    }
    catch (const std::exception& ex)
    {
        cdLog(LogLevel::Error) << "thread exception: " << ex.what();
    }
}

int main()
{
    try
    {
        std::list<std::shared_ptr<std::thread> > threads;
        Logger::getLogger().addStream("test.log");
        Logger::getLogger().addStream(&std::cout);
        for (int i = 0; i < 100; i++)
        {
            threads.push_back(std::shared_ptr<std::thread>(new std::thread(threadFunc, i)));
        }

        for (int i = 0; i < 100; i++)
        {
            cdLog(LogLevel::Debug) << "Main thread, Message " << i;
        }

        for (std::shared_ptr<std::thread> thread : threads)
        {
            thread->join();
        }
    }
    catch (const std::exception& ex)
    {
        cdLog(LogLevel::Error) << "main exception: " << ex.what();
    }

    return 0;
}

