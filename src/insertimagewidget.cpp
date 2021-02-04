/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/
#include "insertimagewidget.h"

#include <KLineEdit>
#include <KLocalizedString>
#include <KSeparator>
#include <KUrlRequester>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QImageReader>
#include <QLabel>
#include <QMimeDatabase>
#include <QSpinBox>
#include <QVBoxLayout>

namespace KPIMTextEdit
{
static inline QString resolveAlias(const QString &name)
{
    QMimeDatabase db;
    const QString str = db.mimeTypeForName(name).name();
    return str;
}

class InsertImageWidgetPrivate
{
public:
    InsertImageWidgetPrivate(InsertImageWidget *qq)
        : q(qq)
    {
        auto lay = new QVBoxLayout(q);
        lay->setContentsMargins(0, 0, 0, 0);
        auto hbox = new QHBoxLayout;
        QLabel *lab = new QLabel(i18n("Image Location:"));
        imageUrlRequester = new KUrlRequester;

        QStringList lstMimeTypes;
        const QList<QByteArray> mimetypes = QImageReader::supportedMimeTypes();
        lstMimeTypes.reserve(mimetypes.count());
        for (const QByteArray &ba : mimetypes) {
            const QString resolvedAlias = resolveAlias(QString::fromUtf8(ba));
            if (!resolvedAlias.isEmpty()) {
                lstMimeTypes << resolvedAlias;
            }
        }
        imageUrlRequester->setMimeTypeFilters(lstMimeTypes);
        imageUrlRequester->setWindowTitle(i18nc("@title:window", "Add Image"));
        imageUrlRequester->setMode(KFile::File);
        q->connect(imageUrlRequester->lineEdit(), &KLineEdit::textChanged, q, [this](const QString &str) {
            _k_slotUrlChanged(str);
        });

        hbox->addWidget(lab);
        hbox->addWidget(imageUrlRequester);
        lab->setBuddy(imageUrlRequester);
        lay->addLayout(hbox);

        keepOriginalSize = new QCheckBox(i18n("Keep Original Size"));
        q->connect(keepOriginalSize, &QCheckBox::clicked, q, [this](bool b) {
            _k_slotKeepOriginalSizeClicked(b);
        });
        keepOriginalSize->setChecked(true);
        lay->addWidget(keepOriginalSize);

        keepImageRatio = new QCheckBox(i18n("Keep Image Ratio"));
        keepImageRatio->setChecked(true);
        keepImageRatio->setEnabled(false);
        lay->addWidget(keepImageRatio);

        hbox = new QHBoxLayout;
        lab = new QLabel(i18n("Width:"));
        width = new QSpinBox;
        width->setMinimum(1);
        width->setMaximum(99999);
        width->setEnabled(false);
        width->setSuffix(i18n(" px"));
        lab->setBuddy(width);
        q->connect(width, QOverload<int>::of(&QSpinBox::valueChanged), q, [this](int val) {
            _k_slotImageWidthChanged(val);
        });
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
        lab->setBuddy(height);
        q->connect(height, QOverload<int>::of(&QSpinBox::valueChanged), q, [this](int val) {
            _k_slotImageHeightChanged(val);
        });
        hbox->addWidget(lab);
        hbox->addWidget(height);
        lay->addLayout(hbox);

        KSeparator *sep = new KSeparator;
        lay->addWidget(sep);

        hbox = new QHBoxLayout;
        lab = new QLabel(i18n("Image Preview:"));
        hbox->addWidget(lab);
        preview = new QLabel;
        preview->setFrameStyle(QFrame::Box);
        preview->setFixedSize(50, 50);
        hbox->addWidget(preview);
        lay->addLayout(hbox);
    }

    void _k_slotKeepOriginalSizeClicked(bool);
    void _k_slotUrlChanged(const QString &);
    void _k_slotImageWidthChanged(int);
    void _k_slotImageHeightChanged(int);

    qreal imageRatio = -1;
    QCheckBox *keepOriginalSize = nullptr;
    QCheckBox *keepImageRatio = nullptr;
    QSpinBox *width = nullptr;
    QSpinBox *height = nullptr;
    KUrlRequester *imageUrlRequester = nullptr;
    QLabel *preview = nullptr;
    InsertImageWidget *const q;
};

void InsertImageWidgetPrivate::_k_slotKeepOriginalSizeClicked(bool checked)
{
    height->setEnabled(!checked);
    width->setEnabled(!checked);
    keepImageRatio->setEnabled(!checked);
    // Update default size
    _k_slotUrlChanged(imageUrlRequester->text());
}

void InsertImageWidgetPrivate::_k_slotUrlChanged(const QString &text)
{
    QImage image(text);
    if (!image.isNull()) {
        height->setValue(image.height());
        width->setValue(image.width());

        imageRatio = (double)((double)image.height() / (double)image.width());
        preview->setPixmap(QPixmap::fromImage(image));
    } else {
        preview->clear();
        imageRatio = -1;
    }
    Q_EMIT q->enableButtonOk(!text.trimmed().isEmpty());
}

void InsertImageWidgetPrivate::_k_slotImageWidthChanged(int value)
{
    if (keepImageRatio->isChecked() && !keepOriginalSize->isChecked()) {
        if (imageRatio != -1) {
            height->blockSignals(true);
            height->setValue(value * imageRatio);
            height->blockSignals(false);
        }
    }
}

void InsertImageWidgetPrivate::_k_slotImageHeightChanged(int value)
{
    if (keepImageRatio->isChecked() && !keepOriginalSize->isChecked()) {
        if (imageRatio != -1) {
            width->blockSignals(true);
            width->setValue(value / imageRatio);
            width->blockSignals(false);
        }
    }
}

InsertImageWidget::InsertImageWidget(QWidget *parent)
    : QWidget(parent)
    , d(new InsertImageWidgetPrivate(this))
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

void InsertImageWidget::setImageWidth(int value)
{
    d->width->setValue(value);
}

void InsertImageWidget::setImageHeight(int value)
{
    d->height->setValue(value);
}

QUrl InsertImageWidget::imageUrl() const
{
    return d->imageUrlRequester->url();
}

void InsertImageWidget::setImageUrl(const QUrl &url)
{
    d->imageUrlRequester->setUrl(url);
}

bool InsertImageWidget::keepOriginalSize() const
{
    return d->keepOriginalSize->isChecked();
}
}

#include "moc_insertimagewidget.cpp"
