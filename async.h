#pragma once

#include "outq.h"
#include "block.h"
#include "parser.h"

#include <thread>
#include <set>


class BulkAsync final
{
public:
  using shparser_t = std::shared_ptr<Parser>;
  explicit BulkAsync();
  ~BulkAsync();

  BulkAsync(BulkAsync &&) = default;
  BulkAsync(const BulkAsync &) = delete;
  BulkAsync& operator=(const BulkAsync&) = delete;
  BulkAsync& operator=(BulkAsync&&) = default;

  shparser_t connect(unsigned N, const std::function<time_t()> &getTime = [](){ return std::time(nullptr);});
  void receive(const shparser_t &shp, const char *buf, size_t size);
  void disconnect(const shparser_t &shp);

private:
  OutQueueSP m_qfile;
  OutQueueSP m_qcout;
  std::thread m_worker_fw1;
  std::thread m_worker_fw2;
  std::thread m_worker_cw;
  std::set<std::shared_ptr<Parser>> m_parser;
};
