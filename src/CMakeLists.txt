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
    )

target_sources(KF5PimTextEdit PRIVATE
    texttospeech/texttospeech.cpp
    texttospeech/texttospeechwidget.cpp
    texttospeech/texttospeechconfigwidget.cpp
    texttospeech/texttospeechinterface.cpp
    texttospeech/abstracttexttospeechinterface.cpp
    texttospeech/abstracttexttospeechconfiginterface.cpp
    texttospeech/texttospeechconfiginterface.cpp
    texttospeech/texttospeechconfigdialog.cpp
    texttospeech/texttospeechlanguagecombobox.cpp
    texttospeech/texttospeechactions.cpp
    )

target_sources(KF5PimTextEdit PRIVATE
    grantleebuilder/plaintextmarkupbuilder.cpp
    grantleebuilder/markupdirector.cpp
    grantleebuilder/texthtmlbuilder.cpp
    )

target_sources(KF5PimTextEdit PRIVATE
    emoticon/emoticontexteditaction.cpp
    emoticon/emoticontexteditselector.cpp
    emoticon/emoticonunicodetab.cpp
    emoticon/emoticonlistwidgetselector.cpp
    emoticon/emoticonunicodeutils.cpp
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
    )

ecm_qt_declare_logging_category(KF5PimTextEdit HEADER kpimtextedit_debug.h IDENTIFIER KPIMTEXTEDIT_LOG CATEGORY_NAME org.kde.pim.kpimtextedit
        DESCRIPTION "kpimtextedit (kpimtextedit)" EXPORT KPIMTEXTEDIT)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimTextEdit PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimTextEdit BASE_NAME kpimtextedit)


target_include_directories(KF5PimTextEdit INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit;>")
target_include_directories(KF5PimTextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KF5PimTextEdit
    PRIVATE
    KF5::SonnetUi
    KF5::SonnetCore
    KF5::WidgetsAddons
    KF5::KIOWidgets
    KF5::ConfigWidgets
    KF5::XmlGui
    KF5::I18n
    KF5::SyntaxHighlighting
    Qt::TextToSpeech
    )

set_target_properties(KF5PimTextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
    )

install(TARGETS KF5PimTextEdit EXPORT KF5PimTextEditTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

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

ecm_generate_headers(PimCommon_CamelCasetextrichtexteditor_HEADERS
    HEADER_NAMES
    RichTextEditorWidget
    RichTextEditor
    RichTextEditFindBar
    REQUIRED_HEADERS PimCommon_richtexteditor_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/richtexteditor
    )

ecm_generate_headers(PimCommon_CamelCaseplaintexteditor_HEADERS
    HEADER_NAMES
    PlainTextEditor
    PlainTextEditorWidget
    PlainTextEditFindBar
    PlainTextSyntaxSpellCheckingHighlighter
    REQUIRED_HEADERS PimCommon_plaintexteditor_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/plaintexteditor
    )

ecm_generate_headers(PimCommon_CamelCasetexteditor_commonwidget_HEADERS
    HEADER_NAMES
    TextGotoLineWidget
    TextEditFindBarBase
    REQUIRED_HEADERS PimCommon_texteditor_commonwidget_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texteditor/commonwidget
    )



ecm_generate_headers(KPimTextEdit_CamelCasetexttospeechs_HEADERS
    HEADER_NAMES
    TextToSpeech
    TextToSpeechActions
    TextToSpeechInterface
    TextToSpeechWidget
    AbstractTextToSpeechInterface
    REQUIRED_HEADERS KPimTextEdit_texttospeechs_HEADERS
    PREFIX KPIMTextEdit
    RELATIVE texttospeech
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
    ${KPimTextEdit_texttospeechs_HEADERS}
    ${PimCommon_richtexteditor_HEADERS}
    ${PimCommon_texteditor_commonwidget_HEADERS}
    ${PimCommon_plaintexteditor_HEADERS}
    ${KPimTextEdit_composerng_HEADERS}
    ${KPimTextEdit_emoticon_HEADERS}
    ${KPimTextEdit_grantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/kpimtextedit
    COMPONENT Devel
    )

install(FILES
    ${KPimTextEdit_CamelCase_HEADERS}
    ${PimCommon_CamelCasetextrichtexteditor_HEADERS}
    ${PimCommon_CamelCaseplaintexteditor_HEADERS}
    ${KPimTextEdit_CamelCasetexttospeechs_HEADERS}
    ${PimCommon_CamelCasetexteditor_commonwidget_HEADERS}
    ${KPimTextEdit_Camelcasecomposerng_HEADERS}
    ${KPimTextEdit_CamelCaseemoticon_HEADERS}
    ${KPimTextEdit_CamelCasegrantlee_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/KPIMTextEdit/
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KF5PimTextEdit DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if (BUILD_TESTING)
    add_subdirectory(texteditor/plaintexteditor/autotests)
    add_subdirectory(texteditor/richtexteditor/autotests)
    add_subdirectory(texteditor/commonwidget/autotests)
    add_subdirectory(texteditor/plaintexteditor/tests)
    add_subdirectory(texteditor/richtexteditor/tests)
    add_subdirectory(texttospeech/autotests)
    add_subdirectory(texttospeech/tests)
    add_subdirectory(composer-ng/autotests)
    add_subdirectory(composer-ng/tests)
    add_subdirectory(grantleebuilder/autotests)
endif()

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()

ecm_qt_install_logging_categories(EXPORT KPIMTEXTEDIT FILE kpimtextedit.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
