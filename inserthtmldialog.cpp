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

#include "inserthtmldialog.h"
#include <KLocale>

#include <KTextEdit>
#include <QVBoxLayout>
#include <QLabel>
using namespace KPIMTextEdit;

class InsertHtmlDialog::InsertHtmlDialogPrivate
{
public:
  InsertHtmlDialogPrivate(InsertHtmlDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Insert HTML") );
    q->setButtons( Ok|Cancel );
    q->setButtonText(KDialog::Ok,i18n("Insert"));
    QWidget *page = new QWidget(q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout(page);
    editor = new KTextEdit;
    editor->setAcceptRichText(false);
    lay->addWidget(editor);
    QLabel *label = new QLabel(i18n("Example: <i> Hello word </i>"));
    label->setTextFormat(Qt::PlainText);
    lay->addWidget(label);
  }
  KTextEdit *editor;
  InsertHtmlDialog *q;
};

InsertHtmlDialog::InsertHtmlDialog(QWidget *parent)
  : KDialog(parent), d(new InsertHtmlDialogPrivate(this))
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

#include "inserthtmldialog.moc"
