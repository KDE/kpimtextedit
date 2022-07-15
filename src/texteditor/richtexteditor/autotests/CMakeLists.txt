# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(richtexteditortest.cpp richtexteditortest.h TEST_NAME richtexteditortest
    NAME_PREFIX "kpimtextedit-richtesteditor-"
    LINK_LIBRARIES KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)

ecm_add_test(richtexteditfindbartest.cpp richtexteditfindbartest.h TEST_NAME richtexteditfindbartest
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)
