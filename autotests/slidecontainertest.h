/*
Gwenview: an image viewer
SPDX-FileCopyrightText: 2011 Aurélien Gâteau <agateau@kde.org>

SPDX-License-Identifier: GPL-2.0-or-later

*/
#ifndef SLIDECONTAINERAUTOTEST_H
#define SLIDECONTAINERAUTOTEST_H

// Local

// KDE

// Qt
#include <QObject>

class SlideContainerAutoTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInit();
    void testSlideIn();
    void testSlideOut();
    void testSlideInDeleteSlideOut();
    void testHiddenContentResize();
};

#endif /* SLIDECONTAINERAUTOTEST_H */
