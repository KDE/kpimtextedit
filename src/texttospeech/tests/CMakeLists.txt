# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(texttospeechwidget_gui texttospeechwidget_gui.cpp)
target_link_libraries(texttospeechwidget_gui
    KF5::PimTextEdit KF5::CoreAddons Qt${QT_MAJOR_VERSION}::Widgets KF5::I18n
    )

add_executable(texttospeechgui texttospeechgui.cpp)
target_link_libraries(texttospeechgui
    KF5::PimTextEdit KF5::CoreAddons Qt${QT_MAJOR_VERSION}::Widgets KF5::I18n
    )

add_executable(texttospeechconfiggui texttospeechconfiggui.cpp)
target_link_libraries(texttospeechconfiggui
    KF5::PimTextEdit KF5::CoreAddons Qt${QT_MAJOR_VERSION}::Widgets KF5::I18n
    )

add_executable(texttospeechconfigdialoggui texttospeechconfigdialoggui.cpp)
target_link_libraries(texttospeechconfigdialoggui
    KF5::PimTextEdit KF5::CoreAddons Qt${QT_MAJOR_VERSION}::Widgets KF5::I18n
    )

