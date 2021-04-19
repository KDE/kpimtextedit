/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserthtmldialog.h"
#include "inserthtmleditor.h"
#include <KLocalizedString>

#include "texteditor/plaintexteditor/plaintexteditorwidget.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace KPIMTextEdit
{
class InsertHtmlDialogPrivate
{
public:
    InsertHtmlDialogPrivate(InsertHtmlDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Insert HTML"));
        auto lay = new QVBoxLayout(q);
        QLabel *label = new QLabel(i18n("Insert HTML tags and texts:"));
        lay->addWidget(label);
        editor = new InsertHtmlEditor;
        editor->setSpellCheckingSupport(false);
        auto editorWidget = new KPIMTextEdit::PlainTextEditorWidget(editor);
        lay->addWidget(editorWidget);
        label = new QLabel(i18n("Example: <i> Hello word </i>"));
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        label->setTextFormat(Qt::PlainText);
        lay->addWidget(label);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        okButton->setText(i18nc("@action:button", "Insert"));

        q->connect(buttonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        q->connect(buttonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);

        lay->addWidget(buttonBox);
        q->connect(editor, &InsertHtmlEditor::textChanged, q, [this]() {
            _k_slotTextChanged();
        });
        okButton->setEnabled(false);
    }

    void _k_slotTextChanged();
    QPushButton *okButton = nullptr;
    InsertHtmlEditor *editor = nullptr;
    InsertHtmlDialog *const q;
};

void InsertHtmlDialogPrivate::_k_slotTextChanged()
{
    okButton->setEnabled(!editor->document()->isEmpty());
}

InsertHtmlDialog::InsertHtmlDialog(QWidget *parent)
    : QDialog(parent)
    , d(new InsertHtmlDialogPrivate(this))
{
    readConfig();
}

InsertHtmlDialog::~InsertHtmlDialog()
{
    writeConfig();
    delete d;
}

void InsertHtmlDialog::setSelectedText(const QString &str)
{
    d->editor->setPlainText(str);
}

QString InsertHtmlDialog::html() const
{
    return d->editor->toPlainText();
}

void InsertHtmlDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "InsertHtmlDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(640, 480));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void InsertHtmlDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "InsertHtmlDialog");
    group.writeEntry("Size", size());
}
}

#include "moc_inserthtmldialog.cpp"
