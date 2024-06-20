# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
#####
add_executable(inserthtmldialog_demo inserthtmldialog_demo.cpp)
set(inserthtmldialog_demo_LIBS KPim6::TextEdit Qt::Widgets KF6::SyntaxHighlighting KF6::TextAddonsWidgets)
target_link_libraries(inserthtmldialog_demo ${inserthtmldialog_demo_LIBS})
#####
add_executable(testgeneratehtmlbuildergui_gui testgeneratehtmlbuildergui.cpp testgeneratehtmlbuildergui.h)
set(testgeneratehtmlbuildergui_gui_LIBS KPim6::TextEdit Qt::Widgets)
target_link_libraries(testgeneratehtmlbuildergui_gui ${testgeneratehtmlbuildergui_gui_LIBS})

#####
add_executable(testgenerateplaintextbuilder_gui testgenerateplaintextbuildergui.cpp testgenerateplaintextbuildergui.h)
set(testgenerateplaintextbuilder_gui_LIBS KPim6::TextEdit Qt::Widgets)
target_link_libraries(testgenerateplaintextbuilder_gui ${testgenerateplaintextbuilder_gui_LIBS})
