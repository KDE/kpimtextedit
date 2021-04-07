/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfigdialog.h"
#include "texttospeechconfigwidget.h"
#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

TextToSpeechConfigDialog::TextToSpeechConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure Text-To-Speech"));
    auto mainLayout = new QVBoxLayout(this);
    mTextToSpeechConfigWidget = new TextToSpeechConfigWidget(parent);
    mainLayout->addWidget(mTextToSpeechConfigWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TextToSpeechConfigDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TextToSpeechConfigDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &TextToSpeechConfigDialog::slotRestoreDefaults);
    mainLayout->addWidget(buttonBox);
    mTextToSpeechConfigWidget->readConfig();
    readConfig();
}

TextToSpeechConfigDialog::~TextToSpeechConfigDialog()
{
    writeConfig();
}

void TextToSpeechConfigDialog::slotRestoreDefaults()
{
    mTextToSpeechConfigWidget->restoreDefaults();
}

void TextToSpeechConfigDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "TextToSpeechConfigDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 200));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TextToSpeechConfigDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "TextToSpeechConfigDialog");
    group.writeEntry("Size", size());
}

void TextToSpeechConfigDialog::slotAccepted()
{
    mTextToSpeechConfigWidget->writeConfig();
    accept();
}
