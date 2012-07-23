/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TABLEACTIONMENU_H
#define TABLEACTIONMENU_H

#include <KActionMenu>

class KActionCollection;

namespace KPIMTextEdit {
class TextEdit;

class TableActionMenu : public KActionMenu
{
public:
  explicit TableActionMenu(KActionCollection *ac, TextEdit *textEdit, QObject * parent);
  ~TableActionMenu();

private:
  class TableActionMenuPrivate;
  TableActionMenuPrivate *d;
  Q_PRIVATE_SLOT( d, void _k_slotInsertRowBelow() )
  Q_PRIVATE_SLOT( d, void _k_slotInsertRowAbove() )
  Q_PRIVATE_SLOT( d, void _k_slotInsertColumnBefore() )
  Q_PRIVATE_SLOT( d, void _k_slotInsertColumnAfter() )
  Q_PRIVATE_SLOT( d, void _k_slotInsertTable() )

};
}

#endif // TABLEACTIONMENU_H
