/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <QDialog>

namespace KPIMTextEdit
{
class SelectSpecialCharDialogPrivate;
/**
 * @brief The SelectSpecialCharDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT SelectSpecialCharDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectSpecialCharDialog(QWidget *parent);
    ~SelectSpecialCharDialog() override;

    void setCurrentChar(QChar c);
    Q_REQUIRED_RESULT QChar currentChar() const;

    void setOkButtonText(const QString &text);

    void showSelectButton(bool show);

    /*
     * When we double click we call accept
     */
    void autoInsertChar();

Q_SIGNALS:
    void charSelected(QChar);

private:
    SelectSpecialCharDialogPrivate *const d;
    friend class SelectSpecialCharDialogPrivate;
    void readConfig();
    void writeConfig();
};
}

