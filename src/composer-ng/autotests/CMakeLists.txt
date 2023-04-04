# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_composer_ng_test _source)

    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "kpimtextedit-composerng-"
        LINK_LIBRARIES Qt::Test KPim6::TextEdit KF6::XmlGui KF6::IconThemes KF6::Codecs
    )
endmacro()

add_composer_ng_test(richtextcomposertest.cpp)
add_composer_ng_test(richtextcomposercontrolertest.cpp)
add_composer_ng_test(richtextcomposeractionstest.cpp)
add_composer_ng_test(richtextexternalcomposertest.cpp)
add_composer_ng_test(richtextcomposerimagestest.cpp)
add_composer_ng_test(richtextcomposerwidgettest.cpp)
