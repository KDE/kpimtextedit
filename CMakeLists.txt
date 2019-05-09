cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.11.2")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.56.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

if (POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW)
endif()


include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)



set(KPIMTEXTEDIT_LIB_VERSION ${PIM_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    SOVERSION 5
)

set(QT_REQUIRED_VERSION "5.10.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(Grantlee5 "5.1" CONFIG REQUIRED)
set_package_properties(Grantlee5 PROPERTIES
    DESCRIPTION "A plug-in based String Template system for Qt"
    URL "http://github.com/steveire/grantlee"
    PURPOSE "Required for the RichText composer"
    TYPE REQUIRED
    )

find_package(KF5DesignerPlugin ${KF5_MIN_VERSION} CONFIG)
set_package_properties(KF5DesignerPlugin PROPERTIES DESCRIPTION "KF5 designer plugin" TYPE OPTIONAL)

########### Find packages ###########
find_package(KF5Codecs ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5CoreAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Emoticons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Sonnet ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5SyntaxHighlighting ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5DesignerPlugin ${KF5_MIN_VERSION} CONFIG)
set_package_properties(KF5DesignerPlugin PROPERTIES DESCRIPTION "KF5 designer plugin" TYPE OPTIONAL)

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")

# workaround for https://bugreports.qt.io/browse/QTBUG-74665 (bug in qt5.13 reevaluate it)
if (${Qt5Widgets_VERSION} STRGREATER "5.13")
   MESSAGE(STATUS "Qt version: ${Qt5Widgets_VERSION} DISABLE compile without deprecated methods. bug QTBUG-74665")
else()
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
endif()

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS TextToSpeech)

if(BUILD_TESTING)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test)
   add_definitions(-DBUILD_TESTING)
endif()

########### Targets ###########
add_subdirectory(src)

if(BUILD_TESTING)
    find_package(KF5TextWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimTextEdit")

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
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
    COMPONENT Devel
    )

install( FILES kpimtextedit.categories DESTINATION ${KDE_INSTALL_CONFDIR} )


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
