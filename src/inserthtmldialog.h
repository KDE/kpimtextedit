/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef RICHTEXTCOMPOSER_INSERTHTMLDIALOG_H
#define RICHTEXTCOMPOSER_INSERTHTMLDIALOG_H

#include <QDialog>

#include "kpimtextedit_private_export.h"
#include "kpimtextedit/plaintexteditor.h"

namespace KPIMTextEdit {
class InsertHtmlDialogPrivate;
class KPIMTEXTEDIT_TESTS_EXPORT InsertHtmlDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertHtmlDialog(QWidget *parent = nullptr);
    ~InsertHtmlDialog();
    Q_REQUIRED_RESULT QString html() const;
    void setSelectedText(const QString &str);

private:
    friend class InsertHtmlDialogPrivate;
    InsertHtmlDialogPrivate *const d;
};
}

#endif
