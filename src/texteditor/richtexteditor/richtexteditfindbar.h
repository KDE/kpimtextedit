/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit/texteditfindbarbase.h"
#include "kpimtextedit_export.h"
#include <QTextDocument>

class QTextEdit;
namespace KPIMTextEdit
{
class RichTextEditFindBarPrivate;
/**
 * @brief The RichTextEditFindBar class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextEditFindBar : public TextEditFindBarBase
{
    Q_OBJECT
public:
    explicit RichTextEditFindBar(QTextEdit *view, QWidget *parent = nullptr);
    ~RichTextEditFindBar() override;

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
    RichTextEditFindBarPrivate *const d;
};
}

