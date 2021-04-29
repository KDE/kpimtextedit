/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <KActionMenu>
class QTextEdit;
namespace KPIMTextEdit
{
class TableActionMenuPrivate;

class TableActionMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit TableActionMenu(QTextEdit *textEdit);
    ~TableActionMenu() override;

    void setRichTextMode(bool richTextMode);

private:
    TableActionMenuPrivate *const d;
    friend class TableActionMenuPrivate;
};
}

