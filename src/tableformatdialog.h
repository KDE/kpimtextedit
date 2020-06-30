/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef KPIMTEXTEDIT_TABLEFORMATDIALOG_H
#define KPIMTEXTEDIT_TABLEFORMATDIALOG_H

#include <QDialog>

#include <QTextLength>

namespace KPIMTextEdit {
class TableFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableFormatDialog(QWidget *parent);
    ~TableFormatDialog();

    Q_REQUIRED_RESULT int columns() const;
    Q_REQUIRED_RESULT int rows() const;
    Q_REQUIRED_RESULT int border() const;

    void setColumns(int);
    void setRows(int);
    void setBorder(int);

    Q_REQUIRED_RESULT int padding() const;
    void setPadding(int);

    Q_REQUIRED_RESULT int spacing() const;
    void setSpacing(int);

    void setAlignment(Qt::Alignment alignment);
    Q_REQUIRED_RESULT Qt::Alignment alignment() const;

    Q_REQUIRED_RESULT QTextLength::Type typeOfLength() const;
    void setTypeOfLength(QTextLength::Type type);

    Q_REQUIRED_RESULT int length() const;
    void setLength(int);

    Q_REQUIRED_RESULT QColor tableBackgroundColor() const;
    void setTableBackgroundColor(const QColor &col);

    Q_REQUIRED_RESULT bool useBackgroundColor() const;

private:
    class TableFormatDialogPrivate;
    TableFormatDialogPrivate *const d;
};
}
#endif // KPIMTEXTEDIT_TABLEFORMATDIALOG_H
