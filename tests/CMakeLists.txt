# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/../src)
#####
add_executable(inserthtmldialog_demo inserthtmldialog_demo.cpp)
set(inserthtmldialog_demo_LIBS KF5PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets KF5::SyntaxHighlighting)
target_link_libraries(inserthtmldialog_demo ${inserthtmldialog_demo_LIBS})
#####
add_executable(slidecontainer_gui slidecontainer_gui.cpp)
target_link_libraries(slidecontainer_gui
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Widgets
    KF5PimTextEdit
    )

#####
add_executable(showemoticonunicodetab_gui showemoticonunicodetab_gui.cpp)
set(showemoticonunicodetab_gui_LIBS KF5PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets)
target_link_libraries(showemoticonunicodetab_gui ${showemoticonunicodetab_gui_LIBS})
#####
add_executable(testgeneratehtmlbuildergui_gui testgeneratehtmlbuildergui.cpp testgeneratehtmlbuildergui.h)
set(testgeneratehtmlbuildergui_gui_LIBS KF5PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets )
target_link_libraries(testgeneratehtmlbuildergui_gui ${testgeneratehtmlbuildergui_gui_LIBS})

#####
add_executable(testgenerateplaintextbuilder_gui testgenerateplaintextbuildergui.cpp testgenerateplaintextbuildergui.h)
set(testgenerateplaintextbuilder_gui_LIBS KF5PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets )
target_link_libraries(testgenerateplaintextbuilder_gui ${testgenerateplaintextbuilder_gui_LIBS})
