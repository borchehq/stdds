# Define library. Only source files here!
project(libstdds VERSION 0.1 LANGUAGES C)

add_library(stdds SHARED src/vector.c src/list.c src/forward_list.c src/stack.c src/pair.c src/deque.c)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(stdds
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      src
)

# Depend on a library that we (may) defined in the top-level file
# target_link_libraries()

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS stdds EXPORT stdds-export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR} # This is for Windows
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# this makes the project importable from the install directory
install(EXPORT stdds-export
  FILE
    stddsConfig.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/stdds
)
# This makes the project importable from the build directory
export(TARGETS stdds FILE stddsConfig.cmake)

# Every library has unit tests, of course
add_executable(testvector test/test_vector.c)
add_executable(benchvector test/benchmark_vector.c)
add_executable(benchmark_list test/benchmark_list.c)
add_executable(testlist test/test_list.c)
add_executable(testforwardlist test/test_forward_list.c)
add_executable(benchmarkforwardlist test/benchmark_forward_list.c)
add_executable(teststack test/test_stack.c)
add_executable(testpair test/test_pair.c)
add_executable(testdeque test/test_deque.c)

target_link_libraries(testvector stdds)
target_link_libraries(benchvector stdds)
target_link_libraries(benchmark_list stdds)
target_link_libraries(testlist stdds)
target_link_libraries(testforwardlist stdds)
target_link_libraries(benchmarkforwardlist stdds)
target_link_libraries(teststack stdds)
target_link_libraries(testpair stdds)
target_link_libraries(testdeque stdds)

add_test(testvector testvector)
add_test(testlist testlist)
add_test(testforwardlist testforwardlist)
add_test(teststack teststack)
add_test(testpair testpair)
add_test(testdeque testdeque)