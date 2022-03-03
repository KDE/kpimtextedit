# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_tests(richtexteditortest.cpp
    NAME_PREFIX "kpimtextedit-richtesteditor-"
    LINK_LIBRARIES KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)

ecm_add_tests(richtexteditfindbartest.cpp
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)
