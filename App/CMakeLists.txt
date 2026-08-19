project(App)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

add_executable(App
    src/Main.cpp
    src/ZipArchive.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(App PRIVATE
    ${LIBZIP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(App PRIVATE
    ${LIBZIP_LIBRARIES}
)
