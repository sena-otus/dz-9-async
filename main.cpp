#include "block.h"
#include "parser.h"
#include "stdoutwriter.h"
#include "filewriter.h"
#include "outq.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>


const int generic_errorcode = 101;

int main(int argc, char const *argv[] )
{
  try {
    unsigned N{0};

    if(argc != 2)
    {
      std::cout << "Запуск:\n bulk <N>\n\n где <N> это количество команд в блоке для обработки\n";
      return 0;
    }

    try {
      N = std::stoul(argv[1]);
    }
    catch(...) {
      throw std::runtime_error("Единственный аргумент командной строки дожен быть положительным числом");
    }

    auto qfile = std::make_shared<OutQueue>(); // queue for file writers
    auto qcout = std::make_shared<OutQueue>(); // queue for std::cout writer
    auto worker_fw1 = std::thread(filewriter, qfile); // start 1st file writer thread
    auto worker_fw2 = std::thread(filewriter, qfile); // start 2nd file writer thread
    auto worker_cw  = std::thread(stdoutwriter, qcout); // start single cout writer thread
    Block block({qfile, qcout});
    Parser parser(N, block);
    for(std::string line; std::getline(std::cin, line);) {
      parser.parse(line);
    }
    parser.finalize();
    qfile->putExit();
    qcout->putExit();
    worker_fw1.join();
    worker_fw2.join();
    worker_cw.join();
    return 0;
  }
  catch(const std::exception &e)
  {
    std::cerr << "Ошибка\n" << e.what() << std::endl;
    return generic_errorcode;
  }
}
