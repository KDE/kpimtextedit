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

#include "tableformatdialog.h"
#include <KLocale>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

class TableFormatDialog::TableFormatDialogPrivate
{
public:
  TableFormatDialogPrivate(TableFormatDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Table Format") );
    q->setButtons( Ok|Cancel );
    QWidget *page = new QWidget(q );
    q->setMainWidget( page );
#if 0
    QVBoxLayout *lay = new QVBoxLayout(page);
    QLabel *label = new QLabel(i18n("Insert HTML tags and texts:"));
    lay->addWidget(label);
    editor = new KTextEdit;
    editor->setAcceptRichText(false);
    lay->addWidget(editor);
    label = new QLabel(i18n("Example: <i> Hello word </i>"));
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    label->setTextFormat(Qt::PlainText);
    lay->addWidget(label);
#endif
  }
  TableFormatDialog *q;
};

TableFormatDialog::TableFormatDialog(QWidget *parent)
  : KDialog(parent), d(new TableFormatDialogPrivate(this))
{
}

TableFormatDialog::~TableFormatDialog()
{
  delete d;
}

