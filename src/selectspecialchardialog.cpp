/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectspecialchardialog.h"
#include <KCharSelect>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace KPIMTextEdit
{
class SelectSpecialCharDialogPrivate
{
public:
    SelectSpecialCharDialogPrivate(SelectSpecialCharDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Select Special Characters"));

        auto lay = new QVBoxLayout(q);

        mCharSelect = new KCharSelect(q, nullptr, KCharSelect::CharacterTable | KCharSelect::BlockCombos);
        q->connect(mCharSelect, &KCharSelect::charSelected, q, &SelectSpecialCharDialog::charSelected);
        lay->addWidget(mCharSelect);

        mButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        QPushButton *okButton = mButtonBox->button(QDialogButtonBox::Ok);
        okButton->setText(i18n("Insert"));
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        lay->addWidget(mButtonBox);
        q->connect(mButtonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        q->connect(mButtonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);

        q->connect(okButton, &QPushButton::clicked, q, [this]() {
            _k_slotInsertChar();
        });
    }

    void addSelectButton()
    {
        mSelectButton = new QPushButton(i18n("Select"));
        mButtonBox->addButton(mSelectButton, QDialogButtonBox::ActionRole);
        q->connect(mSelectButton, &QPushButton::clicked, q, [this]() {
            _k_slotInsertChar();
        });
    }

    void _k_slotInsertChar();

    KCharSelect *mCharSelect = nullptr;
    QDialogButtonBox *mButtonBox = nullptr;
    QPushButton *mSelectButton = nullptr;
    SelectSpecialCharDialog *const q;
};

void SelectSpecialCharDialogPrivate::_k_slotInsertChar()
{
    Q_EMIT q->charSelected(mCharSelect->currentChar());
}

SelectSpecialCharDialog::SelectSpecialCharDialog(QWidget *parent)
    : QDialog(parent)
    , d(new SelectSpecialCharDialogPrivate(this))
{
    readConfig();
}

SelectSpecialCharDialog::~SelectSpecialCharDialog()
{
    writeConfig();
    delete d;
}

void SelectSpecialCharDialog::showSelectButton(bool show)
{
    if (show) {
        d->addSelectButton();
    } else {
        d->mButtonBox->removeButton(d->mSelectButton);
    }
}

void SelectSpecialCharDialog::setCurrentChar(QChar c)
{
    d->mCharSelect->setCurrentChar(c);
}

QChar SelectSpecialCharDialog::currentChar() const
{
    return d->mCharSelect->currentChar();
}

void SelectSpecialCharDialog::autoInsertChar()
{
    connect(d->mCharSelect, &KCharSelect::charSelected, this, &SelectSpecialCharDialog::accept);
}

void SelectSpecialCharDialog::setOkButtonText(const QString &text)
{
    d->mButtonBox->button(QDialogButtonBox::Ok)->setText(text);
}

void SelectSpecialCharDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SelectSpecialCharDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(300, 200));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SelectSpecialCharDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SelectSpecialCharDialog");
    group.writeEntry("Size", size());
}
}

#include "moc_selectspecialchardialog.cpp"
