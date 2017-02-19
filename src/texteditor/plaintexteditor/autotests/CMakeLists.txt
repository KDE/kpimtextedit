ecm_add_tests(plaintexteditortest.cpp
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit KF5::KIOWidgets Qt5::Test Qt5::Widgets
)
