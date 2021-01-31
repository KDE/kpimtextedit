include(ECMAddTests)

ecm_add_tests( textutilstest.cpp  editorutiltest.cpp emoticonlistwidgetselectortest.cpp emoticonunicodetabtest.cpp NAME_PREFIX "kpimtextedit-" LINK_LIBRARIES Qt::Test Qt::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)
ecm_add_test(slidecontainertest.cpp
    TEST_NAME slidecontainertest
    NAME_PREFIX kpimtextedit-widget
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimTextEdit Qt::Widgets
    )
