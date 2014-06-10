/*
  Copyright (c) 2012-2013-2014 Montel Laurent <montel@kde.org>

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

#include "selectspecialchardialog.h"
#include <KCharSelect>
#include <KLocalizedString>
#include <QHBoxLayout>

namespace KPIMTextEdit {

class SelectSpecialCharDialogPrivate
{
public:
    SelectSpecialCharDialogPrivate( SelectSpecialCharDialog *qq )
        : q( qq )
    {
        q->setCaption( i18n( "Select Special Characters" ) );
        q->setButtons( KDialog::Ok|KDialog::Cancel|KDialog::User1 );
        q->setButtonText( KDialog::User1, i18n( "Select" ) );
        QWidget *page = new QWidget( q );
        q->setMainWidget( page );
        QHBoxLayout *lay = new QHBoxLayout( page );
        mCharSelect = new KCharSelect( q, 0, KCharSelect::CharacterTable|KCharSelect::BlockCombos );
        q->connect( mCharSelect, SIGNAL(charSelected(QChar)),
                    q, SIGNAL(charSelected(QChar)) );
        lay->addWidget( mCharSelect );
        q->connect( q, SIGNAL(user1Clicked()),
                    q, SLOT(_k_slotInsertChar()) );
        q->connect( q, SIGNAL(okClicked()),
                    q, SLOT(_k_slotInsertChar()) );
    }

    void _k_slotInsertChar();

    KCharSelect *mCharSelect;
    SelectSpecialCharDialog *q;
};

void SelectSpecialCharDialogPrivate::_k_slotInsertChar()
{
    Q_EMIT q->charSelected( mCharSelect->currentChar() );
}

SelectSpecialCharDialog::SelectSpecialCharDialog( QWidget *parent )
    : KDialog( parent ), d( new SelectSpecialCharDialogPrivate( this ) )
{
}

SelectSpecialCharDialog::~SelectSpecialCharDialog()
{
    delete d;
}

void SelectSpecialCharDialog::showSelectButton( bool show )
{
    if ( show ) {
        setButtons( Ok|Cancel|User1 );
    } else {
        setButtons( Ok|Cancel );
    }
}

void SelectSpecialCharDialog::setCurrentChar( const QChar &c )
{
    d->mCharSelect->setCurrentChar(c);
}

QChar SelectSpecialCharDialog::currentChar() const
{
    return d->mCharSelect->currentChar();
}

void SelectSpecialCharDialog::autoInsertChar()
{
    connect( d->mCharSelect, SIGNAL(charSelected(QChar)), SLOT(accept()) );
}

void SelectSpecialCharDialog::setOkButtonText( const QString &text )
{
    setButtonText( KDialog::User1, text );
}

}

#include "moc_selectspecialchardialog.cpp"
