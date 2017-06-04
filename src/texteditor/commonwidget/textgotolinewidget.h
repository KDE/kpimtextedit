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

#ifndef TEXTGOTOLINEWIDGET_H
#define TEXTGOTOLINEWIDGET_H

#include "kpimtextedit_export.h"
#include <QWidget>

namespace KPIMTextEdit {
class TextGoToLineWidgetPrivate;
class KPIMTEXTEDIT_EXPORT TextGoToLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextGoToLineWidget(QWidget *parent = nullptr);
    ~TextGoToLineWidget();

    void goToLine();

    void setMaximumLineCount(int max);
Q_SIGNALS:
    void moveToLine(int);
    void hideGotoLine();

protected:
    bool event(QEvent *e) override;
    void showEvent(QShowEvent *e) override;

public Q_SLOTS:
    void slotBlockCountChanged(int numberBlockCount);

private Q_SLOTS:
    void slotCloseBar();
    void slotGoToLine();

private:
    TextGoToLineWidgetPrivate *const d;
};
}
#endif // TEXTGOTOLINEWIDGET_H
