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
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>

namespace KPIMTextEdit {

class InsertImageDialogPrivate
{
public:
    InsertImageDialogPrivate( InsertImageDialog *qq )
        :q( qq )
    {
        QVBoxLayout *vbox = new QVBoxLayout;
        q->setLayout(vbox);
        q->setWindowTitle( i18n( "Insert Image" ) );

        imageWidget = new InsertImageWidget(q);
        vbox->addWidget(imageWidget);
        q->connect( imageWidget, SIGNAL(enableButtonOk(bool)),
                    q, SLOT(_k_slotEnabledButtonChanged(bool)) );
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setText( i18n( "Insert" ) );
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        vbox->addWidget(buttonBox);

        q->connect(buttonBox, SIGNAL(accepted()), q, SLOT(accept()));
        q->connect(buttonBox, SIGNAL(rejected()), q, SLOT(reject()));
        okButton->setEnabled( false );
    }

    void _k_slotEnabledButtonChanged(bool enabled)
    {
        okButton->setEnabled(enabled);
    }

    InsertImageWidget *imageWidget;
    QPushButton *okButton;
    InsertImageDialog *q;
};

InsertImageDialog::InsertImageDialog( QWidget *parent )
    : QDialog( parent ), d( new InsertImageDialogPrivate( this ) )
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
#include "moc_insertimagedialog.cpp"
