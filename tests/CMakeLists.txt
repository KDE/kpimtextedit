set(textedittest_SRCS textedittest.cpp)
kde4_add_unit_test(textedittest TESTNAME textedittest ${textedittest_SRCS})

target_link_libraries(textedittest
                      ${QT_QTTEST_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      kmime
                      kpimtextedit)
