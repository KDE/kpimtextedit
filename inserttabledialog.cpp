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
#include <QFormLayout>


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
    rows = new QSpinBox;
    rows->setMinimum(1);
    columns = new QSpinBox;
    columns->setMinimum(1);

    border = new QSpinBox;
    border->setMinimum(0);
    border->setValue(1);
    border->setSuffix(i18n("px"));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(i18n("Rows:"), rows);
    formLayout->addRow(i18n("Columns:"), columns);
    formLayout->addRow(i18n("Border:"), border);
    page->setLayout(formLayout);
  }
  QSpinBox *columns;
  QSpinBox *rows;
  QSpinBox *border;
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

int InsertTableDialog::border() const
{
  return d->border->value();
}
