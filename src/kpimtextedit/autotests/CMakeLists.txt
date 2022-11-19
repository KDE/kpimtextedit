# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddTests)

ecm_add_test(textutilstest.cpp textutilstest.h
    TEST_NAME textutilstest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)

ecm_add_test(editorutiltest.cpp editorutiltest.h
    TEST_NAME editorutiltest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)

ecm_add_test(emoticonunicodetabtest.cpp emoticonunicodetabtest.h
    TEST_NAME emoticonunicodetabtest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF5::Codecs KF5PimTextEdit KF5::TextWidgets KF5::XmlGui)

ecm_add_test(slidecontainertest.cpp slidecontainertest.h
    TEST_NAME slidecontainertest
    NAME_PREFIX kpimtextedit-widget
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimTextEdit Qt::Widgets
    )
