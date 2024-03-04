#pragma once

#include "abstractblock.h"
#include <string>

/**
 * @brief Parser разбирает входные строки на блоки и выводит их
 * */
class Parser final {
public:
  /**
   * @param N размер блока
   * @param block буфер для команд
   * */
  explicit Parser(unsigned N, AbstractBlock &block);

  /**
   *  @brief Разобрать строку и вывести блок по готовности
   *  @param line строка для разбора
   *  */
  void parse(const std::string &line);

  /**
   * @brief Вывести остатки данных в буфере(должно быть вызвано в конце)
   * */
  void finalize();

private:
  AbstractBlock &m_block;
  int m_lineno{0};
  int m_extendedModeLevel{0};
  unsigned m_N;
};
