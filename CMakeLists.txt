project(kpimtextedit)

add_definitions(-DKDE_DEFAULT_DEBUG_AREA=5329)

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")

set(kpimtextedit_lib_srcs
  emailquotehighlighter.cpp
  textedit.cpp
  textutils.cpp
  emoticontexteditaction.cpp
  emoticontexteditselector.cpp
  inserthtmldialog.cpp
  inserttabledialog.cpp
  tableactionmenu.cpp
  tableformatdialog.cpp
  insertimagedialog.cpp
  htmlhighlighter.cpp
  insertimagewidget.cpp
  tablecellformatdialog.cpp
  selectspecialchar.cpp
)

include_directories(
  ${CMAKE_BINARY_DIR}/kpimidentities
  ${CMAKE_BINARY_DIR}/kmime
)

kde4_add_library(kpimtextedit ${LIBRARY_TYPE} ${kpimtextedit_lib_srcs})
generate_export_header(kpimtextedit)

target_link_libraries(kpimtextedit
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KEMOTICONS_LIBS}
  kmime
)

add_subdirectory(tests)

set_target_properties(kpimtextedit PROPERTIES
  VERSION ${GENERIC_LIB_VERSION}
  SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(TARGETS kpimtextedit EXPORT kdepimlibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_export.h
  textedit.h
  textutils.h
  emailquotehighlighter.h
  htmlhighlighter.h
  emoticontexteditaction.h
  inserthtmldialog.h
  inserttabledialog.h
  tableformatdialog.h
  insertimagedialog.h
  insertimagewidget.h
  tablecellformatdialog.h
  selectspecialchar.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/kpimtextedit COMPONENT Devel
)
