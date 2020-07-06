/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "inserthtmldialog.h"
#include "inserthtmleditor.h"
#include <KLocalizedString>

#include "texteditor/plaintexteditor/plaintexteditorwidget.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

namespace KPIMTextEdit {
class InsertHtmlDialogPrivate
{
public:
    InsertHtmlDialogPrivate(InsertHtmlDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Insert HTML"));
        QVBoxLayout *lay = new QVBoxLayout(q);
        QLabel *label = new QLabel(i18n("Insert HTML tags and texts:"));
        lay->addWidget(label);
        editor = new InsertHtmlEditor;
        editor->setSpellCheckingSupport(false);
        KPIMTextEdit::PlainTextEditorWidget *editorWidget = new KPIMTextEdit::PlainTextEditorWidget(editor);
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
        q->resize(640, 480);
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
}

InsertHtmlDialog::~InsertHtmlDialog()
{
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
}

#include "moc_inserthtmldialog.cpp"
