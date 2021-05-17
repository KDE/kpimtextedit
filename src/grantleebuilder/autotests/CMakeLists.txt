add_definitions( -DGRANTLEEBUILDER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
ecm_add_tests(texthtmlbuildertest.cpp
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KF5::PimTextEdit Qt::Test Qt::Widgets 
)

ecm_add_tests(plaintextmarkupbuildertest.cpp
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KF5::PimTextEdit Qt::Test Qt::Widgets
)
