include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/../src)
#####
add_executable(inserthtmldialog_demo inserthtmldialog_demo.cpp)
set(inserthtmldialog_demo_LIBS KF5PimTextEdit Qt5::Widgets KF5::SyntaxHighlighting)
target_link_libraries(inserthtmldialog_demo ${inserthtmldialog_demo_LIBS})
#####
set(slidecontainer_gui_SRCS slidecontainer_gui.cpp)
add_executable(slidecontainer_gui ${slidecontainer_gui_SRCS})
target_link_libraries(slidecontainer_gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    KF5PimTextEdit
    )

#####
add_executable(showemoticonunicodetab_gui showemoticonunicodetab_gui.cpp)
set(showemoticonunicodetab_gui_LIBS KF5PimTextEdit Qt5::Widgets)
target_link_libraries(showemoticonunicodetab_gui ${showemoticonunicodetab_gui_LIBS})
#####
add_executable(testgeneratehtmlbuildergui_gui testgeneratehtmlbuildergui.cpp)
set(testgeneratehtmlbuildergui_gui_LIBS KF5PimTextEdit Qt5::Widgets )
target_link_libraries(testgeneratehtmlbuildergui_gui ${testgeneratehtmlbuildergui_gui_LIBS})
