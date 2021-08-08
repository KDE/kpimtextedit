cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.18.1")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.83.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)



set(KPIMTEXTEDIT_LIB_VERSION ${PIM_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    SOVERSION 5
)

set(QT_REQUIRED_VERSION "5.15.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)

########### Find packages ###########
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

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()


option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055400)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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


kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
