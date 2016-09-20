include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/../src)

add_executable(inserthtmldialog_demo inserthtmldialog_demo.cpp)
set(inserthtmldialog_demo_LIBS KF5PimTextEdit Qt5::Widgets)
if (KDEPIM_KF5SYNTAXHIGHLIGHTING_SUPPORT)
    set(inserthtmldialog_demo_LIBS ${inserthtmldialog_demo_LIBS} KF5::SyntaxHighlighting)
endif()
target_link_libraries(inserthtmldialog_demo ${inserthtmldialog_demo_LIBS})

set(slidecontainer_gui_SRCS slidecontainer_gui.cpp)
add_executable(slidecontainer_gui ${slidecontainer_gui_SRCS})
target_link_libraries(slidecontainer_gui
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    KF5PimTextEdit
    )

add_executable(syntaxhighlightertest_demo syntaxhighlightertest_demo.cpp)
target_link_libraries(syntaxhighlightertest_demo KF5PimTextEdit Qt5::Widgets)

