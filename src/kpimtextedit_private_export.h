/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef KPIMTEXTEDIT_TESTS_EXPORT
#define KPIMTEXTEDIT_TESTS_EXPORT KPIMTEXTEDIT_EXPORT
#endif
#else /* not compiling tests */
#define KPIMTEXTEDIT_TESTS_EXPORT
#endif

