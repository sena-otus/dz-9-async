#include "outq.h"

void OutQueue::put(Task &&task)
{
  {
    std::lock_guard<std::mutex> lg(m_mutex);
    m_oq.emplace(task);
  }
  m_event.notify_one();
}

void OutQueue::putExit()
{
  put({"", 0, 0, true});
}


OutQueue::Task OutQueue::fetch()
{
  std::unique_lock<std::mutex> ul(m_mutex);
  m_event.wait(ul, [this]{return !m_oq.empty();});
  auto task = m_oq.front();
  if(!task.m_exit)
  {
      // "exit" task stays in queue forever
    m_oq.pop();
  }
  return task;
}
