# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
set(richtextcomposerwidget_gui_SRCS richtextcomposerwidget_gui.cpp)
add_executable(richtextcomposerwidget_gui ${richtextcomposerwidget_gui_SRCS})
target_link_libraries(
    richtextcomposerwidget_gui
    KPim6::TextEdit
    KF6::XmlGui
    KF6::I18n
)
