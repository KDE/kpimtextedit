/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef KPIMTEXTEDIT_INSERTIMAGEDIALOG_H
#define KPIMTEXTEDIT_INSERTIMAGEDIALOG_H

#include <QDialog>
#include <QUrl>

namespace KPIMTextEdit {
class InsertImageDialogPrivate;

class InsertImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertImageDialog(QWidget *parent = nullptr);
    ~InsertImageDialog();

    Q_REQUIRED_RESULT int imageWidth() const;
    Q_REQUIRED_RESULT int imageHeight() const;

    void setImageWidth(int value);
    void setImageHeight(int value);

    Q_REQUIRED_RESULT QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    Q_REQUIRED_RESULT bool keepOriginalSize() const;

private:
    friend class InsertImageDialogPrivate;
    InsertImageDialogPrivate *const d;
};
}

#endif // KPIMTEXTEDIT_INSERTIMAGEDIALOG_H
