ecm_add_tests(texthtmlbuildertest.cpp
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KF5::PimTextEdit Qt5::Test Qt5::Widgets Grantlee5::TextDocument
)

ecm_add_tests(plaintextmarkupbuildertest.cpp
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KF5::PimTextEdit Qt5::Test Qt5::Widgets Grantlee5::TextDocument
)
