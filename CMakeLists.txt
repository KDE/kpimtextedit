# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.240.81")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF_MIN_VERSION "5.245.0")
set(QT_REQUIRED_VERSION "6.5.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})


include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMGenerateHeaders)

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

set(KTEXTADDONS_MIN_VERSION "1.5.2")

find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)

########### Find packages ###########
find_package(KF6Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6CoreAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6IconThemes ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Sonnet ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6SyntaxHighlighting ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Codecs ${KF_MIN_VERSION} CONFIG REQUIRED)

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")

add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.6   KF 5.245.0)


find_package(KF6TextEditTextToSpeech ${KTEXTADDONS_MIN_VERSION} CONFIG)
if (TARGET KF6::TextEditTextToSpeech)
    add_definitions(-DHAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT)
    set(HAVE_TEXTTOSPEECH TRUE)
endif()

find_package(KF6TextEmoticonsWidgets ${KTEXTADDONS_MIN_VERSION} CONFIG REQUIRED)

find_package(KF6TextCustomEditor ${KTEXTADDONS_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6TextAddonsWidgets ${KTEXTADDONS_MIN_VERSION} CONFIG REQUIRED)

find_package(Qt6Core5Compat)

if(BUILD_TESTING)
   find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test)
   add_definitions(-DBUILD_TESTING)
endif()

########### Targets ###########
add_subdirectory(src)

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
