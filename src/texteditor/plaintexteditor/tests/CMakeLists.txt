# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(plaintexteditor_gui plaintexteditor_gui.cpp)
target_link_libraries(plaintexteditor_gui
  KPim${KF_MAJOR_VERSION}::TextEdit KF${KF_MAJOR_VERSION}::CoreAddons Qt::Widgets KF${KF_MAJOR_VERSION}::I18n
)
