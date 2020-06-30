/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/
#ifndef KPIMTEXTEDIT_INSERTIMAGEWIDGET_H
#define KPIMTEXTEDIT_INSERTIMAGEWIDGET_H

#include <QUrl>

#include <QWidget>

namespace KPIMTextEdit {
class InsertImageWidgetPrivate;

class InsertImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InsertImageWidget(QWidget *parent);
    ~InsertImageWidget();

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

#endif // KPIMTEXTEDIT_INSERTIMAGEWIDGET_H
