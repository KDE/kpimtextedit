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

#include "tableformatdialog.h"
#include "inserttabledialog.h"

#include <KLocale>
#include <KComboBox>
#include <KSeparator>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>

using namespace KPIMTextEdit;

class TableFormatDialog::TableFormatDialogPrivate
{
public:
  TableFormatDialogPrivate(TableFormatDialog *qq)
    :q( qq )
  {
    q->setCaption( i18n( "Table Format" ) );
    q->setButtons( Ok|Cancel );
    QWidget *page = new QWidget( q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout( page );
    tableWidget = new InsertTableWidget;
    lay->addWidget( tableWidget );

    KSeparator *sep = new KSeparator;
    lay->addWidget( sep );

    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *lab = new QLabel( i18n( "Spacing:" ) );
    hbox->addWidget( lab );
    spacing = new QSpinBox;
    spacing->setMinimum( 0 );
    hbox->addWidget( spacing );
    lab = new QLabel( i18n( "pixels between cells" ) );
    hbox->addWidget( lab );
    lay->addLayout( hbox );

    hbox = new QHBoxLayout;
    lab = new QLabel( i18n( "Padding:" ) );
    hbox->addWidget( lab );
    padding = new QSpinBox;
    padding->setMinimum( 0 );
    hbox->addWidget( padding );
    lab = new QLabel( i18n( "pixels between cell border and content" ) );
    hbox->addWidget( lab );
    lay->addLayout( hbox );

    sep = new KSeparator;
    lay->addWidget( sep );

    alignment = new KComboBox;
    alignment->addItem( i18n( "Left" ), Qt::AlignLeft );
    alignment->addItem( i18n( "Right" ), Qt::AlignRight );
    alignment->addItem( i18n( "Center" ), Qt::AlignHCenter );
    alignment->addItem( i18n( "Justify" ), Qt::AlignJustify );

    hbox = new QHBoxLayout;
    lab = new QLabel( i18n( "Table Alignment:" ) );
    hbox->addWidget( lab );
    hbox->addWidget( alignment );

    lay->addLayout( hbox );

  }
  KComboBox *alignment;
  QSpinBox *spacing;
  QSpinBox *padding;
  KPIMTextEdit::InsertTableWidget *tableWidget;
  TableFormatDialog *q;
};

TableFormatDialog::TableFormatDialog(QWidget *parent)
  : KDialog( parent ), d( new TableFormatDialogPrivate( this ) )
{
}

TableFormatDialog::~TableFormatDialog()
{
  delete d;
}

int TableFormatDialog::columns() const
{
  return d->tableWidget->columns();
}

int TableFormatDialog::rows() const
{
  return d->tableWidget->rows();
}

int TableFormatDialog::border() const
{
  return d->tableWidget->border();
}

void TableFormatDialog::setColumns(int col)
{
  d->tableWidget->setColumns( col );
}

void TableFormatDialog::setRows(int row)
{
  d->tableWidget->setRows( row );
}

void TableFormatDialog::setBorder(int border)
{
  d->tableWidget->setBorder( border );
}

int TableFormatDialog::padding() const
{
  return d->padding->value();
}

void TableFormatDialog::setPadding(int value)
{
  d->padding->setValue( value );
}

int TableFormatDialog::spacing() const
{
  return d->spacing->value();
}

void TableFormatDialog::setSpacing(int value)
{
  d->spacing->setValue( value );
}

void TableFormatDialog::setAlignment(Qt::Alignment alignment)
{
  d->alignment->setCurrentIndex( d->alignment->findData( QVariant( alignment ) ) );
}

Qt::Alignment TableFormatDialog::alignment() const
{
  return ( Qt::Alignment )d->alignment->itemData( d->alignment->currentIndex () ).toInt();
}


QTextLength::Type TableFormatDialog::typeOfLength() const
{
  return d->tableWidget->typeOfLength();
}

int TableFormatDialog::length() const
{
  return d->tableWidget->length();
}

void TableFormatDialog::setLength(int val)
{
  d->tableWidget->setLength(val);
}

void TableFormatDialog::setTypeOfLength(QTextLength::Type type)
{
    d->tableWidget->setTypeOfLength(type);
}
