/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef KPIMTEXTEDIT_TABLEACTIONMENU_H
#define KPIMTEXTEDIT_TABLEACTIONMENU_H

#include <KActionMenu>
#include <QTextEdit>
namespace KPIMTextEdit
{
class TableActionMenuPrivate;

class TableActionMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit TableActionMenu(QTextEdit *textEdit);
    ~TableActionMenu();

    void setRichTextMode(bool richTextMode);

private:
    TableActionMenuPrivate *const d;
    friend class TableActionMenuPrivate;
};
}

#endif // KPIMTEXTEDIT_TABLEACTIONMENU_H
