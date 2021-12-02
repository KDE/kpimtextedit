# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_tests(richtexteditortest.cpp
    NAME_PREFIX "kpimtextedit-richtesteditor-"
    LINK_LIBRARIES KF5::PimTextEdit Qt::Test Qt::Widgets
)
