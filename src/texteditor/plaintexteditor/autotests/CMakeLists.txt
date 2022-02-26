# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_tests(plaintexteditortest.cpp
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit KF5::KIOWidgets Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)

ecm_add_tests(plaintexteditfindbartest.cpp
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit KF5::KIOWidgets Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)
