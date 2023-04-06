# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim${KF_MAJOR_VERSION}TextEdit")
set(KPIMTEXTEDIT_KF5_COMPAT FALSE)
ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}TextEditConfigVersion.cmake"
    SOVERSION 5
)


if(BUILD_TESTING)
    find_package(KF${KF_MAJOR_VERSION}TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()


add_library(KPim${KF_MAJOR_VERSION}TextEdit)
add_library(KPim${KF_MAJOR_VERSION}::TextEdit ALIAS KPim${KF_MAJOR_VERSION}TextEdit)

target_sources(KPim${KF_MAJOR_VERSION}TextEdit PRIVATE
    texteditor/plaintexteditor/plaintexteditfindbar.cpp
    texteditor/plaintexteditor/plaintexteditor.cpp
    texteditor/plaintexteditor/plaintexteditorwidget.cpp
    texteditor/plaintexteditor/plaintextsyntaxspellcheckinghighlighter.cpp
    texteditor/commonwidget/findutils.cpp
    texteditor/commonwidget/textfindreplacewidget.cpp
    texteditor/commonwidget/texteditfindbarbase.cpp
    texteditor/commonwidget/textgotolinewidget.cpp
    texteditor/commonwidget/textmessageindicator.cpp
    texteditor/richtexteditor/richtexteditor.cpp
    texteditor/richtexteditor/richtexteditfindbar.cpp
    texteditor/richtexteditor/richtexteditorwidget.cpp
    texteditor/plaintexteditor/plaintexteditfindbar.h
    texteditor/plaintexteditor/plaintexteditor.h
    texteditor/plaintexteditor/plaintexteditorwidget.h
    texteditor/plaintexteditor/plaintextsyntaxspellcheckinghighlighter.h
    texteditor/commonwidget/findutils.h
    texteditor/commonwidget/textfindreplacewidget.h
    texteditor/commonwidget/texteditfindbarbase.h
    texteditor/commonwidget/textgotolinewidget.h
    texteditor/commonwidget/textmessageindicator.h
    texteditor/richtexteditor/richtexteditor.h
    texteditor/richtexteditor/richtexteditfindbar.h
    texteditor/richtexteditor/richtexteditorwidget.h
    )

target_sources(KPim${KF_MAJOR_VERSION}TextEdit PRIVATE
    composer-ng/richtextcomposer.cpp
    composer-ng/richtextcomposercontroler.cpp
    composer-ng/richtextcomposeractions.cpp
    composer-ng/klinkdialog.cpp
    composer-ng/nestedlisthelper.cpp
    composer-ng/richtextexternalcomposer.cpp
    composer-ng/richtextcomposerimages.cpp
    composer-ng/richtextcomposeremailquotedecorator.cpp
    composer-ng/richtextcomposeremailquotehighlighter.cpp
    composer-ng/richtextcomposerwidget.cpp
    composer-ng/richtextcomposer.h
    composer-ng/richtextcomposercontroler.h
    composer-ng/richtextcomposeractions.h
    composer-ng/klinkdialog_p.h
    composer-ng/nestedlisthelper_p.h
    composer-ng/richtextexternalcomposer.h
    composer-ng/richtextcomposerimages.h
    composer-ng/richtextcomposeremailquotedecorator.h
    composer-ng/richtextcomposeremailquotehighlighter.h
    composer-ng/richtextcomposerwidget.h
    )

target_sources(KPim${KF_MAJOR_VERSION}TextEdit PRIVATE
    grantleebuilder/plaintextmarkupbuilder.cpp
    grantleebuilder/markupdirector.cpp
    grantleebuilder/texthtmlbuilder.cpp
    grantleebuilder/plaintextmarkupbuilder.h
    grantleebuilder/markupdirector.h
    grantleebuilder/texthtmlbuilder.h
    )

target_sources(KPim${KF_MAJOR_VERSION}TextEdit PRIVATE
    emoticon/emoticontexteditaction.cpp
    emoticon/emoticontexteditselector.cpp
    emoticon/emoticonunicodetab.cpp
    emoticon/emoticonunicodeutils.cpp
    emoticon/emoticonunicodemodel.cpp
    emoticon/emoticonlistview.cpp
    emoticon/emoticonunicodeproxymodel.cpp
    emoticon/emoticonunicodemodelmanager.cpp
    emoticon/emoticonrecentusedfilterproxymodel.cpp
    emoticon/emoticonrecentlistview.cpp

    emoticon/emoticontexteditaction.h
    emoticon/emoticontexteditselector.h
    emoticon/emoticonunicodetab.h
    emoticon/emoticonunicodeutils.h
    emoticon/emoticonunicodemodel.h
    emoticon/emoticonlistview.h
    emoticon/emoticonunicodeproxymodel.h
    emoticon/emoticonunicodemodelmanager.h
    emoticon/emoticonrecentusedfilterproxymodel.h
    emoticon/emoticonrecentlistview.h


    emoticon/emoticoncategory.cpp
    emoticon/emoticoncategory.h
    emoticon/unicodeemoticon.cpp
    emoticon/unicodeemoticon.h
    emoticon/unicodeemoticonmanager.cpp
    emoticon/unicodeemoticonmanager.h
    emoticon/unicodeemoticonparser.cpp
    emoticon/unicodeemoticonparser.h
    emoticon/emoticoncategorymodelfilterproxymodel.cpp
    emoticon/emoticoncategorymodelfilterproxymodel.h
    emoticon/emoji.qrc
    )

target_sources(KPim${KF_MAJOR_VERSION}TextEdit PRIVATE
    inserthtmldialog.cpp
    insertimagedialog.cpp
    insertimagewidget.cpp
    inserttabledialog.cpp
    selectspecialchardialog.cpp
    tableactionmenu.cpp
    tablecellformatdialog.cpp
    tableformatdialog.cpp
    textutils.cpp
    inserttablewidget.cpp
    texteditorcompleter.cpp
    inserthtmleditor.cpp
    slidecontainer.cpp
    editorutil.cpp

    inserthtmldialog.h
    insertimagedialog.h
    insertimagewidget.h
    inserttabledialog.h
    selectspecialchardialog.h
    tableactionmenu.h
    tablecellformatdialog.h
    tableformatdialog.h
    textutils.h
    inserttablewidget.h
    texteditorcompleter.h
    inserthtmleditor.h
    slidecontainer.h
    editorutil.h
    )

ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}TextEdit HEADER kpimtextedit_debug.h IDENTIFIER KPIMTEXTEDIT_LOG CATEGORY_NAME org.kde.pim.kpimtextedit
        DESCRIPTION "kpimtextedit (kpimtextedit)" EXPORT KPIMTEXTEDIT)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}TextEdit PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim${KF_MAJOR_VERSION}TextEdit BASE_NAME kpimtextedit)


target_include_directories(KPim${KF_MAJOR_VERSION}TextEdit INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KPIMTextEdit;>")
target_include_directories(KPim${KF_MAJOR_VERSION}TextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KPim${KF_MAJOR_VERSION}TextEdit
    PRIVATE
    KF${KF_MAJOR_VERSION}::SonnetUi
    KF${KF_MAJOR_VERSION}::SonnetCore
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::KIOWidgets
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::SyntaxHighlighting
    KF${KF_MAJOR_VERSION}::Codecs
    )

if (TARGET KF${KF_MAJOR_VERSION}::TextEditTextToSpeech)
    target_link_libraries(KPim${KF_MAJOR_VERSION}TextEdit
        PUBLIC
        KF${KF_MAJOR_VERSION}::TextEditTextToSpeech
    )
endif()

set_target_properties(KPim${KF_MAJOR_VERSION}TextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
    )

install(TARGETS KPim${KF_MAJOR_VERSION}TextEdit EXPORT KPim${KF_MAJOR_VERSION}TextEditTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

########### Generate Headers ###############
ecm_generate_headers(KPimTextEdit_CamelCase_HEADERS
    HEADER_NAMES
    EditorUtil
    SelectSpecialCharDialog
    SlideContainer
    TextEditorCompleter
    TextUtils

    PREFIX KPIMTextEdit
    REQUIRED_HEADERS kpimtextedit_HEADERS
    )

ecm_generate_headers(KPimTextEdit_CamelCaseemoticon_HEADERS
    HEADER_NAMES
    EmoticonUnicodeTab
    PREFIX KPIMTextEdit
    REQUIRED_HEADERS kpimtextedit_HEADERS
    RELATIVE emoticon
    )

ecm_generate_headers(KPimTextEdit_CamelCasegrantlee_HEADERS
    HEADER_NAMES
    PlainTextMarkupBuilder
    TextHTMLBuilder
    MarkupDirector
    AbstractMarkupBuilder
    PREFIX KPIMTextEdit
    REQUIRED_HEADERS kpimtextedit_HEADERS
    RELATIVE grantleebuilder
    )

ecm_generate_headers(KPimTextEdit_CamelCasetextrichtexteditor_HEADERS
    HEADER_NAMES
    RichTextEditorWidget
    RichTextEditor
    RichTextEditFindBar
    REQUIRED_HEADERS KPimTextEdit_richtexteditor_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/richtexteditor
    )

ecm_generate_headers(KPimTextEdit_CamelCaseplaintexteditor_HEADERS
    HEADER_NAMES
    PlainTextEditor
    PlainTextEditorWidget
    PlainTextEditFindBar
    PlainTextSyntaxSpellCheckingHighlighter
    REQUIRED_HEADERS KPimTextEdit_plaintexteditor_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/plaintexteditor
    )

ecm_generate_headers(KPimTextEdit_CamelCasetexteditor_commonwidget_HEADERS
    HEADER_NAMES
    TextGotoLineWidget
    TextEditFindBarBase
    REQUIRED_HEADERS KPimTextEdit_texteditor_commonwidget_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/commonwidget
    )



ecm_generate_headers(KPimTextEdit_Camelcasecomposerng_HEADERS
    HEADER_NAMES
    RichTextComposer
    RichTextComposerControler
    RichTextComposerImages
    RichTextExternalComposer
    RichTextComposerActions
    RichTextComposerEmailQuoteHighlighter
    RichTextComposerWidget
    REQUIRED_HEADERS KPimTextEdit_composerng_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE composer-ng
    )


########### install files ###############

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_export.h
    ${kpimtextedit_HEADERS}
    ${KPimTextEdit_richtexteditor_HEADERS}
    ${KPimTextEdit_texteditor_commonwidget_HEADERS}
    ${KPimTextEdit_plaintexteditor_HEADERS}
    ${KPimTextEdit_composerng_HEADERS}
    ${KPimTextEdit_emoticon_HEADERS}
    ${KPimTextEdit_grantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KPIMTextEdit/kpimtextedit
    COMPONENT Devel
    )

install(FILES
    ${KPimTextEdit_CamelCase_HEADERS}
    ${KPimTextEdit_CamelCasetextrichtexteditor_HEADERS}
    ${KPimTextEdit_CamelCaseplaintexteditor_HEADERS}
    ${KPimTextEdit_CamelCasetexteditor_commonwidget_HEADERS}
    ${KPimTextEdit_Camelcasecomposerng_HEADERS}
    ${KPimTextEdit_CamelCaseemoticon_HEADERS}
    ${KPimTextEdit_CamelCasegrantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KPIMTextEdit/KPIMTextEdit/
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KPim${KF_MAJOR_VERSION}TextEdit DEPS "KPIMTextEditTextToSpeech" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KPIMTextEdit/)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if (BUILD_TESTING)
    add_subdirectory(texteditor/plaintexteditor/autotests)
    add_subdirectory(texteditor/richtexteditor/autotests)
    add_subdirectory(texteditor/commonwidget/autotests)
    add_subdirectory(texteditor/plaintexteditor/tests)
    add_subdirectory(texteditor/richtexteditor/tests)
    add_subdirectory(composer-ng/autotests)
    add_subdirectory(composer-ng/tests)
    add_subdirectory(grantleebuilder/autotests)
endif()

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()

ecm_qt_install_logging_categories(EXPORT KPIMTEXTEDIT FILE kpimtextedit.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

if(BUILD_TESTING)
    add_subdirectory(emoticon/autotests)
endif()	

if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}TextEdit_QCH
        NAME KPimTextEdit
        BASE_NAME KPim${KF_MAJOR_VERSION}TextEdit
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${kpimtextedit_HEADERS}
            ${KPimTextEdit_richtexteditor_HEADERS}
            ${KPimTextEdit_texteditor_commonwidget_HEADERS}
            ${KPimTextEdit_plaintexteditor_HEADERS}
            ${KPimTextEdit_composerng_HEADERS}
            ${KPimTextEdit_emoticon_HEADERS}
            ${KPimTextEdit_grantlee_HEADERS}
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KPIMTEXTEDIT_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
    ecm_install_qch_export(
        TARGETS KPim${KF_MAJOR_VERSION}PimTextEdit_QCH
        FILE KPim${KF_MAJOR_VERSION}TextEditQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim${KF_MAJOR_VERSION}TextEditQchTargets.cmake\")")
endif()


configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimTextEditConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}TextEditConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}TextEditConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}TextEditConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KPim${KF_MAJOR_VERSION}TextEditTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim${KF_MAJOR_VERSION}TextEditTargets.cmake
    NAMESPACE KPim${KF_MAJOR_VERSION}::
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KPIMTextEdit
    COMPONENT Devel
    )

if (BUILD_WITH_COMPAT_LIBS)
if (QT_MAJOR_VERSION STREQUAL "5")
##
# TODO: Backwards compatiblity. Remove in next major version
##
set(CMAKECONFIG_INSTALL_DIR_KF5 "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimTextEdit")
set(KPIMTEXTEDIT_KF5_COMPAT TRUE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimTextEditConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR_KF5}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}TextEditConfigVersion.cmake"
    RENAME "KF5PimTextEditConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(EXPORT KPim${KF_MAJOR_VERSION}TextEditTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    FILE KPim${KF_MAJOR_VERSION}TextEditTargets.cmake
    NAMESPACE KF5::
)
endif()
endif()
