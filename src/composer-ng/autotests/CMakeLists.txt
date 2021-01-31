macro(add_composer_ng_test _source)
    ecm_add_test(${_source}
        NAME_PREFIX "kpimtextedit-composerng-"
        LINK_LIBRARIES Qt::Test KF5::PimTextEdit KF5::XmlGui KF5::IconThemes
    )
endmacro()

add_composer_ng_test(richtextcomposertest.cpp)
add_composer_ng_test(richtextcomposercontrolertest.cpp)
add_composer_ng_test(richtextcomposeractionstest.cpp)
add_composer_ng_test(richtextexternalcomposertest.cpp)
add_composer_ng_test(richtextcomposerimagestest.cpp)
add_composer_ng_test(richtextcomposerwidgettest.cpp)
