project(App)

add_executable(App
    src/Main.cpp
    src/ZipArchive.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
     DESTINATION ${CMAKE_BINARY_DIR}/App)

target_include_directories(App PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(App PRIVATE
    zip
)
