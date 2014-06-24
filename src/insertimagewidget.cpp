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
#include "insertimagewidget.h"

#include <KLocalizedString>
#include <KUrlRequester>
#include <KSeparator>
#include <KLineEdit>

#include <QImageReader>
#include <QFileDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

namespace KPIMTextEdit {

class InsertImageWidgetPrivate
{
public:
    InsertImageWidgetPrivate( InsertImageWidget *qq )
        : imageRatio( -1 ), q( qq )
    {
        QVBoxLayout *lay = new QVBoxLayout( q );
        QHBoxLayout *hbox = new QHBoxLayout;
        QLabel *lab = new QLabel( i18n( "Image Location:" ) );
        imageUrlRequester = new KUrlRequester;

        const QList<QByteArray> mimetypes = QImageReader::supportedMimeTypes();
        //QT5 imageUrlRequester->fileDialog()->selectMimeTypeFilter( mimetypes.join( QLatin1String( " " ) ) );
        imageUrlRequester->setWindowTitle( i18n( "Add Image" ) );
        //QT5       imageUrlRequester->fileDialog()->okButton()->setGuiItem(
        //         KGuiItem( i18n( "&Add" ), QLatin1String( "document-open" ) ) );
        imageUrlRequester->fileDialog()->setFileMode( QFileDialog::AnyFile );
        q->connect( imageUrlRequester->lineEdit(), SIGNAL(textChanged(QString)),
                    q, SLOT(_k_slotUrlChanged(QString)) );

        hbox->addWidget( lab );
        hbox->addWidget( imageUrlRequester );
        lab->setBuddy( imageUrlRequester );
        lay->addLayout( hbox );

        keepOriginalSize = new QCheckBox( i18n( "Keep Original Size" ) );
        q->connect( keepOriginalSize, SIGNAL(clicked(bool)),
                    q, SLOT(_k_slotKeepOriginalSizeClicked(bool)) );
        keepOriginalSize->setChecked( true );
        lay->addWidget( keepOriginalSize );

        keepImageRatio = new QCheckBox( i18n( "Keep Image Ratio" ) );
        keepImageRatio->setChecked( true );
        keepImageRatio->setEnabled( false );
        lay->addWidget( keepImageRatio );

        hbox = new QHBoxLayout;
        lab = new QLabel( i18n( "Width:" ) );
        width = new QSpinBox;
        width->setMinimum( 1 );
        width->setMaximum( 99999 );
        width->setEnabled( false );
        width->setSuffix( i18n( " px" ) );
        lab->setBuddy( width );
        q->connect( width, SIGNAL(valueChanged(int)),
                    q, SLOT(_k_slotImageWidthChanged(int)) );
        hbox->addWidget( lab );
        hbox->addWidget( width );
        lay->addLayout( hbox );

        hbox = new QHBoxLayout;
        lab = new QLabel( i18n( "Height:" ) );
        height = new QSpinBox;
        height->setMinimum( 1 );
        height->setMaximum( 99999 );
        height->setEnabled( false );
        height->setSuffix( i18n( " px" ) );
        lab->setBuddy( height );
        q->connect( height, SIGNAL(valueChanged(int)),
                    q, SLOT(_k_slotImageHeightChanged(int)) );
        hbox->addWidget( lab );
        hbox->addWidget( height );
        lay->addLayout( hbox );

        KSeparator *sep = new KSeparator;
        lay->addWidget( sep );

        hbox = new QHBoxLayout;
        lab = new QLabel( i18n("Image Preview:"));
        hbox->addWidget(lab);
        preview = new QLabel;
        preview->setFrameStyle(QFrame::Box);
        preview->setFixedSize(50,50);
        hbox->addWidget(preview);
        lay->addLayout( hbox );
    }

    void _k_slotKeepOriginalSizeClicked( bool );
    void _k_slotUrlChanged( const QString & );
    void _k_slotImageWidthChanged( int );
    void _k_slotImageHeightChanged( int );

    qreal imageRatio;
    QCheckBox *keepOriginalSize;
    QCheckBox *keepImageRatio;
    QSpinBox *width;
    QSpinBox *height;
    KUrlRequester *imageUrlRequester;
    QLabel *preview;
    InsertImageWidget *q;
};

void InsertImageWidgetPrivate::_k_slotKeepOriginalSizeClicked( bool checked )
{
    height->setEnabled( !checked );
    width->setEnabled( !checked );
    keepImageRatio->setEnabled( !checked );
    //Update default size
    _k_slotUrlChanged( imageUrlRequester->text() );
}

void InsertImageWidgetPrivate::_k_slotUrlChanged( const QString &text )
{
    QImage image( text );
    if ( !image.isNull() ) {
        height->setValue( image.height() );
        width->setValue( image.width() );

        imageRatio = (double)( (double)image.height() / (double)image.width() );
        preview->setPixmap(QPixmap::fromImage(image));
    } else {
        preview->clear();
        imageRatio = -1;
    }
    q->enableButtonOk( !text.isEmpty() );
}

void InsertImageWidgetPrivate::_k_slotImageWidthChanged( int value )
{
    if ( keepImageRatio->isChecked() && !keepOriginalSize->isChecked() ) {
        if ( imageRatio != -1 ) {
            height->blockSignals( true );
            height->setValue( value * imageRatio );
            height->blockSignals( false );
        }
    }
}

void InsertImageWidgetPrivate::_k_slotImageHeightChanged( int value )
{
    if ( keepImageRatio->isChecked() && !keepOriginalSize->isChecked() ) {
        if ( imageRatio != -1 ) {
            width->blockSignals( true );
            width->setValue( value / imageRatio );
            width->blockSignals( false );
        }
    }
}

InsertImageWidget::InsertImageWidget( QWidget *parent )
    : QWidget( parent ), d( new InsertImageWidgetPrivate( this ) )
{
}

InsertImageWidget::~InsertImageWidget()
{
    delete d;
}

int InsertImageWidget::imageWidth() const
{
    return d->width->value();
}

int InsertImageWidget::imageHeight() const
{
    return d->height->value();
}

void InsertImageWidget::setImageWidth( int value )
{
    d->width->setValue( value );
}

void InsertImageWidget::setImageHeight( int value )
{
    d->height->setValue( value );
}

QUrl InsertImageWidget::imageUrl() const
{
    return d->imageUrlRequester->url();
}

void InsertImageWidget::setImageUrl( const QUrl &url )
{
    d->imageUrlRequester->setUrl( url );
}

bool InsertImageWidget::keepOriginalSize() const
{
    return d->keepOriginalSize->isChecked();
}

}

#include "moc_insertimagewidget.cpp"
