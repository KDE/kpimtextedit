include_directories(${CMAKE_SOURCE_DIR}/kpimtextedit)
#set(htmlbuildertest_SRCS htmlbuildertest.cpp)
#kde4_add_unit_test(htmlbuildertest TESTNAME htmlbuildertest ${htmlbuildertest_SRCS})

#target_link_libraries(htmlbuildertest
#                      ${QT_QTTEST_LIBRARY}
#                      ${QT_QTCORE_LIBRARY}
#                      ${QT_QTGUI_LIBRARY}
#                      ${KDE4_KDECORE_LIBS}
#                      ${KDE4_KDEUI_LIBS}
#                      kpimtextedit)

set(plainmarkupbuildertest_SRCS plainmarkupbuildertest.cpp)
kde4_add_unit_test(plainmarkupbuildertest TESTNAME plainmarkupbuildertest ${plainmarkupbuildertest_SRCS})

target_link_libraries(plainmarkupbuildertest
                      ${QT_QTTEST_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      kpimtextedit)


