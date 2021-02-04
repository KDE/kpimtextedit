/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserttabledialog.h"
#include "inserttablewidget.h"

#include <KLocalizedString>
#include <KSeparator>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

class InsertTableDialog::InsertTableDialogPrivate
{
public:
    InsertTableDialogPrivate(InsertTableDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Insert Table"));
        auto mainLayout = new QVBoxLayout(q);

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
    InsertTableDialog *const q;
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
