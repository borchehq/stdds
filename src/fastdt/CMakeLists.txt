# Define library. Only source files here!
project(libfastdt VERSION 0.1 LANGUAGES C)

add_library(fastdt SHARED src/vector.c)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(fastdt
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      src
)

# Depend on a library that we (may) defined in the top-level file
# target_link_libraries()

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS fastdt EXPORT fastdt-export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR} # This is for Windows
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# this makes the project importable from the install directory
install(EXPORT fastdt-export
  FILE
    fastdtConfig.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/fastdt
)
# This makes the project importable from the build directory
export(TARGETS fastdt FILE fastdtConfig.cmake)

# Every library has unit tests, of course
add_executable(testvector test/test_vector.c)
add_executable(benchvector test/benchmark_vector.c)

target_link_libraries(testvector fastdt)
target_link_libraries(benchvector fastdt)

add_test(testvector testvector)
