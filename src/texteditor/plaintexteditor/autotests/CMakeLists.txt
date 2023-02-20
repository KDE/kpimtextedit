# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(plaintexteditortest.cpp plaintexteditortest.h
    TEST_NAME plaintexteditortest
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit KF${KF_MAJOR_VERSION}::KIOWidgets Qt::Test Qt::Widgets
)

ecm_add_test(plaintexteditfindbartest.cpp plaintexteditfindbartest.h
    TEST_NAME plaintexteditfindbartest
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit KF${KF_MAJOR_VERSION}::KIOWidgets Qt::Test Qt::Widgets
)
