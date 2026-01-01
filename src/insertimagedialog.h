/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QDialog>
#include <QUrl>

namespace KPIMTextEdit
{
class InsertImageDialogPrivate;

class InsertImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertImageDialog(QWidget *parent = nullptr);
    ~InsertImageDialog() override;

    [[nodiscard]] int imageWidth() const;
    [[nodiscard]] int imageHeight() const;

    void setImageWidth(int value);
    void setImageHeight(int value);

    [[nodiscard]] QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    [[nodiscard]] bool keepOriginalSize() const;

private:
    friend class InsertImageDialogPrivate;
    std::unique_ptr<InsertImageDialogPrivate> const d;
};
}
