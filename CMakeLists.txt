cmake_minimum_required(VERSION 3.10)

project(ZipArchive)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

add_executable(ZipArchive
    Main.cpp
    ZipArchive.cpp
)

target_include_directories(ZipArchive PRIVATE
    ${LIBZIP_INCLUDE_DIRS}
)

target_link_libraries(ZipArchive PRIVATE
    ${LIBZIP_LIBRARIES}
)
