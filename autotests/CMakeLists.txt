include(ECMMarkAsTest)

set(QT_REQUIRED_VERSION "5.2.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

########### next target ###############

set(textedittest_SRCS textedittest.cpp)

add_executable(textedittest ${textedittest_SRCS})
add_test(textedittest textedittest)
ecm_mark_as_test(textedittest)
target_link_libraries(textedittest KF5PimTextEdit Qt5::Test Qt5::Widgets KF5::Mime)

########### next target ###############

set(textutilstest_SRCS textutilstest.cpp)

add_executable(textutilstest ${textedittest_SRCS})
add_test(textutilstest textutilstest)
ecm_mark_as_test(textutilstest)
target_link_libraries(textutilstest KF5PimTextEdit Qt5::Test Qt5::Widgets KF5::Mime)
