/**
 * klinkdialog
 *
 * SPDX-FileCopyrightText: 2008 Stephen Kelly <steveire@gmailcom>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "klinkdialog_p.h"

#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

KLinkDialog::KLinkDialog(QWidget *parent)
    : QDialog(parent)
    , textLabel(new QLabel(i18nc("@label:textbox", "Link Text:"), this))
    , textLineEdit(new QLineEdit(this))
    , linkUrlLabel(new QLabel(i18nc("@label:textbox", "Link URL:"), this))
    , linkUrlLineEdit(new QLineEdit(this))
    , buttonBox(new QDialogButtonBox(this))
{
    setWindowTitle(i18nc("@title:window", "Manage Link"));
    setModal(true);

    auto layout = new QVBoxLayout(this);

    auto grid = new QGridLayout;

    textLineEdit->setClearButtonEnabled(true);
    linkUrlLineEdit->setClearButtonEnabled(true);

    grid->addWidget(textLabel, 0, 0);
    grid->addWidget(textLineEdit, 0, 1);
    grid->addWidget(linkUrlLabel, 1, 0);
    grid->addWidget(linkUrlLineEdit, 1, 1);

    layout->addLayout(grid);

    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    textLineEdit->setFocus();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(textLineEdit, &QLineEdit::textChanged, this, &KLinkDialog::slotTextChanged);
}

KLinkDialog::~KLinkDialog() = default;

void KLinkDialog::slotTextChanged(const QString &text)
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.trimmed().isEmpty());
}

void KLinkDialog::setLinkText(const QString &linkText)
{
    textLineEdit->setText(linkText);
    if (!linkText.trimmed().isEmpty()) {
        linkUrlLineEdit->setFocus();
    }
}

void KLinkDialog::setLinkUrl(const QString &linkUrl)
{
    linkUrlLineEdit->setText(linkUrl);
}

QString KLinkDialog::linkText() const
{
    return textLineEdit->text().trimmed();
}

QString KLinkDialog::linkUrl() const
{
    return linkUrlLineEdit->text();
}

#include "moc_klinkdialog_p.cpp"
