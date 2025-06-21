cmake_minimum_required(VERSION 3.10)
project(LibZipPlayground)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

add_executable(LibZipPlayground Main.cpp ZipArchive.cpp)

target_include_directories(LibZipPlayground PRIVATE ${LIBZIP_INCLUDE_DIRS})

target_link_libraries(LibZipPlayground PRIVATE ${LIBZIP_LIBRARIES})
