/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedit_export.h"

#include <QTextCursor>

namespace KPIMTextEdit
{
class KPIMTEXTEDIT_EXPORT EditorUtil
{
public:
    EditorUtil();
    void upperCase(QTextCursor &cursor) const;
    void lowerCase(QTextCursor &cursor) const;
    void sentenceCase(QTextCursor &cursor) const;
    void reverseCase(QTextCursor &cursor) const;
};
}

