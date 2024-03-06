#pragma once

#include <string>
#include <ctime>
#include <queue>
#include <condition_variable>
#include <thread>


class OutQueue
{
public:
  struct Task
  {
    std::string m_str; ///<! строка с командами для вывода
    std::time_t m_ts;  ///<! метка времени
    int m_suffix;      ///<! суффикс
    bool m_exit;       ///<! true - игнорировать содержимое и выйти
  };

  void put(Task && task);
  Task fetch();
  void putExit();

private:
  std::condition_variable m_event;
  std::mutex m_mutex;
  std::queue<Task> m_oq;
  bool m_exitFlag{false};
};

using OutQueueSP = std::shared_ptr<OutQueue>;
