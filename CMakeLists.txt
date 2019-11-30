cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.13.0")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.63.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

if (POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW)
endif()


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)



set(KPIMTEXTEDIT_LIB_VERSION ${PIM_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    SOVERSION 5
)

set(QT_REQUIRED_VERSION "5.11.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(Grantlee5 "5.1" CONFIG REQUIRED)
set_package_properties(Grantlee5 PROPERTIES
    DESCRIPTION "A plug-in based String Template system for Qt"
    URL "https://github.com/steveire/grantlee"
    PURPOSE "Required for the RichText composer"
    TYPE REQUIRED
    )

########### Find packages ###########
find_package(KF5Codecs ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5CoreAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Sonnet ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5SyntaxHighlighting ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)

option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")

if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
   add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050e00)
   add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054000)
endif()

add_definitions(-DQT_NO_FOREACH)

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

install(FILES kpimtextedit.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
