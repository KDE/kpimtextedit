cmake_minimum_required(VERSION 3.0)
set(PIM_VERSION "5.6.40")

project(KPimTextEdit VERSION ${PIM_VERSION})

# ECM setup
set(KF5_VERSION "5.36.0")
find_package(ECM ${KF5_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

if (POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW)
endif()


include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)
include(ECMCoverageOption)


set(KPIMTEXTEDIT_LIB_VERSION ${PIM_VERSION})

ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDIT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedit_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditConfigVersion.cmake"
    SOVERSION 5
)

set(QT_REQUIRED_VERSION "5.7.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(Grantlee5 "5.1" CONFIG REQUIRED)
set_package_properties(Grantlee5 PROPERTIES
    DESCRIPTION "A plug-in based String Template system for Qt"
    URL "http://github.com/steveire/grantlee"
    PURPOSE "Required for the RichText composer"
    TYPE REQUIRED
)
find_package(Qt5Designer NO_MODULE)
set_package_properties(Qt5Designer PROPERTIES
    PURPOSE "Required to build the Qt Designer plugins"
    TYPE OPTIONAL
)

########### Find packages ###########
find_package(KF5Codecs ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5ConfigWidgets ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5CoreAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Emoticons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Sonnet ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5SyntaxHighlighting ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_VERSION} CONFIG REQUIRED)

if (Qt5Designer_FOUND)
    find_package(KF5DesignerPlugin ${KF5_VERSION} CONFIG REQUIRED)
endif()

add_definitions(-DTRANSLATION_DOMAIN=\"libkpimtextedit\")
add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
option(KPIMTEXTEDIT_NO_TEXTTOSPEECH "Do not use TextToSpeech in the kdepim applications" FALSE)

if (NOT KPIMTEXTEDIT_NO_TEXTTOSPEECH)
    find_package(Qt5 OPTIONAL_COMPONENTS TextToSpeech)
endif()

if (KPIMTEXTEDIT_NO_TEXTTOSPEECH)
    set(KPIMTEXTEDIT_HAVE_TEXTTOSPEECH FALSE)
else()
    if (${Qt5TextToSpeech_FOUND})
        set(KPIMTEXTEDIT_HAVE_TEXTTOSPEECH TRUE)
    endif()
endif()

if(BUILD_TESTING)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

########### Targets ###########
add_subdirectory(src)

if(BUILD_TESTING)
    find_package(KF5TextWidgets ${KF5_VERSION} CONFIG REQUIRED)
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
