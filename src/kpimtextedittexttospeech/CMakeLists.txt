# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_library(KF5PimTextEditTextToSpeech)
add_library(KF5::PimTextEditTextToSpeech ALIAS KF5PimTextEditTextToSpeech)


target_sources(KF5PimTextEditTextToSpeech PRIVATE
)

ecm_qt_declare_logging_category(KF5PimTextEditTextToSpeech HEADER kpimtextedittexttospeech_debug.h
    IDENTIFIER KPIMTEXTEDITTEXTTOSPEECH_LOG CATEGORY_NAME org.kde.pim.kpimtextedit.texttospeech
        DESCRIPTION "kpimtextedit (kpimtextedit text to speech)" EXPORT KPIMTEXTEDIT)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimTextEditTextToSpeech PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimTextEditTextToSpeech BASE_NAME kpimtextedittexttospeech)

target_include_directories(KF5PimTextEditTextToSpeech INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech;>")
target_include_directories(KF5PimTextEditTextToSpeech PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KF5PimTextEditTextToSpeech
    PRIVATE
    KF5::I18n
    )

install(TARGETS KF5PimTextEditTextToSpeech EXPORT KF5PimTextEditTextToSpeechTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedittexttospeech_export.h
#    ${CMAKE_CURRENT_BINARY_DIR}/../../kpimtextedit-texttospeech.h
#    ${kpimtextedit_HEADERS}
#    ${KPimTextEdit_richtexteditor_HEADERS}
#    ${KPimTextEdit_texteditor_commonwidget_HEADERS}
#    ${KPimTextEdit_plaintexteditor_HEADERS}
#    ${KPimTextEdit_composerng_HEADERS}
#    ${KPimTextEdit_emoticon_HEADERS}
#    ${KPimTextEdit_grantlee_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech/kpimtextedittexttospeech
#    COMPONENT Devel
#    )

#install(FILES
#    ${KPimTextEdit_CamelCase_HEADERS}
#    ${KPimTextEdit_CamelCasetextrichtexteditor_HEADERS}
#    ${KPimTextEdit_CamelCaseplaintexteditor_HEADERS}
#    ${KPimTextEdit_CamelCasetexteditor_commonwidget_HEADERS}
#    ${KPimTextEdit_Camelcasecomposerng_HEADERS}
#    ${KPimTextEdit_CamelCaseemoticon_HEADERS}
#    ${KPimTextEdit_CamelCasegrantlee_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/KPIMTextEdit/
#    COMPONENT Devel
#    )

#ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KF5PimTextEdit DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/)
#install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
