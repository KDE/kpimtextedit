# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6TextEdit")
ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6TextEditConfigVersion.cmake"
    SOVERSION 6
)


if(BUILD_TESTING)
    find_package(KF6TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()


add_library(KPim6TextEdit)
add_library(KPim6::TextEdit ALIAS KPim6TextEdit)

target_sources(KPim6TextEdit PRIVATE
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

target_sources(KPim6TextEdit PRIVATE
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

target_sources(KPim6TextEdit PRIVATE
    grantleebuilder/plaintextmarkupbuilder.cpp
    grantleebuilder/markupdirector.cpp
    grantleebuilder/texthtmlbuilder.cpp
    grantleebuilder/plaintextmarkupbuilder.h
    grantleebuilder/markupdirector.h
    grantleebuilder/texthtmlbuilder.h
    )

target_sources(KPim6TextEdit PRIVATE
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

target_sources(KPim6TextEdit PRIVATE
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

ecm_qt_declare_logging_category(KPim6TextEdit HEADER kpimtextedit_debug.h IDENTIFIER KPIMTEXTEDIT_LOG CATEGORY_NAME org.kde.pim.kpimtextedit
        DESCRIPTION "kpimtextedit (kpimtextedit)" EXPORT KPIMTEXTEDIT)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6TextEdit PROPERTIES UNITY_BUILD ON)
endif()
ecm_generate_export_header(KPim6TextEdit
    BASE_NAME kpimtextedit
    VERSION ${KPIMTEXTEDIT_VERSION}
    DEPRECATED_BASE_VERSION 0
    USE_VERSION_HEADER
)

target_include_directories(KPim6TextEdit INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KPIMTextEdit;>")
target_include_directories(KPim6TextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KPim6TextEdit
    PRIVATE
    KF6::SonnetUi
    KF6::SonnetCore
    KF6::WidgetsAddons
    KF6::KIOWidgets
    KF6::XmlGui
    KF6::I18n
    KF6::SyntaxHighlighting
    KF6::Codecs
    )

if (TARGET KF6::TextEditTextToSpeech)
    target_link_libraries(KPim6TextEdit
        PRIVATE
        KF6::TextEditTextToSpeech
    )
endif()

set_target_properties(KPim6TextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
    )

install(TARGETS KPim6TextEdit EXPORT KPim6TextEditTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

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
    ${KPimTextEdit_grantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KPIMTextEdit/kpimtextedit
    COMPONENT Devel
    )

install(FILES
    ${KPimTextEdit_CamelCase_HEADERS}
    ${KPimTextEdit_CamelCasetextrichtexteditor_HEADERS}
    ${KPimTextEdit_CamelCaseplaintexteditor_HEADERS}
    ${KPimTextEdit_CamelCasetexteditor_commonwidget_HEADERS}
    ${KPimTextEdit_Camelcasecomposerng_HEADERS}
    ${KPimTextEdit_CamelCasegrantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KPIMTextEdit/KPIMTextEdit/
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KPim6TextEdit DEPS "KPIMTextEditTextToSpeech" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim6/KPIMTextEdit/)
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
        KPim6TextEdit_QCH
        NAME KPimTextEdit
        BASE_NAME KPim6TextEdit
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
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KPIMTEXTEDIT_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
    ecm_install_qch_export(
        TARGETS KPim6PimTextEdit_QCH
        FILE KPim6TextEditQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6TextEditQchTargets.cmake\")")
endif()


configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimTextEditConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6TextEditConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6TextEditConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6TextEditConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KPim6TextEditTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim6TextEditTargets.cmake
    NAMESPACE KPim6::
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KPIMTextEdit
    COMPONENT Devel
    )

