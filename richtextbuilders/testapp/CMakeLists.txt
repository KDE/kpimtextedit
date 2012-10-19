PROJECT( krichtexteditor )
include_directories(${CMAKE_SOURCE_DIR}/kpimtextedit/richtextbuilders
	${CMAKE_SOURCE_DIR}/kpimtextedit/)

SET(krichtexteditorSources
  main.cpp
  krichtexteditor.cpp
)

KDE4_ADD_EXECUTABLE(krichtexteditor ${krichtexteditorSources} )
TARGET_LINK_LIBRARIES(krichtexteditor ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kpimtextedit )

install(TARGETS krichtexteditor ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
  krichtexteditorui.rc
  DESTINATION  ${DATA_INSTALL_DIR}/krichtexteditor)



