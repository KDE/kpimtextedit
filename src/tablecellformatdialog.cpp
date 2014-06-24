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

#include <KColorButton>
#include <KComboBox>
#include <KLocalizedString>
#include <KSeparator>

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>

using namespace KPIMTextEdit;

class TableCellFormatDialog::TableCellFormatDialogPrivate
{
public:
    TableCellFormatDialogPrivate( TableCellFormatDialog *qq )
        : q( qq )
    {
        q->setWindowTitle( i18n( "Cell Format" ) );
        QVBoxLayout *mainLayout = new QVBoxLayout;
        q->setLayout(mainLayout);

        QHBoxLayout *hbox = new QHBoxLayout;
        QLabel *lab = new QLabel( i18n( "Vertical Alignment:" ) );
        hbox->addWidget(lab);
        verticalAlignment = new KComboBox;
        verticalAlignment->addItem( i18n( "Top" ), QTextCharFormat::AlignTop );
        verticalAlignment->addItem( i18n( "Middle" ), QTextCharFormat::AlignMiddle );
        verticalAlignment->addItem( i18n( "Bottom" ), QTextCharFormat::AlignBottom );

        hbox->addWidget( verticalAlignment );
        mainLayout->addLayout( hbox );

        KSeparator *sep = new KSeparator;
        mainLayout->addWidget( sep );

        hbox = new QHBoxLayout;
        useBackgroundColor = new QCheckBox( i18n( "Background Color:" ) );
        hbox->addWidget( useBackgroundColor );
        backgroundColor = new KColorButton;
        backgroundColor->setDefaultColor( Qt::white );
        hbox->addWidget( backgroundColor );
        mainLayout->addLayout(hbox);

        sep = new KSeparator;
        mainLayout->addWidget( sep );
        backgroundColor->setEnabled( false );
        q->connect( useBackgroundColor, SIGNAL(toggled(bool)),
                    backgroundColor, SLOT(setEnabled(bool)) );

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        buttonBox->button(QDialogButtonBox::Ok)->setShortcut(Qt::CTRL | Qt::Key_Return);

        mainLayout->addWidget(buttonBox);
    }
    QCheckBox *useBackgroundColor;

    KColorButton *backgroundColor;
    KComboBox *verticalAlignment;
    TableCellFormatDialog *q;
};

TableCellFormatDialog::TableCellFormatDialog( QWidget *parent )
    : QDialog( parent ), d( new TableCellFormatDialogPrivate( this ) )
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

void TableCellFormatDialog::setTableCellBackgroundColor( const QColor &col )
{
    d->backgroundColor->setColor( col );
    d->useBackgroundColor->setChecked( true );
}

QTextCharFormat::VerticalAlignment TableCellFormatDialog::verticalAlignment() const
{
    return
            ( QTextCharFormat::VerticalAlignment )d->verticalAlignment->itemData(
                d->verticalAlignment->currentIndex () ).toInt();
}

void TableCellFormatDialog::setVerticalAlignment( QTextCharFormat::VerticalAlignment vertical )
{
    d->verticalAlignment->setCurrentIndex( d->verticalAlignment->findData( QVariant( vertical ) ) );
}

bool TableCellFormatDialog::useBackgroundColor() const
{
    return d->useBackgroundColor->isChecked();
}
