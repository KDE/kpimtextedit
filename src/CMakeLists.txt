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
)

add_library(KF5PimTextEdit ${kpimtextedit_SRCS})

generate_export_header(KF5PimTextEdit BASE_NAME kpimtextedit)

add_library(KF5::PimTextEdit ALIAS KF5PimTextEdit)

target_include_directories(KF5PimTextEdit INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KPIMTextEdit>")
target_include_directories(KF5PimTextEdit PUBLIC "$<BUILD_INTERFACE:${KPimTextEdit_SOURCE_DIR}/src;${KPimTextEdit_BINARY_DIR}/src>")


target_link_libraries(KF5PimTextEdit
  KF5::Emoticons
  KF5::KIOCore
  KF5::KDELibs4Support
  KF5::Mime
)

set_target_properties(KF5PimTextEdit PROPERTIES
    VERSION ${KPIMTEXTEDIT_VERSION_STRING}
    SOVERSION ${KPIMTEXTEDIT_SOVERSION}
    EXPORT_NAME PimTextEdit
)

install(TARGETS KF5PimTextEdit EXPORT KF5PimTextEditTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

########### Generate Headers ###############
ecm_generate_headers(KPimTextEdit_CamelCase_HEADERS
  HEADER_NAMES
  EMailQuoteHighlighter
  EmoticonTextEditAction
  HtmlHighlighter
  InsertHtmlDialog
  InsertImageDialog
  InsertImageWidget
  InsertTableDialog
  InsertTableWidget 
  SelectSpecialCharDialog
  TableCellFormatDialog
  TableFormatDialog
  TextEdit
  TextUtils

  REQUIRED_HEADERS kpimtextedit_HEADERS
)

########### install files ###############

install(FILES
  ${KPimTextEdit_CamelCase_HEADERS}
  ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_export.h
  ${kpimtextedit_HEADERS}
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KPIMTextEdit
  COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KPIMTextEdit LIB_NAME KF5PIMTextEdit DEPS "Emoticons KDELibs4Support KIO Mime" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
