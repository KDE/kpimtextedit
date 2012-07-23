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
#include "textedit.h"
#include "inserttabledialog.h"
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

  void _k_slotInsertRowBelow();
  void _k_slotInsertRowAbove();
  void _k_slotInsertColumnBefore();
  void _k_slotInsertColumnAfter();
  void _k_slotInsertTable();

  KAction *actionInsertTable;
  KAction *actionInsertRowBelow;
  KAction *actionInsertRowAbove;

  KAction *actionInsertColumnBefore;
  KAction *actionInsertColumnAfter;

  KActionCollection *actionCollection;
  TextEdit *textEdit;
  TableActionMenu *q;
};

void TableActionMenu::TableActionMenuPrivate::_k_slotInsertRowBelow()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {

  }
}

void TableActionMenu::TableActionMenuPrivate::_k_slotInsertRowAbove()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {

  }
}

void TableActionMenu::TableActionMenuPrivate::_k_slotInsertColumnBefore()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {

  }
}

void TableActionMenu::TableActionMenuPrivate::_k_slotInsertTable()
{
  if(textEdit->textMode() == KRichTextEdit::Rich ) {
    InsertTableDialog *dialog = new InsertTableDialog(textEdit);
    if(dialog->exec()) {
      QTextCursor cursor = textEdit->textCursor();
      QTextTableFormat tableFormat;
      tableFormat.setBorder(dialog->border());
      cursor.insertTable( dialog->rows(),dialog->columns(), tableFormat );
    }
    delete dialog;
  }
}


TableActionMenu::TableActionMenu(KActionCollection *ac, TextEdit *textEdit, QObject * parent)
  : KActionMenu(parent), d(new TableActionMenuPrivate(ac, textEdit,this))
{
    KActionMenu *insertMenu = new KActionMenu(i18n("Insert"),this);
    addAction(insertMenu);

    d->actionInsertTable = new KAction( i18n( "Insert Table" ), this );
    insertMenu->addAction(d->actionInsertTable);
    ac->addAction( QLatin1String( "insert_new_table" ), d->actionInsertTable );
    connect( d->actionInsertTable, SIGNAL(triggered(bool)), SLOT(_k_slotInsertTable()) );

    d->actionInsertRowBelow = new KAction( i18n( "Row Below" ), this );
    insertMenu->addAction(d->actionInsertRowBelow);
    ac->addAction( QLatin1String( "insert_row_below" ), d->actionInsertRowBelow );
    connect( d->actionInsertRowBelow, SIGNAL(triggered(bool)), SLOT(_k_slotInsertRowBelow()) );

    d->actionInsertRowAbove = new KAction( i18n( "Insert Row Above" ), this );
    insertMenu->addAction(d->actionInsertRowAbove);
    ac->addAction( QLatin1String( "insert_row_above" ), d->actionInsertRowAbove );
    connect( d->actionInsertRowAbove, SIGNAL(triggered(bool)), SLOT(_k_slotInsertRowAbove()) );


    d->actionInsertColumnBefore = new KAction( i18n( "Column Before" ), this );
    insertMenu->addAction(d->actionInsertColumnBefore);
    ac->addAction( QLatin1String( "insert_column_before" ), d->actionInsertColumnBefore );

    connect( d->actionInsertColumnBefore, SIGNAL(triggered(bool)), SLOT(_k_slotInsertColumnBefore()) );

    d->actionInsertColumnAfter = new KAction( i18n( "Insert Column After" ), this );
    insertMenu->addAction(d->actionInsertColumnAfter);
    ac->addAction( QLatin1String( "insert_column_after" ), d->actionInsertColumnAfter );
    connect( d->actionInsertColumnAfter, SIGNAL(triggered(bool)), SLOT(_k_slotInsertColumnAfter()) );

    KActionMenu *deleteMenu = new KActionMenu(i18n("Delete"),this);
    addAction(deleteMenu);


}

TableActionMenu::~TableActionMenu()
{
  delete d;
}
