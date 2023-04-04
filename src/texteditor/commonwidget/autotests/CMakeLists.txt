# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause

macro(add_kpimtextedit_commonwidget_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        NAME_PREFIX "kpimtextedit-plaintext-"
        LINK_LIBRARIES Qt::Test KPim6::TextEdit Qt::Widgets
        )
endmacro()


add_kpimtextedit_commonwidget_unittest(textgotolinewidgettest.cpp)

add_kpimtextedit_commonwidget_unittest(textfindwidgettest.cpp)

add_kpimtextedit_commonwidget_unittest(textreplacewidgettest.cpp)
