set(texttospeechwidget_gui_SRCS texttospeechwidget_gui.cpp)
add_executable(texttospeechwidget_gui ${texttospeechwidget_gui_SRCS})
target_link_libraries(texttospeechwidget_gui
    KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
    )

set(texttospeechgui_SRCS texttospeechgui.cpp)
add_executable(texttospeechgui ${texttospeechgui_SRCS})
target_link_libraries(texttospeechgui
    KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
    )

set(texttospeechconfiggui_SRCS texttospeechconfiggui.cpp)
add_executable(texttospeechconfiggui ${texttospeechconfiggui_SRCS})
target_link_libraries(texttospeechconfiggui
    KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
    )

set(texttospeechconfigdialoggui_SRCS texttospeechconfigdialoggui.cpp)
add_executable(texttospeechconfigdialoggui ${texttospeechconfigdialoggui_SRCS})
target_link_libraries(texttospeechconfigdialoggui
    KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
    )

