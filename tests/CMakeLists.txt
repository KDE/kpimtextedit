include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/../src)
#add_executable(textedit_demo textedit_demo.cpp)
#target_link_libraries(textedit_demo KF5PimTextEdit)


add_executable(inserthtmldialog_demo inserthtmldialog_demo.cpp)
target_link_libraries(inserthtmldialog_demo KF5PimTextEdit)

set(slidecontainer_gui_SRCS slidecontainer_gui.cpp)
add_executable(slidecontainer_gui ${slidecontainer_gui_SRCS})
target_link_libraries(slidecontainer_gui
  Qt5::Core
  Qt5::Gui
  KF5PimTextEdit
)
