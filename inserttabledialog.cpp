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


InsertTableWidget::InsertTableWidget(QWidget *parent)
  :QWidget(parent)
{
  mRows = new QSpinBox;
  mRows->setMinimum(1);
  mColumns = new QSpinBox;
  mColumns->setMinimum(1);

  mBorder = new QSpinBox;
  mBorder->setMinimum(0);
  mBorder->setValue(1);
  mBorder->setSuffix(i18n("px"));

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow(i18n("Rows:"), mRows);
  formLayout->addRow(i18n("Columns:"), mColumns);
  formLayout->addRow(i18n("Border:"), mBorder);


  setLayout(formLayout);
}

InsertTableWidget::~InsertTableWidget()
{
}


void InsertTableWidget::setColumns(int col)
{
  mColumns->setValue(col);
}

void InsertTableWidget::setRows(int rows)
{
  mRows->setValue(rows);
}

void InsertTableWidget::setBorder(int border)
{
  mBorder->setValue(border);
}


int InsertTableWidget::columns() const
{
  return mColumns->value();
}

int InsertTableWidget::rows() const
{
  return mRows->value();
}

int InsertTableWidget::border() const
{
  return mBorder->value();
}


class InsertTableDialog::InsertTableDialogPrivate
{
public:
  InsertTableDialogPrivate(InsertTableDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Insert Table") );
    q->setButtons( Ok|Cancel );
    q->setButtonText(KDialog::Ok,i18n("Insert"));
    insertTableWidget = new InsertTableWidget(q);
    q->setMainWidget( insertTableWidget );
  }
  InsertTableWidget *insertTableWidget;
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
  return d->insertTableWidget->columns();
}

int InsertTableDialog::rows() const
{
  return d->insertTableWidget->rows();
}

int InsertTableDialog::border() const
{
  return d->insertTableWidget->border();
}

