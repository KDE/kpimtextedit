/*
Gwenview: an image viewer
SPDX-FileCopyrightText: 2007 Aurélien Gâteau <agateau@kde.org>

SPDX-License-Identifier: GPL-2.0-or-later

*/
#pragma once

// Qt
#include <QFrame>
#include <QPointer>

#include "kpimtextedit_export.h"

class QPropertyAnimation;

namespace KPIMTextEdit
{
/**
 * This widget is design to contain one child widget, the "content" widget.
 * It will start hidden by default. Calling slideIn() will slide in the content
 * widget from the top border. Calling slideOut() will slide it out.
 */
class KPIMTEXTEDIT_EXPORT SlideContainer : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int slideHeight READ slideHeight WRITE setSlideHeight)
public:
    explicit SlideContainer(QWidget *parent = nullptr);

    /**
     * Returns the content widget
     */
    QWidget *content() const;

    /**
     * Defines the content widget
     */
    void setContent(QWidget *content);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    Q_REQUIRED_RESULT int slideHeight() const;

    Q_INVOKABLE void setSlideHeight(int height);

public Q_SLOTS:
    /**
     * Slides the content widget in.
     * Calling it multiple times won't cause the animation to be replayed.
     */
    void slideIn();

    /**
     * Slides the content widget out.
     * Calling it multiple times won't cause the animation to be replayed.
     */
    void slideOut();

Q_SIGNALS:
    void slidedIn();
    void slidedOut();

protected:
    void resizeEvent(QResizeEvent *) override;
    bool eventFilter(QObject *, QEvent *event) override;

private Q_SLOTS:
    void slotAnimFinished();

private:
    QPointer<QWidget> mContent;
    QPointer<QPropertyAnimation> mAnim;
    bool mSlidingOut = false;

    void adjustContentGeometry();

    void animTo(int height);
};
} /* namespace */

