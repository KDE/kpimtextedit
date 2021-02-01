macro(add_kpimtextedit_texttospeech_unittest _source)
    ecm_add_test(${_source}
        NAME_PREFIX "kpimtextedit-texttospeech-"
        LINK_LIBRARIES Qt5::Test KF5::PimTextEdit Qt5::Widgets
        )
endmacro()

add_kpimtextedit_texttospeech_unittest( texttospeechwidgettest.cpp )
add_kpimtextedit_texttospeech_unittest( texttospeechconfigwidgettest.cpp )
add_kpimtextedit_texttospeech_unittest( texttospeechactionstest.cpp )
