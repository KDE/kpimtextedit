/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserthtmldialog.h"
#include "inserthtmleditor.h"
#include <KLocalizedString>

#include <TextCustomEditor/PlainTextEditorWidget>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myInsertHtmlDialogConfigGroupName[] = "InsertHtmlDialog";
}
namespace KPIMTextEdit
{
class InsertHtmlDialogPrivate
{
public:
    explicit InsertHtmlDialogPrivate(InsertHtmlDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Insert HTML"));
        auto lay = new QVBoxLayout(q);
        auto label = new QLabel(i18n("Insert HTML tags and texts:"));
        lay->addWidget(label);
        editor = new InsertHtmlEditor;
        editor->setSpellCheckingSupport(false);
        auto editorWidget = new TextCustomEditor::PlainTextEditorWidget(editor);
        lay->addWidget(editorWidget);
        label = new QLabel(i18n("Example: <i> Hello word </i>"));
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        label->setTextFormat(Qt::PlainText);
        lay->addWidget(label);
        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(640, 480));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myInsertHtmlDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void InsertHtmlDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myInsertHtmlDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
}

#include "moc_inserthtmldialog.cpp"
