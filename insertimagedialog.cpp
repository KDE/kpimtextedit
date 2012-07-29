/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "insertimagedialog.h"
#include <KLocale>
#include <KUrlRequester>
#include <KImageIO>
#include <KFileDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <KLineEdit>

namespace KPIMTextEdit {

class InsertImageDialogPrivate
{
public:
  InsertImageDialogPrivate(InsertImageDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Insert Image") );
    q->setButtons( KDialog::Ok|KDialog::Cancel );
    q->setButtonText(KDialog::Ok,i18n("Insert"));
    QWidget *page = new QWidget(q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout(page);
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *lab = new QLabel(i18n("Image Location:"));
    imageUrlRequester = new KUrlRequester;

    QStringList mimetypes = KImageIO::mimeTypes( KImageIO::Reading );
    imageUrlRequester->fileDialog()->setFilter(mimetypes.join(QLatin1String(" ")));
    imageUrlRequester->fileDialog()->setOperationMode( KFileDialog::Other );
    imageUrlRequester->fileDialog()->setCaption( i18n( "Add Image" ) );
    imageUrlRequester->fileDialog()->okButton()->setGuiItem( KGuiItem( i18n( "&Add" ), QLatin1String( "document-open" ) ) );
    imageUrlRequester->fileDialog()->setMode( KFile::File );
    q->connect(imageUrlRequester->lineEdit(),SIGNAL(textChanged(QString)),q,SLOT(_k_slotUrlChanged(QString)));

    hbox->addWidget(lab);
    hbox->addWidget(imageUrlRequester);
    lay->addLayout(hbox);

    keepOriginalSize = new QCheckBox(i18n("Keep Original Size"));
    q->connect(keepOriginalSize,SIGNAL(clicked(bool)),q,SLOT(_k_slotKeepOriginalSizeClicked(bool)));
    keepOriginalSize->setChecked(true);
    lay->addWidget(keepOriginalSize);

    contrainsSize = new QCheckBox(i18n("Contrains Size"));
    contrainsSize->setChecked(true);
    contrainsSize->setEnabled(false);
    lay->addWidget(contrainsSize);



    hbox = new QHBoxLayout;
    lab = new QLabel(i18n("Width:"));
    width = new QSpinBox;
    width->setMinimum(1);
    width->setMaximum(99999);
    width->setEnabled(false);
    width->setSuffix(i18n(" px"));
    q->connect(width,SIGNAL(valueChanged(int)),q,SLOT(_k_slotImageWidthChanged(int)));
    hbox->addWidget(lab);
    hbox->addWidget(width);
    lay->addLayout(hbox);


    hbox = new QHBoxLayout;
    lab = new QLabel(i18n("Height:"));
    height = new QSpinBox;
    height->setMinimum(1);
    height->setMaximum(99999);
    height->setEnabled(false);
    height->setSuffix(i18n(" px"));
    q->connect(height,SIGNAL(valueChanged(int)),q,SLOT(_k_slotImageHeightChanged(int)));
    hbox->addWidget(lab);
    hbox->addWidget(height);
    lay->addLayout(hbox);

    q->enableButtonOk(false);
  }

  void _k_slotKeepOriginalSizeClicked(bool);
  void _k_slotUrlChanged(const QString&);
  void _k_slotImageWidthChanged(int);
  void _k_slotImageHeightChanged(int);


  int imageRatio;
  QCheckBox *keepOriginalSize;
  QCheckBox *contrainsSize;
  QSpinBox *width;
  QSpinBox *height;
  KUrlRequester *imageUrlRequester;
  InsertImageDialog *q;
};

void InsertImageDialogPrivate::_k_slotKeepOriginalSizeClicked(bool checked)
{
  height->setEnabled(!checked);
  width->setEnabled(!checked);
  contrainsSize->setEnabled(!checked);
}

void InsertImageDialogPrivate::_k_slotUrlChanged(const QString& text)
{
  q->enableButtonOk(!text.isEmpty());
  QImage image(text);
  if(!image.isNull()) {
    height->setValue(image.height());
    width->setValue(image.width());
  }
}

void InsertImageDialogPrivate::_k_slotImageWidthChanged(int)
{
  if(contrainsSize->isChecked()) {

  }
}

void InsertImageDialogPrivate::_k_slotImageHeightChanged(int)
{
  if(contrainsSize->isChecked()) {

  }
}


InsertImageDialog::InsertImageDialog(QWidget *parent)
  :KDialog(parent),d(new InsertImageDialogPrivate(this))
{
}


InsertImageDialog::~InsertImageDialog()
{
  delete d;
}

int InsertImageDialog::imageWidth() const
{
  return d->width->value();
}

int InsertImageDialog::imageHeight() const
{
  return d->height->value();
}

void InsertImageDialog::setImageWidth(int value)
{
  d->width->setValue(value);
}

void InsertImageDialog::setImageHeight(int value)
{
  d->height->setValue(value);
}

KUrl InsertImageDialog::imageUrl() const
{
  return d->imageUrlRequester->url();
}

void InsertImageDialog::setImageUrl(const KUrl&url)
{
  d->imageUrlRequester->setUrl(url);
}

bool InsertImageDialog::keepOriginalSize() const
{
  return d->keepOriginalSize->isChecked();
}

}

#include "insertimagedialog.moc"
