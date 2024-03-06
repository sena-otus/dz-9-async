#include "filewriter.h"
#include "outq.h"

#include <boost/format.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

  // NOLINTNEXTLINE(performance-unnecessary-value-param)
void filewriter(OutQueueSP oq) {
  while (true) {
    try {
      const OutQueue::Task task = oq->fetch();
      if (task.m_exit) {
        return;
      }

      std::string fname;
      if (task.m_suffix == 0) {
        fname = "bulk" + std::to_string(task.m_ts) + ".log";
      } else {
        fname = str(boost::format("bulk%d-%03d.log") % task.m_ts % task.m_suffix);
      }

      // std::ofstream не позволяет создать файл экслюзивно (до c++23)
      // поэтому сначала создаём файл с помощью std::fopen(name,"wx")
      // ::open будет работать тоже, но его нет в стандарте c++
      {
        auto fp = std::unique_ptr<FILE, decltype(&fclose)>(std::fopen(fname.c_str(), "wx"), &fclose);
        if (!fp) throw std::runtime_error("ошибка создания файла '" + fname + "'");
      }
      {
        std::ofstream ofs(fname);
        if (!ofs.is_open()) throw std::runtime_error("ошибка открытия файла " + fname);
        ofs << "bulk: " << task.m_str << "\n";
        ofs.close(); // чтобы проверить ошибки записи
        if (!ofs) throw std::runtime_error("ошибка записи в файл " + fname);
      }
    } catch (std::exception &se) {
        // просто вывести в стандартный поток ошибок и продолжить
      std::cerr << se.what() << std::endl;
    }
  }
}
