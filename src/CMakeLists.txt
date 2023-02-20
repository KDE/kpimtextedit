# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimTextEdit")

ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    SOVERSION 5
)


if(BUILD_TESTING)
    find_package(KF${KF_MAJOR_VERSION}TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()


add_library(KF5PimTextEdit)
add_library(KF5::PimTextEdit ALIAS KF5PimTextEdit)


target_sources(KF5PimTextEdit PRIVATE
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

target_sources(KF5PimTextEdit PRIVATE
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

target_sources(KF5PimTextEdit PRIVATE
    grantleebuilder/plaintextmarkupbuilder.cpp
    grantleebuilder/markupdirector.cpp
    grantleebuilder/texthtmlbuilder.cpp
    grantleebuilder/plaintextmarkupbuilder.h
    grantleebuilder/markupdirector.h
    grantleebuilder/texthtmlbuilder.h
    )

target_sources(KF5PimTextEdit PRIVATE
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
    )

target_sources(KF5PimTextEdit PRIVATE
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

ecm_qt_declare_logging_category(KF5PimTextEdit HEADER kpimtextedit_debug.h IDENTIFIER KPIMTEXTEDIT_LOG CATEGORY_NAME org.kde.pim.kpimtextedit
        DESCRIPTION "kpimtextedit (kpimtextedit)" EXPORT KPIMTEXTEDIT)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimTextEdit PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimTextEdit BASE_NAME kpimtextedit)


target_include_directories(KF5PimTextEdit INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit;>")
target_include_directories(KF5PimTextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KF5PimTextEdit
    PRIVATE
    KF${KF_MAJOR_VERSION}::SonnetUi
    KF${KF_MAJOR_VERSION}::SonnetCore
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::KIOWidgets
    KF${KF_MAJOR_VERSION}::ConfigWidgets
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::SyntaxHighlighting
    KF${KF_MAJOR_VERSION}::Codecs
    )

if (TARGET KF${KF_MAJOR_VERSION}::TextEditTextToSpeech)
    target_link_libraries(KF5PimTextEdit
        PUBLIC
        KF${KF_MAJOR_VERSION}::TextEditTextToSpeech
    )
endif()

set_target_properties(KF5PimTextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
    )

install(TARGETS KF5PimTextEdit EXPORT KF5PimTextEditTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

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
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/kpimtextedit
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
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/KPIMTextEdit/
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KF5PimTextEdit DEPS "KPIMTextEditTextToSpeech" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/)
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
        KF5PimTextEdit_QCH
        NAME KPimTextEdit
        BASE_NAME KF5PimTextEdit
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
        TARGETS KF5PimTextEdit_QCH
        FILE KF5PimTextEditQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PimTextEditQchTargets.cmake\")")
endif()


configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5PimTextEditConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KF5PimTextEditTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5PimTextEditTargets.cmake
    NAMESPACE KF5::
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit
    COMPONENT Devel
    )
