include(ECMAddTests)

ecm_add_tests( textutilstest.cpp  editorutiltest.cpp emoticonlistwidgetselectortest.cpp emoticonunicodetabtest.cpp NAME_PREFIX "kpimtextedit-" LINK_LIBRARIES Qt5::Test Qt5::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)
ecm_add_test(slidecontainertest.cpp
    TEST_NAME slidecontainertest
    NAME_PREFIX kpimtextedit-widget
    LINK_LIBRARIES Qt5::Test Qt5::Gui KF5::PimTextEdit Qt5::Widgets
    )
