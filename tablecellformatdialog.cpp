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

#include "tablecellformatdialog.h"

#include <KSeparator>
#include <KLocale>
#include <KColorButton>
#include <KComboBox>

#include <QVBoxLayout>
#include <QLabel>

using namespace KPIMTextEdit;

class TableCellFormatDialog::TableCellFormatDialogPrivate
{
public:
  TableCellFormatDialogPrivate(TableCellFormatDialog *qq)
    :q( qq )
  {
      q->setCaption( i18n( "Cell Format" ) );
      q->setButtons( Ok|Cancel );
      QWidget *page = new QWidget( q );
      q->setMainWidget( page );
      QVBoxLayout *lay = new QVBoxLayout( page );


      QHBoxLayout *hbox = new QHBoxLayout;
      QLabel *lab = new QLabel( i18n("Vertical Alignement:") );
      hbox->addWidget(lab);
      verticalAlignement = new KComboBox;
      verticalAlignement->addItem( i18n( "Top" ), QTextCharFormat::AlignTop );
      verticalAlignement->addItem( i18n( "Middle" ), QTextCharFormat::AlignMiddle );
      verticalAlignement->addItem( i18n( "Bottom" ), QTextCharFormat::AlignBottom );


      hbox->addWidget(verticalAlignement);
      lay->addLayout(hbox);

      KSeparator *sep = new KSeparator;
      lay->addWidget( sep );

      hbox = new QHBoxLayout;
      lab = new QLabel( i18n( "Background Color:" ) );
      hbox->addWidget( lab );
      backgroundColor = new KColorButton;
      backgroundColor->setDefaultColor(Qt::white);
      hbox->addWidget( backgroundColor );
      lay->addLayout(hbox);

      sep = new KSeparator;
      lay->addWidget( sep );

    }
    KColorButton *backgroundColor;
    KComboBox *verticalAlignement;
    TableCellFormatDialog *q;
};


TableCellFormatDialog::TableCellFormatDialog(QWidget *parent)
    :KDialog(parent), d(new TableCellFormatDialogPrivate(this))
{
}

TableCellFormatDialog::~TableCellFormatDialog()
{
    delete d;
}

QColor TableCellFormatDialog::tableCellBackgroundColor() const
{
    return d->backgroundColor->color();
}

void TableCellFormatDialog::setTableCellBackgroundColor(const QColor& col)
{
    d->backgroundColor->setColor(col);
}

QTextCharFormat::VerticalAlignment TableCellFormatDialog::verticalAlignement() const
{
    return ( QTextCharFormat::VerticalAlignment )d->verticalAlignement->itemData( d->verticalAlignement->currentIndex () ).toInt();
}

void TableCellFormatDialog::setVerticalAlignement(QTextCharFormat::VerticalAlignment vertical)
{
    d->verticalAlignement->setCurrentIndex( d->verticalAlignement->findData( QVariant( vertical ) ) );
}
