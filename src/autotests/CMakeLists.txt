# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddTests)

ecm_add_test(textutilstest.cpp textutilstest.h
    TEST_NAME textutilstest
    NAME_PREFIX "kpimtextedit-"
    LINK_LIBRARIES Qt::Test Qt::Widgets KF6::Codecs KPim6::TextEdit KF6::XmlGui)
