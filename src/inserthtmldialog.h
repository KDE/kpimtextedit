/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include "kpimtextedit_private_export.h"
#include <QDialog>

namespace KPIMTextEdit
{
class InsertHtmlDialogPrivate;
class KPIMTEXTEDIT_TESTS_EXPORT InsertHtmlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertHtmlDialog(QWidget *parent = nullptr);
    ~InsertHtmlDialog() override;
    [[nodiscard]] QString html() const;
    void setSelectedText(const QString &str);

private:
    friend class InsertHtmlDialogPrivate;
    std::unique_ptr<InsertHtmlDialogPrivate> const d;
    void readConfig();
    void writeConfig();
};
}
