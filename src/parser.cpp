#include "parser.h"
#include <stdexcept>
#include <iostream>


Parser::Parser(unsigned N, std::unique_ptr<AbstractBlock> &&block)
  : m_block(std::move(block)), m_N(N), m_complete(true)
{}

void Parser::parse(const std::string_view& line)
{
  bool complete{true};
  auto eol = line.find_first_of('\n');
  if(eol == std::string::npos) {
    complete = false;
  }
  else {
    m_lineno++;
  }
  auto newcmd = line.substr(0, eol);
  if(m_complete) {
    m_cmd = newcmd;
  }
  else {
    m_cmd += newcmd;
  }
  m_complete = complete;
  if(!complete) return;

  if(m_cmd == "{")
  {
    m_extendedModeLevel++;
    if(m_extendedModeLevel > 1) return;
    m_block->flush();
    return;
  }
  if(m_cmd == "}")
  {
    m_extendedModeLevel--;
    if(m_extendedModeLevel > 0) return;
    if(m_extendedModeLevel < 0) throw std::runtime_error("лишняя скобка } в строке " + std::to_string(m_lineno));
    m_block->flush();
    return;
  }
  if(m_block->cmdnum() < m_N || m_extendedModeLevel > 0) m_block->append(m_cmd);
  if(m_block->cmdnum() == m_N && m_extendedModeLevel == 0) m_block->flush();
}



void Parser::finalize()
{
  if(m_extendedModeLevel==0)
  {
    m_block->flush();
  }
}
