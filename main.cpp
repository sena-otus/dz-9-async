#include "async.h"
#include <iostream>
#include <stdexcept>


const int generic_errorcode = 200;

int main(int argc[[maybe_unused]], char const *argv[[maybe_unused]][] ) {
  try {
    std::size_t bulk = 5;
    auto *h = async::connect(bulk);
    auto *h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);
    return 0;
  }
  catch(const std::exception &e)
  {
    std::cerr << "Ошибка\n" << e.what() << std::endl;
    return generic_errorcode;
  }
}
