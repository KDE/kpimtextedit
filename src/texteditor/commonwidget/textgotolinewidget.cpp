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

#include "textgotolinewidget.h"

#include <KLocalizedString>
#include <QPushButton>
#include <QIcon>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>

using namespace KPIMTextEdit;

class KPIMTextEdit::TextGoToLineWidgetPrivate
{
public:
    TextGoToLineWidgetPrivate()
    {
    }

    QSpinBox *mSpinbox = nullptr;
    QPushButton *mGoToLine = nullptr;
};

TextGoToLineWidget::TextGoToLineWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::TextGoToLineWidgetPrivate)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setMargin(2);
    QToolButton *closeBtn = new QToolButton(this);
    closeBtn->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    closeBtn->setIconSize(QSize(16, 16));
    closeBtn->setToolTip(i18n("Close"));
    closeBtn->setObjectName(QStringLiteral("closebutton"));
#ifndef QT_NO_ACCESSIBILITY
    closeBtn->setAccessibleName(i18n("Close"));
#endif

    closeBtn->setAutoRaise(true);
    connect(closeBtn, &QToolButton::clicked, this, &TextGoToLineWidget::slotCloseBar);
    hbox->addWidget(closeBtn);

    QLabel *lab = new QLabel(i18n("Go to Line:"));
    hbox->addWidget(lab);
    d->mSpinbox = new QSpinBox;
    d->mSpinbox->setMinimum(1);
    d->mSpinbox->setObjectName(QStringLiteral("line"));
    connect(d->mSpinbox, &QSpinBox::editingFinished, this, &TextGoToLineWidget::slotGoToLine);
    hbox->addWidget(d->mSpinbox);
    d->mGoToLine = new QPushButton(QIcon::fromTheme(QStringLiteral("go-jump")), i18n("Go"));
    d->mGoToLine->setFlat(true);
    connect(d->mGoToLine, &QPushButton::clicked, this, &TextGoToLineWidget::slotGoToLine);
    d->mGoToLine->setObjectName(QStringLiteral("gotoline"));
    hbox->addWidget(d->mGoToLine);
    hbox->addStretch();
    d->mSpinbox->setFocus();
}

TextGoToLineWidget::~TextGoToLineWidget()
{
    // mSpinbox can emit signals from its dtor, which are connected to this object
    // so we need to make sure these connections are removed before we destroy ourselves
    delete d->mSpinbox;

    delete d;
}

void TextGoToLineWidget::setMaximumLineCount(int max)
{
    d->mSpinbox->setMaximum(max);
}

void TextGoToLineWidget::goToLine()
{
    show();
    d->mSpinbox->setFocus();
    d->mSpinbox->selectAll();
}

void TextGoToLineWidget::slotGoToLine()
{
    Q_EMIT moveToLine(d->mSpinbox->value());
}

void TextGoToLineWidget::showEvent(QShowEvent *e)
{
    if (!e->spontaneous()) {
        d->mSpinbox->setFocus();
    }
    QWidget::showEvent(e);
}

void TextGoToLineWidget::slotBlockCountChanged(int numberBlockCount)
{
    if (!isHidden()) {
        setMaximumLineCount(numberBlockCount);
    }
}

void TextGoToLineWidget::slotCloseBar()
{
    hide();
    Q_EMIT hideGotoLine();
}

bool TextGoToLineWidget::event(QEvent *e)
{
    // Close the bar when pressing Escape.
    // Not using a QShortcut for this because it could conflict with
    // window-global actions (e.g. Emil Sedgh binds Esc to "close tab").
    // With a shortcut override we can catch this before it gets to kactions.
    const bool shortCutOverride = (e->type() == QEvent::ShortcutOverride);
    if (shortCutOverride || e->type() == QEvent::KeyPress) {
        QKeyEvent *kev = static_cast<QKeyEvent * >(e);
        if (kev->key() == Qt::Key_Escape) {
            e->accept();
            slotCloseBar();
            return true;
        }
    }
    return QWidget::event(e);
}
