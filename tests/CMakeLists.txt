include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/../src)
add_executable(textedit_demo textedit_demo.cpp)
target_link_libraries(textedit_demo KF5PimTextEdit)
