# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(plaintexteditor_gui plaintexteditor_gui.cpp)
target_link_libraries(plaintexteditor_gui
  KPim6::TextEdit KF6::CoreAddons Qt::Widgets KF6::I18n
)
