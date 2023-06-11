#include <iostream>
#include <thread>
#include <mutex>

#define MAX_NUM_READERS 8

static std::mutex read_mutex;
static std::binary_semaphore rw_semaphore(1);
static std::counting_semaphore  readers_semaphore(MAX_NUM_READERS);
static int reader_count = 0;

void thread_read(int id)
{
    while(1)
    {   //sleep
        int second = rand() % 2;
        std::this_thread::sleep_for(std::chrono::seconds(second));

        // allow most 8 readers to enter read queue
        readers_semaphore.acquire();
        
        read_mutex.lock();
        if(reader_count == 0)
            rw_semaphore.acquire();
        reader_count++;
        read_mutex.unlock();

        read_mutex.lock();
        // reading code
        std::cout << "Reader " << id << " is reading. There are "<< reader_count << " reading now." << std::endl;
        read_mutex.unlock();

        read_mutex.lock();
        reader_count--;
        if(reader_count == 0)
            rw_semaphore.release();
        read_mutex.unlock();

        readers_semaphore.release();
    }
}

void thread_write(int id)
{
    while(1)
    {
        // sleep
        int second = rand() % 2;
        std::this_thread::sleep_for(std::chrono::seconds(second));

        rw_semaphore.acquire();

        std::cout << "Writer " << id << " is writing." << std::endl;

        rw_semaphore.release();
    }
}

int main()
{
    std::thread reader_threads[10];

    std::thread writer(thread_write,1);

    for(int i = 0; i < 10; i++)
    {
        reader_threads[i] =  std::thread(thread_read,i);
    }
    
    writer.join();

    for(int i = 0; i < 10; i++)
    {
        reader_threads[i].join();
    }
    
    return 0;
}
