#include "parser.h"
#include <stdexcept>
#include <iostream>
#include <string_view>


Parser::Parser(unsigned N, std::unique_ptr<AbstractBlock> &&block)
  : m_block(std::move(block)), m_N(N)
{}



void Parser::parse(const std::string_view& line)
{
  // std::string_view curline{line};
  // while(!curline.empty())
  // {
  //   bool complete{true};
  //   auto eolpos = curline.find_first_of('\n');
  //   std::string_view newcmd;
  //   if(eolpos == std::string::npos) {
  //     newcmd = curline;
  //     curline.remove_prefix(curline.length());
  //     complete = false;
  //   } else {
  //     newcmd = curline.substr(0, eolpos);
  //     curline.remove_prefix(eolpos+1);
  //     m_lineno++;
  //   }
  //   m_cmd += newcmd;
  //   if(!complete) return;
  //   auto cmd = m_cmd;
  //   m_cmd.clear();
  m_acculine.addNewInput(line);
  std::optional<std::string> cmd;
  while((cmd = m_acculine.getNextCmd()))
  {
    if(*cmd == "{")
    {
      m_extendedModeLevel++;
      if(m_extendedModeLevel > 1) continue;
      m_block->flush();
      continue;
    }
    if(*cmd == "}")
    {
      m_extendedModeLevel--;
      if(m_extendedModeLevel > 0) continue;
      if(m_extendedModeLevel < 0) throw std::runtime_error("лишняя скобка } в строке " + std::to_string(m_lineno));
      m_block->flush();
      continue;
    }
    if(m_block->cmdnum() < m_N || m_extendedModeLevel > 0) m_block->append(*cmd);
    if(m_block->cmdnum() == m_N && m_extendedModeLevel == 0) m_block->flush();
  }
}




void Parser::finalize()
{
  if(m_extendedModeLevel==0)
  {
    m_block->flush();
  }
}
