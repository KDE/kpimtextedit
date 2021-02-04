/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "insertimagedialog.h"
#include "insertimagewidget.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace KPIMTextEdit
{
class InsertImageDialogPrivate
{
public:
    InsertImageDialogPrivate(InsertImageDialog *qq)
        : q(qq)
    {
        auto vbox = new QVBoxLayout(q);
        q->setWindowTitle(i18nc("@title:window", "Insert Image"));

        imageWidget = new InsertImageWidget(q);
        vbox->addWidget(imageWidget);
        q->connect(imageWidget, &InsertImageWidget::enableButtonOk, q, [this](bool b) {
            _k_slotEnabledButtonChanged(b);
        });
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setText(i18n("Insert"));
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        vbox->addWidget(buttonBox);

        q->connect(buttonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        q->connect(buttonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);
        okButton->setEnabled(false);
    }

    void _k_slotEnabledButtonChanged(bool enabled)
    {
        okButton->setEnabled(enabled);
    }

    InsertImageWidget *imageWidget = nullptr;
    QPushButton *okButton = nullptr;
    InsertImageDialog *const q;
};

InsertImageDialog::InsertImageDialog(QWidget *parent)
    : QDialog(parent)
    , d(new InsertImageDialogPrivate(this))
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

void InsertImageDialog::setImageWidth(int value)
{
    d->imageWidget->setImageWidth(value);
}

void InsertImageDialog::setImageHeight(int value)
{
    d->imageWidget->setImageHeight(value);
}

QUrl InsertImageDialog::imageUrl() const
{
    return d->imageWidget->imageUrl();
}

void InsertImageDialog::setImageUrl(const QUrl &url)
{
    d->imageWidget->setImageUrl(url);
}

bool InsertImageDialog::keepOriginalSize() const
{
    return d->imageWidget->keepOriginalSize();
}
}
#include "moc_insertimagedialog.cpp"
