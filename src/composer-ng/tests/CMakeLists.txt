# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
set(richtextcomposerwidget_gui_SRCS richtextcomposerwidget_gui.cpp)
add_executable(richtextcomposerwidget_gui ${richtextcomposerwidget_gui_SRCS})
target_link_libraries(richtextcomposerwidget_gui
  KPim${KF_MAJOR_VERSION}::TextEdit KF${KF_MAJOR_VERSION}::XmlGui KF${KF_MAJOR_VERSION}::I18n
)

