/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

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

#include "inserttabledialog.h"
#include "inserttablewidget.h"

#include <KLocalizedString>
#include <KSeparator>

#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

class InsertTableDialog::InsertTableDialogPrivate
{
public:
    InsertTableDialogPrivate(InsertTableDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18n("Insert Table"));
        QVBoxLayout *mainLayout = new QVBoxLayout(q);

        insertTableWidget = new InsertTableWidget(q);
        mainLayout->addWidget(insertTableWidget);

        KSeparator *sep = new KSeparator(q);
        mainLayout->addWidget(sep);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setText(i18n("Insert"));
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        q->connect(buttonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        q->connect(buttonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);

        mainLayout->addWidget(buttonBox);
    }

    InsertTableWidget *insertTableWidget = nullptr;
    InsertTableDialog *q = nullptr;
};

InsertTableDialog::InsertTableDialog(QWidget *parent)
    : QDialog(parent)
    , d(new InsertTableDialogPrivate(this))
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

QTextLength::Type InsertTableDialog::typeOfLength() const
{
    return d->insertTableWidget->typeOfLength();
}

int InsertTableDialog::length() const
{
    return d->insertTableWidget->length();
}

void InsertTableDialog::setColumns(int col)
{
    d->insertTableWidget->setColumns(col);
}

void InsertTableDialog::setRows(int rows)
{
    d->insertTableWidget->setRows(rows);
}

void InsertTableDialog::setBorder(int border)
{
    d->insertTableWidget->setBorder(border);
}

void InsertTableDialog::setLength(int val)
{
    d->insertTableWidget->setLength(val);
}

void InsertTableDialog::setTypeOfLength(QTextLength::Type type)
{
    d->insertTableWidget->setTypeOfLength(type);
}
