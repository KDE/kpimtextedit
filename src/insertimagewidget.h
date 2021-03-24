/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT int imageWidth() const;

    void setImageHeight(int value);
    Q_REQUIRED_RESULT int imageHeight() const;

    Q_REQUIRED_RESULT QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    Q_REQUIRED_RESULT bool keepOriginalSize() const;

Q_SIGNALS:
    void enableButtonOk(bool enabled);

private:
    friend class InsertImageWidgetPrivate;
    InsertImageWidgetPrivate *const d;
};
}

