#pragma once

#include "outq.h"
#include "block.h"
#include "parser.h"

#include <shared_mutex>
#include <thread>
#include <map>

/** @brief Интерфейс к библиотеке async */
class BulkAsync final
{
public:
  using shparser_t = std::shared_ptr<Parser>;
  explicit BulkAsync();
  ~BulkAsync();

    /** \defgroup m1 методы необходимые из-за объявления деструктора
     *  @{ */
  BulkAsync(BulkAsync &&) = default;
  BulkAsync(const BulkAsync &) = delete;
  BulkAsync& operator=(const BulkAsync&) = delete;
  BulkAsync& operator=(BulkAsync&&) = default;
    /** @}*/

    /**
     * @brief Создаёт контекст для обработки потока команд. Может вызываться
     * несколько раз
     * @param N размер блока команд
     * @param getTime функция для генерации временных меток
     * @return контекст
     * */
  Parser* connect(unsigned N, const std::function<time_t()> &getTime = [](){ return std::time(nullptr);});


    /**
     * @brief Обработка очередной команды
     * @param void указатель на  контекст потока команд
     * @param buf указатель на буфер с командой
     * @param size размер буфера
     * */
  void receive(void *ptr, const char *buf, size_t size);


    /**
     * @brief Обработка очередной команды
     * @param shp контекст потока команд
     * @param str строка  с командой
     * */
  void receive(void *ptr, const std::string &str) {receive(ptr, str.data(), str.size());}

    /**
     * @brief Разрушить контекст потока команд.
     * @param shp контекст потока команд
     * */
  void disconnect(void *ptr);

    /**
     * @brief Разрушить все контексты потоков.
     * */
  void closeAll();

private:
  OutQueueSP m_qfile; ///!< очередь на запись в файл
  OutQueueSP m_qcout; ///!< очередь на вывод на экран
  std::thread m_worker_fw1; ///!< первый тред записи в файл
  std::thread m_worker_fw2; ///!< второй тред записи в файл
  std::thread m_worker_cw;  ///!< тред вывода на экран
  std::shared_mutex m_mapmutex; ///!< доступ к мапе
  std::map<void *, std::shared_ptr<Parser>> m_parser; ///!< контексты потоков команд
};
