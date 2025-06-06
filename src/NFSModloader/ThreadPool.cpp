#include "ThreadPool.h"

using ThreadPool = nfsloader::ThreadPool;

ThreadPool::ThreadPool(size_t threadCount)
   : m_stopFlag(false)
{
   for (size_t i = 0; i < threadCount; i++)
   {
      m_workers.emplace_back([this]() {
         while (true)
         {
            Task task;

            {
               std::unique_lock<std::mutex> lock(this->m_queueMutex);
               this->m_condition.wait(lock, [this] {
                  return this->m_stopFlag || !this->m_tasks.empty();
               });

               if (this->m_stopFlag && this->m_tasks.empty())
                  return;

               task = std::move(this->m_tasks.front());
               this->m_tasks.pop();
            }

            task();
         }
      });
   }
}

ThreadPool::~ThreadPool()
{
   {
      std::lock_guard<std::mutex> lock(m_queueMutex);
      m_stopFlag = true;
   }

   m_condition.notify_all();

   for (auto& worker : m_workers)
   {
      worker.join();
   }
}