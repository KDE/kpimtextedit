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

#include "tableactionmenu.h"
#include <KActionCollection>
#include <KLocale>

using namespace KPIMTextEdit;

class TableActionMenu::TableActionMenuPrivate
{
public:
  TableActionMenuPrivate(KActionCollection *ac, TextEdit *edit,TableActionMenu *qq)
    : actionCollection( ac ),
      textEdit(edit),
      q(qq)
  {

  }
  KActionCollection *actionCollection;
  TextEdit *textEdit;
  TableActionMenu *q;
};

TableActionMenu::TableActionMenu(KActionCollection *ac, TextEdit *textEdit, QObject * parent)
  : KActionMenu(parent), d(new TableActionMenuPrivate(ac, textEdit,this))
{
    KActionMenu *insert = new KActionMenu(i18n("Insert"),this);

}

TableActionMenu::~TableActionMenu()
{
  delete d;
}
