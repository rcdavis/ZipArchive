cmake_minimum_required(VERSION 3.10)

project(ZipArchive)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

add_executable(ZipArchive
    src/Main.cpp
    src/ZipArchive.cpp
)

add_custom_command(TARGET ZipArchive POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:ZipArchive>/assets)

target_include_directories(ZipArchive PRIVATE
    ${LIBZIP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ZipArchive PRIVATE
    ${LIBZIP_LIBRARIES}
)
