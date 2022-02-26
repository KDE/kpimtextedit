# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_kpimtextedit_texttospeech_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        NAME_PREFIX "kpimtextedit-texttospeech-"
        LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets
        )
endmacro()

add_kpimtextedit_texttospeech_unittest( texttospeechwidgettest.cpp )
add_kpimtextedit_texttospeech_unittest( texttospeechconfigwidgettest.cpp )
add_kpimtextedit_texttospeech_unittest( texttospeechactionstest.cpp )
