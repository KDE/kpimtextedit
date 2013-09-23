add_definitions("-DQT_GUI_LIB") # to be able to use keyboard events like QTest::keyClick()

include_directories(
  ${CMAKE_SOURCE_DIR}/kpimtextedit
  ${CMAKE_BINARY_DIR}/kpimtextedit
)

set(textedittest_SRCS textedittest.cpp)
kde4_add_unit_test(textedittest TESTNAME textedittest ${textedittest_SRCS})

target_link_libraries(textedittest
                      ${QT_QTTEST_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      kmime
                      kpimtextedit)

set(textutilstest_SRCS textutilstest.cpp)
kde4_add_unit_test(textutilstest TESTNAME textutilstest ${textutilstest_SRCS})

target_link_libraries(textutilstest
                      ${QT_QTTEST_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      kpimtextedit)
