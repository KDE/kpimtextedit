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
#include <QDialogButtonBox>
#include <QPushButton>

namespace KPIMTextEdit {

class SelectSpecialCharDialogPrivate
{
public:
    SelectSpecialCharDialogPrivate( SelectSpecialCharDialog *qq )
        : mSelectButton(0), q( qq )
    {
        q->setWindowTitle( i18n( "Select Special Characters" ) );

        QHBoxLayout *lay = new QHBoxLayout( q );

        mCharSelect = new KCharSelect( q, 0, KCharSelect::CharacterTable|KCharSelect::BlockCombos );
        q->connect( mCharSelect, SIGNAL(charSelected(QChar)),
                    q, SIGNAL(charSelected(QChar)) );
        lay->addWidget( mCharSelect );

        mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        QPushButton *okButton = mButtonBox->button(QDialogButtonBox::Ok);
        okButton->setText( i18n( "Insert" ) );
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        lay->addWidget( mButtonBox );

        q->connect( okButton, SIGNAL(okClicked()),
                    q, SLOT(_k_slotInsertChar()) );
    }

    void addSelectButton()
    {
        mSelectButton = new QPushButton(i18n( "Select" ));
        mButtonBox->addButton(mSelectButton, QDialogButtonBox::ActionRole);
        q->connect( mSelectButton, SIGNAL(clicked()),
                    q, SLOT(_k_slotInsertChar()) );
    }

    void _k_slotInsertChar();

    KCharSelect *mCharSelect;
    QDialogButtonBox *mButtonBox;
    QPushButton *mSelectButton;
    SelectSpecialCharDialog *q;
};

void SelectSpecialCharDialogPrivate::_k_slotInsertChar()
{
    Q_EMIT q->charSelected( mCharSelect->currentChar() );
}

SelectSpecialCharDialog::SelectSpecialCharDialog( QWidget *parent )
    : QDialog( parent ), d( new SelectSpecialCharDialogPrivate( this ) )
{
}

SelectSpecialCharDialog::~SelectSpecialCharDialog()
{
    delete d;
}

void SelectSpecialCharDialog::showSelectButton( bool show )
{
    if ( show ) {
        d->addSelectButton();
    } else {
        d->mButtonBox->removeButton(d->mSelectButton);
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
    d->mSelectButton->setText( text );
}

}

#include "moc_selectspecialchardialog.cpp"
