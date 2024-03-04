cmake_minimum_required(VERSION 3.23)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

# underscores are not allowed in deb package name, but adding underscore to comply with requirements...
project(async VERSION ${PROJECT_VERSION})

option(WITH_TEST "Whether to build Google test" ON)
option(WITH_CLANG_TIDY "use clang-tidy" OFF)

find_package(Boost REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WITH_CLANG_TIDY)
  find_program(CLANGTIDY NAMES clang-tidy-15 clang-tidy-14 clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY}")
  message(NOTICE "found and enabled CLANGTIDY=${CLANGTIDY}")
endif()

configure_file(version.h.in version.h)

add_executable(async main.cpp block.cpp parser.cpp stdoutwriter.cpp filewriter.cpp outq.cpp)

target_link_libraries(async PRIVATE Boost::boost)

set_target_properties(async PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async
    PRIVATE "${CMAKE_BINARY_DIR}"
  )

if(WITH_TEST)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(gtest_async gtest_async.cpp block.cpp parser.cpp stdoutwriter.cpp filewriter.cpp outq.cpp)

    set_target_properties(gtest_async PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(gtest_async
      PRIVATE
      GTest::gtest_main
      Boost::boost
    )

    gtest_discover_tests(gtest_async)
endif()

configure_file(testinput1.txt . COPYONLY)
configure_file(testinput2.txt . COPYONLY)
configure_file(testoutput1.txt . COPYONLY)
configure_file(testoutput2.txt . COPYONLY)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --all-warnings -Wall -Werror -Wextra -pedantic")
endif()

install(TARGETS async RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
