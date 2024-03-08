find_package(GTest REQUIRED)

enable_testing()


add_executable(gtest_async gtest_async.cpp)

set_target_properties(gtest_async PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
)

target_include_directories(gtest_async
  PRIVATE "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/src"
)


target_link_libraries(gtest_async
  PRIVATE
  GTest::gtest_main
  async
)

gtest_discover_tests(gtest_async)

configure_file(testinput1.txt . COPYONLY)
configure_file(testinput2.txt . COPYONLY)
configure_file(testoutput1.txt . COPYONLY)
configure_file(testoutput2.txt . COPYONLY)
