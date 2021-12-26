# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddTests)

ecm_add_tests( textutilstest.cpp  editorutiltest.cpp emoticonunicodetabtest.cpp NAME_PREFIX "kpimtextedit-" LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)
ecm_add_test(slidecontainertest.cpp
    TEST_NAME slidecontainertest
    NAME_PREFIX kpimtextedit-widget
    LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Gui KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Widgets
    )
