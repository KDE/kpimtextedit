/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/
#pragma once

#include <QUrl>

#include <QWidget>

namespace KPIMTextEdit
{
class InsertImageWidgetPrivate;

class InsertImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InsertImageWidget(QWidget *parent);
    ~InsertImageWidget() override;

    void setImageWidth(int value);
    [[nodiscard]] int imageWidth() const;

    void setImageHeight(int value);
    [[nodiscard]] int imageHeight() const;

    [[nodiscard]] QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    [[nodiscard]] bool keepOriginalSize() const;

Q_SIGNALS:
    void enableButtonOk(bool enabled);

private:
    friend class InsertImageWidgetPrivate;
    std::unique_ptr<InsertImageWidgetPrivate> const d;
};
}
