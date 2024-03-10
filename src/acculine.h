#pragma once

#include <optional>
#include <string_view>
#include <string>

class AccuLine
{
public:
  void addNewInput(const std::string_view &inputstr)
  {
    m_curline = inputstr;
  }

  std::optional<std::string> getNextCmd()
  {
    if(m_curline.empty()) return {};
    bool complete{true};
    auto eolpos = m_curline.find_first_of('\n');
    std::string_view newcmd;
    if(eolpos == std::string::npos) {
      newcmd = m_curline;
      m_curline.remove_prefix(m_curline.length());
      complete = false;
    } else {
      newcmd = m_curline.substr(0, eolpos);
      m_curline.remove_prefix(eolpos+1);
    }
    m_cmd += newcmd;
    if(!complete) return {};
    std::string cmd = m_cmd;
    m_cmd.clear();
    return cmd;
  }

private:
  std::string m_cmd;
  std::string_view m_curline;
};
