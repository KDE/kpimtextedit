project(kpimtextedit)

add_definitions(-DKDE_DEFAULT_DEBUG_AREA=5329)

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")

set(kpimtextedit_lib_srcs
  emailquotehighlighter.cpp
  textedit.cpp
)

kde4_add_library(kpimtextedit SHARED ${kpimtextedit_lib_srcs})

target_link_libraries(kpimtextedit
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  kmime
)

add_subdirectory(tests)

set_target_properties(kpimtextedit PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS kpimtextedit EXPORT kdepimlibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
  kpimtextedit_export.h
  textedit.h
  emailquotehighlighter.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/kpimtextedit COMPONENT Devel
)


