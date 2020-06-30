/*
Code from Gwenview
SPDX-FileCopyrightText: 2007 Aurélien Gâteau <agateau@kde.org>

SPDX-License-Identifier: GPL-2.0-or-later

*/
#include "slidecontainer.h"

// Qt
#include <QEvent>
#include <QPropertyAnimation>
#include <QResizeEvent>

using namespace KPIMTextEdit;
static const int SLIDE_DURATION = 250;

SlideContainer::SlideContainer(QWidget *parent)
    : QFrame(parent)
    , mContent(nullptr)
{
    setFixedHeight(0);
    hide();
}

QWidget *SlideContainer::content() const
{
    return mContent;
}

void SlideContainer::setContent(QWidget *content)
{
    if (mContent) {
        mContent->setParent(nullptr);
        mContent->removeEventFilter(this);
    }
    mContent = content;
    if (mContent) {
        mContent->setParent(this);
        mContent->installEventFilter(this);
        mContent->hide();
    }
}

void SlideContainer::animTo(int newHeight)
{
    if (mAnim.data()) {
        mAnim.data()->deleteLater();
        disconnect(mAnim.data(), &QPropertyAnimation::finished, this, &SlideContainer::slotAnimFinished);
    }
    QPropertyAnimation *anim = new QPropertyAnimation(this, "slideHeight", this);
    anim->setDuration(SLIDE_DURATION);
    anim->setStartValue(slideHeight());
    anim->setEndValue(newHeight);
    mAnim = anim;
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    connect(anim, &QPropertyAnimation::finished, this, &SlideContainer::slotAnimFinished);
}

void SlideContainer::slideIn()
{
    mSlidingOut = false;
    show();
    mContent->show();
    mContent->adjustSize();
    delete mAnim.data();
    if (height() == mContent->height()) {
        return;
    }
    animTo(mContent->height());
}

void SlideContainer::slideOut()
{
    if (height() == 0) {
        return;
    }
    mSlidingOut = true;
    animTo(0);
}

QSize SlideContainer::sizeHint() const
{
    if (mContent) {
        return mContent->sizeHint();
    } else {
        return QSize();
    }
}

QSize SlideContainer::minimumSizeHint() const
{
    if (mContent) {
        return mContent->minimumSizeHint();
    } else {
        return QSize();
    }
}

void SlideContainer::resizeEvent(QResizeEvent *event)
{
    if (mContent) {
        if (event->oldSize().width() != width()) {
            adjustContentGeometry();
        }
    }
}

void SlideContainer::adjustContentGeometry()
{
    if (mContent) {
        mContent->setGeometry(0, height() - mContent->height(), width(), mContent->height());
    }
}

bool SlideContainer::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        if (!mSlidingOut && height() != 0) {
            animTo(mContent->height());
        }
    }
    return false;
}

int SlideContainer::slideHeight() const
{
    return isVisible() ? height() : 0;
}

void SlideContainer::setSlideHeight(int value)
{
    setFixedHeight(value);
    adjustContentGeometry();
}

void SlideContainer::slotAnimFinished()
{
    if (height() == 0) {
        mSlidingOut = false;
        hide();
        Q_EMIT slidedOut();
    } else {
        Q_EMIT slidedIn();
    }
}
