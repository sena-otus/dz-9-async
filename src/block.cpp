#include "block.h"
#include "outq.h"
#include <algorithm>

Block::Block(
  wlist_t &&wlist,
  std::function<time_t()> getTime)
  : m_wlist(std::move(wlist)), m_getTime(std::move(getTime)) {}

void Block::append(const std::string_view &line) {
  if (m_cmdnum == 0) {
    m_str = line;
    const time_t newts = m_getTime();
    if (newts == m_timestamp) { // same timestamp like in previous file
      m_suffix++;                 // add suffix
    } else {                    // no suffix
      m_suffix = 0;
    }
    m_timestamp = newts;
  } else {
    m_str += ", " + std::string(line);
  }
  m_cmdnum++;
}

[[nodiscard]] unsigned Block::cmdnum() const { return m_cmdnum; }

void Block::flush() {
  if (m_cmdnum > 0) {
    OutQueue::Task task{m_str, m_timestamp, m_suffix, false};
    std::for_each(m_wlist.begin(), m_wlist.end(),
                  [&](OutQueueSP &outqptr){outqptr->put(std::move(task));});
    m_cmdnum = 0;
  }
}
