/*
   Copyright (C) 2014-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTMESSAGEINDICATOR_H
#define TEXTMESSAGEINDICATOR_H

/**
 * @short A widget that displays messages in the top-left corner.
 *
 * This is a widget with thin border and rounded corners that displays a given
 * text along as an icon. It's meant to be used for displaying messages to the
 * user by placing this above other widgets.
 */
#include <QWidget>
class QTimer;
namespace KPIMTextEdit
{
class TextMessageIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit TextMessageIndicator(QWidget *parent = nullptr);

    enum Icon {
        None,
        Info,
        Warning,
        Error
    };

    void display(const QString &message, const QString &details = QString(), Icon icon = None, int durationMs = 4000);

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    QRect computeTextRect(const QString &message, int extra_width) const;
    void computeSizeAndResize();
    QString mMessage;
    QString mDetails;
    QTimer *mTimer;
    QPixmap mSymbol;
    int mLineSpacing;
};
}

#endif // TEXTMESSAGEINDICATOR_H

