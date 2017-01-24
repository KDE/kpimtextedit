/*
   Copyright (C) 2014-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "texttospeechconfigdialog.h"
#include <KLocalizedString>
#include "texttospeechconfigwidget.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <kconfiggroup.h>
#include <KSharedConfig>

using namespace KPIMTextEdit;

TextToSpeechConfigDialog::TextToSpeechConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Configure Text-To-Speech"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mTextToSpeechConfigWidget = new TextToSpeechConfigWidget(parent);
    mainLayout->addWidget(mTextToSpeechConfigWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TextToSpeechConfigDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TextToSpeechConfigDialog::reject);
    mainLayout->addWidget(buttonBox);
    mTextToSpeechConfigWidget->readConfig();
    readConfig();
}

TextToSpeechConfigDialog::~TextToSpeechConfigDialog()
{
    writeConfig();
}

void TextToSpeechConfigDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "TextToSpeechConfigDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 200));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void TextToSpeechConfigDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "TextToSpeechConfigDialog");
    group.writeEntry("Size", size());
}

void TextToSpeechConfigDialog::slotAccepted()
{
    mTextToSpeechConfigWidget->writeConfig();
    accept();
}
