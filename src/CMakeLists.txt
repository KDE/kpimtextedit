
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)
configure_file(config-kpimtextedit.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kpimtextedit.h )

set(kpimtextedit_texteditor_SRCS
  texteditor/plaintexteditor/plaintexteditfindbar.cpp
  texteditor/plaintexteditor/plaintexteditor.cpp
  texteditor/plaintexteditor/plaintexteditorwidget.cpp
  texteditor/plaintexteditor/plaintextsyntaxspellcheckinghighlighter.cpp
  texteditor/commonwidget/textfindreplacewidget.cpp
  texteditor/commonwidget/texteditfindbarbase.cpp
  texteditor/commonwidget/textgotolinewidget.cpp
  texteditor/commonwidget/textmessageindicator.cpp
  texteditor/richtexteditor/richtexteditor.cpp
  texteditor/richtexteditor/richtexteditfindbar.cpp
  texteditor/richtexteditor/richtexteditorwidget.cpp
)

set(kpimtextedit_composerng_SRCS
     composer-ng/richtextcomposer.cpp
     composer-ng/richtextcomposercontroler.cpp
     composer-ng/richtextcomposeractions.cpp
     composer-ng/klinkdialog.cpp
     composer-ng/nestedlisthelper.cpp
     composer-ng/richtextexternalcomposer.cpp
     composer-ng/richtextcomposerimages.cpp
     composer-ng/richtextcomposeremailquotedecorator.cpp
     composer-ng/richtextcomposeremailquotehighlighter.cpp
)

set(kpimtextedit_texttospeech_SRCS
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


set(kpimtextedit_SRCS
  emailquotehighlighter.cpp
  emoticontexteditaction.cpp
  emoticontexteditselector.cpp
  htmlhighlighter.cpp
  inserthtmldialog.cpp
  insertimagedialog.cpp
  insertimagewidget.cpp
  inserttabledialog.cpp
  selectspecialchardialog.cpp
  tableactionmenu.cpp
  tablecellformatdialog.cpp
  tableformatdialog.cpp
  textedit.cpp
  textutils.cpp
  inserttablewidget.cpp
  emailquotedecorator.cpp
  syntaxhighlighterbase.cpp
  texteditorcompleter.cpp
  slidecontainer.cpp
)

add_library(KF5PimTextEdit ${kpimtextedit_SRCS} ${kpimtextedit_texteditor_SRCS} ${kpimtextedit_texttospeech_SRCS} ${kpimtextedit_composerng_SRCS})

generate_export_header(KF5PimTextEdit BASE_NAME kpimtextedit)

add_library(KF5::PimTextEdit ALIAS KF5PimTextEdit)

target_include_directories(KF5PimTextEdit INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit;>")
target_include_directories(KF5PimTextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KF5PimTextEdit
PUBLIC
  KF5::TextWidgets
PRIVATE
  KF5::Emoticons
  KF5::Codecs
  KF5::SonnetUi
  KF5::SonnetCore
  KF5::WidgetsAddons
  KF5::KIOWidgets
  KF5::ConfigWidgets
  KF5::IconThemes
  Qt5::DBus
  Grantlee5::TextDocument
  KF5::XmlGui
)

if(KPIMTEXTEDIT_HAVE_TEXTTOSPEECH)
  target_link_libraries(KF5PimTextEdit PRIVATE Qt5::TextToSpeech)
endif()

set_target_properties(KF5PimTextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION_STRING}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
)

install(TARGETS KF5PimTextEdit EXPORT KF5PimTextEditTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### Generate Headers ###############
ecm_generate_headers(KPimTextEdit_CamelCase_HEADERS
  HEADER_NAMES
  EMailQuoteHighlighter
  EmailQuoteDecorator
  EmoticonTextEditAction
  HtmlHighlighter
  InsertHtmlDialog
  InsertImageDialog
  InsertImageWidget
  InsertTableDialog
  InsertTableWidget 
  SelectSpecialCharDialog
  SyntaxHighlighterBase
  SlideContainer
  TableCellFormatDialog
  TableFormatDialog
  TableActionMenu
  TextEdit
  TextEditorCompleter
  TextUtils

  PREFIX KPIMTextEdit
  REQUIRED_HEADERS kpimtextedit_HEADERS
)

ecm_generate_headers(PimCommon_CamelCasetextrichtexteditor_HEADERS
  HEADER_NAMES
  RichTextEditorWidget
  RichTextEditor
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
  REQUIRED_HEADERS KPimTextEdit_composerng_HEADERS
  PREFIX KPIMTextEdit
  RELATIVE composer-ng
)


########### install files ###############

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_export.h
  ${CMAKE_CURRENT_BINARY_DIR}/config-kpimtextedit.h
  ${kpimtextedit_HEADERS}
  ${KPimTextEdit_texttospeechs_HEADERS}
  ${PimCommon_richtexteditor_HEADERS}
  ${PimCommon_texteditor_commonwidget_HEADERS}
  ${PimCommon_plaintexteditor_HEADERS}
  ${KPimTextEdit_composerng_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/kpimtextedit
  COMPONENT Devel
)

install(FILES
  ${KPimTextEdit_CamelCase_HEADERS}
  ${PimCommon_CamelCasetextrichtexteditor_HEADERS}
  ${KPimTextEdit_CamelCasetexttospeechs_HEADERS}
  ${PimCommon_CamelCasetexteditor_commonwidget_HEADERS}
  ${PimCommon_CamelCasetexteditor_commonwidget_HEADERS}
  ${KPimTextEdit_Camelcasecomposerng_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/KPIMTextEdit/
  COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KF5PIMTextEdit DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KPIMTextEdit/)
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

endif()
