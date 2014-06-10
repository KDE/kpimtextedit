/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#ifndef KPIMTEXTEDIT_TABLEACTIONMENU_H
#define KPIMTEXTEDIT_TABLEACTIONMENU_H

#include <KActionMenu>

namespace KPIMTextEdit {

class TextEdit;
class TableActionMenuPrivate;

class TableActionMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit TableActionMenu( TextEdit *textEdit );
    ~TableActionMenu();

    QList<QAction *> listAction() const;
private:
    TableActionMenuPrivate * const d;
    friend class TableActionMenuPrivate;

    Q_PRIVATE_SLOT( d, void _k_slotInsertRowBelow() )
    Q_PRIVATE_SLOT( d, void _k_slotInsertRowAbove() )
    Q_PRIVATE_SLOT( d, void _k_slotInsertColumnBefore() )
    Q_PRIVATE_SLOT( d, void _k_slotInsertColumnAfter() )
    Q_PRIVATE_SLOT( d, void _k_slotInsertTable() )
    Q_PRIVATE_SLOT( d, void _k_slotRemoveRowBelow() )
    Q_PRIVATE_SLOT( d, void _k_slotRemoveRowAbove() )
    Q_PRIVATE_SLOT( d, void _k_slotRemoveColumnBefore() )
    Q_PRIVATE_SLOT( d, void _k_slotRemoveColumnAfter() )
    Q_PRIVATE_SLOT( d, void _k_slotRemoveCellContents() )
    Q_PRIVATE_SLOT( d, void _k_slotMergeCell() )
    Q_PRIVATE_SLOT( d, void _k_slotMergeSelectedCells() )
    Q_PRIVATE_SLOT( d, void _k_slotTableCellFormat() )
    Q_PRIVATE_SLOT( d, void _k_slotTableFormat() )
    Q_PRIVATE_SLOT( d, void _k_slotSplitCell() )
    Q_PRIVATE_SLOT( d, void _k_updateActions(bool forceUpdate = false) )
};

}

#endif // KPIMTEXTEDIT_TABLEACTIONMENU_H
