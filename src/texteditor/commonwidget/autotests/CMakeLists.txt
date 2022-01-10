# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
ecm_add_tests(textgotolinewidgettest.cpp
    NAME_PREFIX "kpimtextedit-plaintext-"
    LINK_LIBRARIES KF5::PimTextEdit Qt${QT_MAJOR_VERSION}::Test Qt${QT_MAJOR_VERSION}::Widgets
)
