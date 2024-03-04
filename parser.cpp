#include "parser.h"
#include <stdexcept>


Parser::Parser(unsigned N, AbstractBlock &block)
  : m_block(block), m_N(N)
{}

void Parser::parse(const std::string& line)
{
  m_lineno++;
  if(line == "{")
  {
    m_extendedModeLevel++;
    if(m_extendedModeLevel > 1) return;
    m_block.flush();
    return;
  }
  if(line == "}")
  {
    m_extendedModeLevel--;
    if(m_extendedModeLevel > 0) return;
    if(m_extendedModeLevel < 0) throw std::runtime_error("лишняя скобка } в строке " + std::to_string(m_lineno));
    m_block.flush();
    return;
  }
  if(m_block.cmdnum() < m_N || m_extendedModeLevel > 0) m_block.append(line);
  if(m_block.cmdnum() == m_N && m_extendedModeLevel == 0) m_block.flush();
}



void Parser::finalize()
{
  if(m_extendedModeLevel==0)
  {
    m_block.flush();
  }
}
