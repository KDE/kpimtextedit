/*
  Copyright (c) 2014 Montel Laurent <montel@kde.org>

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

#include "inserttablewidget.h"
#include <KLocalizedString>
#include <KComboBox>
#include <QDebug>

#include <QSpinBox>
#include <QFormLayout>

using namespace KPIMTextEdit;

class InsertTableWidget::InsertTableWidgetPrivate
{
public:
    InsertTableWidgetPrivate( InsertTableWidget *qq )
        :q( qq )
    {
        mRows = new QSpinBox;
        mRows->setMinimum( 1 );
        mRows->setValue( 2 );

        mColumns = new QSpinBox;
        mColumns->setMinimum( 1 );
        mColumns->setValue( 2 );

        mBorder = new QSpinBox;
        mBorder->setMinimum( 0 );
        mBorder->setValue( 1 );
        mBorder->setSuffix( i18n( " px" ) );

        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow( i18n( "Rows:" ), mRows );
        formLayout->addRow( i18n( "Columns:" ), mColumns );
        formLayout->addRow( i18n( "Border:" ), mBorder );

        QHBoxLayout *lay = new QHBoxLayout;
        mTypeOfLength = new KComboBox;
        q->connect( mTypeOfLength, SIGNAL(activated(int)),q,SLOT(slotTypeOfLengthChanged(int)) );
        // xgettext: no-c-format
        mTypeOfLength->addItem( i18n( "% of windows" ), QTextLength::PercentageLength );
        mTypeOfLength->addItem( i18n( "pixels" ), QTextLength::FixedLength );
        mLength = new QSpinBox;
        mLength->setMinimum( 1 );
        mLength->setMaximum( 100 );
        mLength->setValue( 100 );
        lay->addWidget( mLength );
        lay->addWidget( mTypeOfLength );

        formLayout->addRow( i18n( "Width:" ), lay );
        q->setLayout( formLayout );
    }

    QSpinBox *mColumns;
    QSpinBox *mRows;
    QSpinBox *mBorder;
    QSpinBox *mLength;
    KComboBox *mTypeOfLength;

    InsertTableWidget *q;
};

InsertTableWidget::InsertTableWidget( QWidget *parent )
    : QWidget( parent ), d( new InsertTableWidgetPrivate( this ) )
{
}

InsertTableWidget::~InsertTableWidget()
{
    delete d;
}

void InsertTableWidget::slotTypeOfLengthChanged( int index )
{
    switch ( index ) {
    case 0:
        d->mLength->setMaximum( 100 );
        d->mLength->setValue( qMin( d->mLength->value(), 100 ) );
        break;
    case 1:
        d->mLength->setMaximum( 9999 );
        break;
    default:
        qDebug() << " index not defined " << index;
        break;
    }
}

QTextLength::Type InsertTableWidget::typeOfLength() const
{
    return
            ( QTextLength::Type )d->mTypeOfLength->itemData(
                d->mTypeOfLength->currentIndex() ).toInt();
}

void InsertTableWidget::setTypeOfLength( QTextLength::Type type )
{
    const int index = d->mTypeOfLength->findData( QVariant( type ) );
    d->mTypeOfLength->setCurrentIndex( index );
    slotTypeOfLengthChanged( index );
}

int InsertTableWidget::length() const
{
    return d->mLength->value();
}

void InsertTableWidget::setLength( int val )
{
    d->mLength->setValue(val);
}

void InsertTableWidget::setColumns( int col )
{
    d->mColumns->setValue( col );
}

void InsertTableWidget::setRows( int rows )
{
    d->mRows->setValue( rows );
}

void InsertTableWidget::setBorder( int border )
{
    d->mBorder->setValue( border );
}

int InsertTableWidget::columns() const
{
    return d->mColumns->value();
}

int InsertTableWidget::rows() const
{
    return d->mRows->value();
}

int InsertTableWidget::border() const
{
    return d->mBorder->value();
}

