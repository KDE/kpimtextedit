# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.22.43")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF_MIN_VERSION "5.102.0")

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)

include(ECMAddQch)


option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(KPIMTEXTEDIT_LIB_VERSION ${PIM_VERSION})

set(QT_REQUIRED_VERSION "5.15.2")
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_MIN_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
    set(KTEXTADDONS_MIN_VERSION "1.0.42")
else()
    set(KF_MAJOR_VERSION "5")
    set(KTEXTADDONS_MIN_VERSION "1.0.0")
endif()

find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)

########### Find packages ###########
find_package(KF${KF_MAJOR_VERSION}Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}ConfigWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}CoreAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}IconThemes ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Sonnet ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}SyntaxHighlighting ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Codecs ${KF_MIN_VERSION} CONFIG REQUIRED)

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

option(USE_KTEXTADDONS_LIB "Use ktextaddons lib (experimental)" ON)
option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")

ecm_set_disabled_deprecation_versions(QT 6.4   KF 5.102.0)


if (USE_KTEXTADDONS_LIB)
    find_package(KF5TextEditTextToSpeech ${KTEXTADDONS_MIN_VERSION} CONFIG)
    if (TARGET KF5::TextEditTextToSpeech)
        add_definitions(-DHAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT)
        set(KPIMTEXTEDIT_TEXT_TO_SPEECH TRUE)
    endif()
else()
    find_package(Qt${QT_MAJOR_VERSION}TextToSpeech ${QT_REQUIRED_VERSION} CONFIG)
    set_package_properties(Qt${QT_MAJOR_VERSION}TextToSpeech PROPERTIES DESCRIPTION
        "Add support for text to speech"
        TYPE OPTIONAL
    )
    if(TARGET Qt${QT_MAJOR_VERSION}::TextToSpeech)
        add_definitions(-DHAVE_TEXT_TO_SPEECH_SUPPORT)
        set(KPIMTEXTEDIT_TEXT_TO_SPEECH TRUE)
    endif()
endif()


configure_file(kpimtextedit-texttospeech.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit-texttospeech.h )


if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(Qt6Core5Compat)
endif()

if(BUILD_TESTING)
   find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test)
   add_definitions(-DBUILD_TESTING)
endif()

########### Targets ###########
add_subdirectory(src)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
