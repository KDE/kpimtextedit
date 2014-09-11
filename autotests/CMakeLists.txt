include(ECMAddTests)

set(QT_REQUIRED_VERSION "5.2.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

ecm_add_tests( textedittest.cpp textutilstest.cpp  NAME_PREFIX "kpimtextedit-" LINK_LIBRARIES Qt5::Test Qt5::Widgets KF5::Mime KF5PimTextEdit KF5::IconThemes KF5::TextWidgets)
