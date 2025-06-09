# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DGRANTLEEBUILDER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
ecm_add_test(texthtmlbuildertest.cpp texthtmlbuildertest.h
    TEST_NAME texthtmlbuildertest
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KPim6::TextEdit Qt::Test Qt::Widgets
)

ecm_add_test(plaintextmarkupbuildertest.cpp plaintextmarkupbuildertest.h
    TEST_NAME plaintextmarkupbuildertest
    NAME_PREFIX "kpimtextedit-grantleebuilder-"
    LINK_LIBRARIES KPim6::TextEdit Qt::Test Qt::Widgets
)
