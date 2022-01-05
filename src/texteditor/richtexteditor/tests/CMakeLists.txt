# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(richtexteditor_gui richtexteditor_gui.cpp)
target_link_libraries(richtexteditor_gui
  KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
)

