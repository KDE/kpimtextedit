/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit/texteditfindbarbase.h"
#include "kpimtextedit_export.h"
#include <QTextDocument>

class QPlainTextEdit;
namespace KPIMTextEdit
{
class PlainTextEditFindBarPrivate;
/**
 * @brief The PlainTextEditFindBar class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT PlainTextEditFindBar : public TextEditFindBarBase
{
    Q_OBJECT

public:
    explicit PlainTextEditFindBar(QPlainTextEdit *view, QWidget *parent = nullptr);
    ~PlainTextEditFindBar() override;

protected:
    Q_REQUIRED_RESULT bool viewIsReadOnly() const override;
    Q_REQUIRED_RESULT bool documentIsEmpty() const override;
    Q_REQUIRED_RESULT bool searchInDocument(const QString &text, QTextDocument::FindFlags searchOptions) override;
    Q_REQUIRED_RESULT bool searchInDocument(const QRegularExpression &regExp, QTextDocument::FindFlags searchOptions) override;
    void autoSearchMoveCursor() override;

public Q_SLOTS:
    void slotSearchText(bool backward = false, bool isAutoSearch = true) override;

private Q_SLOTS:
    void slotReplaceText() override;
    void slotReplaceAllText() override;

private:
    PlainTextEditFindBarPrivate *const d;
};
}

