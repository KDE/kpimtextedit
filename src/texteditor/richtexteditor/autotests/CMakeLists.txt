# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_test(richtexteditortest.cpp richtexteditortest.h TEST_NAME richtexteditortest
    NAME_PREFIX "kpimtextedit-richtesteditor-"
    LINK_LIBRARIES KPim6::TextEdit Qt::Test Qt::Widgets
)

ecm_add_test(richtexteditfindbartest.cpp richtexteditfindbartest.h TEST_NAME richtexteditfindbartest
    NAME_PREFIX "kpimtextedit-plaintexteditor-"
    LINK_LIBRARIES KPim6::TextEdit Qt::Test Qt::Widgets
)
