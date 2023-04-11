# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddTests)

ecm_add_test(textutilstest.cpp textutilstest.h
    TEST_NAME textutilstest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF6::Codecs KPim6::TextEdit KF6::TextWidgets KF6::XmlGui)

ecm_add_test(editorutiltest.cpp editorutiltest.h
    TEST_NAME editorutiltest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF6::Codecs KPim6::TextEdit KF6::TextWidgets KF6::XmlGui)

ecm_add_test(slidecontainertest.cpp slidecontainertest.h
    TEST_NAME slidecontainertest
    NAME_PREFIX kpimtextedit-widget
    LINK_LIBRARIES Qt::Test Qt::Gui KPim6::TextEdit Qt::Widgets
    )
