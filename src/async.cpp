#include "async.h"
#include "stdoutwriter.h"
#include "filewriter.h"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string_view>

BulkAsync::BulkAsync()
  : m_qfile(std::make_shared<OutQueue>()),
    m_qcout(std::make_shared<OutQueue>()),
    m_worker_fw1(std::thread(filewriter , m_qfile)), // start 1st file writer thread
    m_worker_fw2(std::thread(filewriter , m_qfile)), // start 2nd file writer thread
    m_worker_cw (std::thread(stdoutwriter,m_qcout)) // start single cout writer thread
{
}

BulkAsync::~BulkAsync()
{
  if(m_qfile) // not already closed...
  {
    closeAll();
  }
}


BulkAsync::shparser_t BulkAsync::connect(const unsigned N, const std::function<time_t()>& getTime)
{
  if(!m_qfile || !m_qcout) return nullptr;
  shparser_t parser = std::make_shared<Parser>(N, std::make_unique<Block>(Block::wlist_t{m_qfile, m_qcout}, getTime));
  m_parser.emplace(parser);
  return parser;
}

void
BulkAsync::receive(const shparser_t &shp, const char *buf, size_t size)
{
  assert((buf != nullptr));
  assert((m_qfile != nullptr));
    // execute only if it is "our" shp (stored in set)
  if(m_parser.contains(shp)) shp->parse(std::string_view(buf, size));
}


void BulkAsync::disconnect(const shparser_t &shp)
{
    // execute only if it is "our" shp (stored in set)
  auto pit = m_parser.find(shp);
  if(pit != m_parser.end()) {
    shp->finalize();
    m_parser.erase(pit);
  }
}

void BulkAsync::closeAll()
{
  for(auto && shp : m_parser)
  {
    shp->finalize();
  }
  m_parser.clear();

  m_qfile->putExit();
  m_qcout->putExit();
  m_worker_fw1.join();
  m_worker_fw2.join();
  m_worker_cw.join();
  m_qfile = nullptr;
  m_qcout = nullptr;
}
