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

#include "inserthtmldialog.h"
#include "htmlhighlighter.h"
#include <KLocale>

#include <KTextEdit>
#include <QVBoxLayout>
#include <QLabel>
namespace KPIMTextEdit {

class InsertHtmlDialogPrivate
{
public:
  InsertHtmlDialogPrivate(InsertHtmlDialog *qq)
    :q( qq )
  {
    q->setCaption( i18n( "Insert HTML" ) );
    q->setButtons( KDialog::Ok|KDialog::Cancel );
    q->setButtonText( KDialog::Ok, i18n( "Insert" ) );
    QWidget *page = new QWidget( q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout( page );
    QLabel *label = new QLabel( i18n( "Insert HTML tags and texts:" ) );
    lay->addWidget( label );
    editor = new KTextEdit;
    new HtmlHighlighter( editor->document() );
    editor->setAcceptRichText( false );
    editor->setFocus();
    lay->addWidget( editor );
    label = new QLabel( i18n( "Example: <i> Hello word </i>" ) );
    QFont font = label->font();
    font.setBold( true );
    label->setFont( font );
    label->setTextFormat( Qt::PlainText );
    lay->addWidget( label );
    q->connect( editor, SIGNAL(textChanged()),q,SLOT(_k_slotTextChanged()) );
    q->enableButtonOk( false );
    q->resize(640,480);
  }

  void _k_slotTextChanged();

  KTextEdit *editor;
  InsertHtmlDialog *q;
};

void InsertHtmlDialogPrivate::_k_slotTextChanged()
{
  q->enableButtonOk( !editor->toPlainText().isEmpty() );
}

InsertHtmlDialog::InsertHtmlDialog(QWidget *parent)
  : KDialog( parent ), d( new InsertHtmlDialogPrivate( this ) )
{
}

InsertHtmlDialog::~InsertHtmlDialog()
{
  delete d;
}

QString InsertHtmlDialog::html() const
{
  return d->editor->toPlainText();
}

}

#include "moc_inserthtmldialog.cpp"
