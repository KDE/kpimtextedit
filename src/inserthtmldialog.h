/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QDialog>

#include "kpimtextedit/plaintexteditor.h"
#include "kpimtextedit_private_export.h"

namespace KPIMTextEdit
{
class InsertHtmlDialogPrivate;
class KPIMTEXTEDIT_TESTS_EXPORT InsertHtmlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertHtmlDialog(QWidget *parent = nullptr);
    ~InsertHtmlDialog() override;
    Q_REQUIRED_RESULT QString html() const;
    void setSelectedText(const QString &str);

private:
    friend class InsertHtmlDialogPrivate;
    InsertHtmlDialogPrivate *const d;
    void readConfig();
    void writeConfig();
};
}

