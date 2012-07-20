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

#include "inserttabledialog.h"

#include <KLocale>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>


using namespace KPIMTextEdit;

class InsertTableDialog::InsertTableDialogPrivate
{
public:
  InsertTableDialogPrivate(InsertTableDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Insert Table") );
    q->setButtons( Ok|Cancel );
    q->setButtonText(KDialog::Ok,i18n("Insert"));
    QWidget *page = new QWidget(q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout(page);
    QLabel *lab = new QLabel(i18n("Rows:"));
    rows = new QSpinBox;
    rows->setMinimum(1);
    lay->addWidget(lab);
    lay->addWidget(rows);

    lab = new QLabel(i18n("Columns:"));
    lay->addWidget(lab);
    columns = new QSpinBox;
    columns->setMinimum(1);
    lay->addWidget(columns);
  }
  QSpinBox *columns;
  QSpinBox *rows;
  InsertTableDialog *q;
};

InsertTableDialog::InsertTableDialog(QWidget *parent)
  : KDialog(parent), d(new InsertTableDialogPrivate(this))
{
}

InsertTableDialog::~InsertTableDialog()
{
  delete d;
}

int InsertTableDialog::columns() const
{
  return d->columns->value();
}

int InsertTableDialog::rows() const
{
  return d->rows->value();
}
