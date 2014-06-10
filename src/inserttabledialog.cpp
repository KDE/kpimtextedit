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

#include "inserttabledialog.h"
#include "inserttablewidget.h"

#include <KLocalizedString>
#include <KComboBox>
#include <QDebug>
#include <KSeparator>

#include <QSpinBox>
#include <QFormLayout>

using namespace KPIMTextEdit;


class InsertTableDialog::InsertTableDialogPrivate
{
public:
    InsertTableDialogPrivate( InsertTableDialog *qq )
        : q( qq )
    {
        q->setCaption( i18n( "Insert Table" ) );
        q->setButtons( Ok|Cancel );
        q->setButtonText( KDialog::Ok, i18n( "Insert" ) );
        QWidget *page = new QWidget;
        QVBoxLayout *lay = new QVBoxLayout;
        page->setLayout(lay);
        insertTableWidget = new InsertTableWidget;
        lay->addWidget(insertTableWidget);
        KSeparator *sep = new KSeparator;
        lay->addWidget( sep );

        q->setMainWidget( page );
    }

    InsertTableWidget *insertTableWidget;
    InsertTableDialog *q;
};

InsertTableDialog::InsertTableDialog( QWidget *parent )
    : KDialog( parent ), d( new InsertTableDialogPrivate( this ) )
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

void InsertTableDialog::setColumns( int col )
{
    d->insertTableWidget->setColumns( col );
}

void InsertTableDialog::setRows( int rows )
{
    d->insertTableWidget->setRows( rows );
}

void InsertTableDialog::setBorder( int border )
{
    d->insertTableWidget->setBorder( border );
}

void InsertTableDialog::setLength( int val )
{
    d->insertTableWidget->setLength( val );
}

void InsertTableDialog::setTypeOfLength( QTextLength::Type type )
{
    d->insertTableWidget->setTypeOfLength( type );
}
