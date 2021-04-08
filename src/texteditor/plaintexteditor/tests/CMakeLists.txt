set(plaintexteditor_gui_SRCS plaintexteditor_gui.cpp)
add_executable(plaintexteditor_gui ${plaintexteditor_gui_SRCS})
target_link_libraries(plaintexteditor_gui
  KF5::PimTextEdit KF5::CoreAddons Qt::Widgets KF5::I18n
)
