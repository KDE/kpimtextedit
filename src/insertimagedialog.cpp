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

#include "insertimagedialog.h"
#include "insertimagewidget.h"

#include <KLocalizedString>

#include <QCheckBox>

namespace KPIMTextEdit {

class InsertImageDialogPrivate
{
public:
    InsertImageDialogPrivate( InsertImageDialog *qq )
        :q( qq )
    {
        q->setCaption( i18n( "Insert Image" ) );
        q->setButtons( KDialog::Ok|KDialog::Cancel );
        q->setButtonText( KDialog::Ok, i18n( "Insert" ) );
        imageWidget = new InsertImageWidget(q);
        q->connect( imageWidget, SIGNAL(enableButtonOk(bool)),
                    q, SLOT(enableButtonOk(bool)) );
        q->setMainWidget( imageWidget );
        q->enableButtonOk( false );
    }

    InsertImageWidget *imageWidget;
    InsertImageDialog *q;
};

InsertImageDialog::InsertImageDialog( QWidget *parent )
    : KDialog( parent ), d( new InsertImageDialogPrivate( this ) )
{
}

InsertImageDialog::~InsertImageDialog()
{
    delete d;
}

int InsertImageDialog::imageWidth() const
{
    return d->imageWidget->imageWidth();
}

int InsertImageDialog::imageHeight() const
{
    return d->imageWidget->imageHeight();
}

void InsertImageDialog::setImageWidth( int value )
{
    d->imageWidget->setImageWidth( value );
}

void InsertImageDialog::setImageHeight( int value )
{
    d->imageWidget->setImageHeight( value );
}

QUrl InsertImageDialog::imageUrl() const
{
    return d->imageWidget->imageUrl();
}

void InsertImageDialog::setImageUrl( const QUrl &url )
{
    d->imageWidget->setImageUrl( url );
}

bool InsertImageDialog::keepOriginalSize() const
{
    return d->imageWidget->keepOriginalSize();
}

}

