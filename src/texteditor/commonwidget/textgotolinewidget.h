/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QWidget>

namespace KPIMTextEdit
{
class TextGoToLineWidgetPrivate;
/**
 * @brief The TextGoToLineWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextGoToLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextGoToLineWidget(QWidget *parent = nullptr);
    ~TextGoToLineWidget() override;

    void goToLine();

    void setMaximumLineCount(int max);
Q_SIGNALS:
    void moveToLine(int);
    void hideGotoLine();

protected:
    bool event(QEvent *e) override;
    void showEvent(QShowEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

public Q_SLOTS:
    void slotBlockCountChanged(int numberBlockCount);

private Q_SLOTS:
    void slotCloseBar();
    void slotGoToLine();

private:
    TextGoToLineWidgetPrivate *const d;
};
}
