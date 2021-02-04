/*
Gwenview: an image viewer
SPDX-FileCopyrightText: 2011 Aurélien Gâteau <agateau@kde.org>

SPDX-License-Identifier: GPL-2.0-or-later

*/
// Self
#include "slidecontainertest.h"

// Local
#include "kpimtextedit/slidecontainer.h"

#include <QSignalSpy>
#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

struct TestWindow : public QWidget {
    explicit TestWindow(QWidget *parent = nullptr)
        : QWidget(parent)
        , mContainer(new SlideContainer)
        , mContent(nullptr)
    {
        createContent();

        mMainWidget = new QTextEdit();
        auto layout = new QVBoxLayout(this);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(mMainWidget);
        layout->addWidget(mContainer);
    }

    void createContent()
    {
        mContent = new QTextEdit;
        mContent->setFixedSize(100, 40);
        mContainer->setContent(mContent);
    }

    SlideContainer *mContainer;
    QWidget *mMainWidget;
    QWidget *mContent;
};

void SlideContainerAutoTest::testInit()
{
    // Even with content, a SlideContainer should be invisible until slideIn()
    // is called
    TestWindow window;
    window.show();

    QTest::qWait(500);
    QCOMPARE(window.mMainWidget->height(), window.height());
}

void SlideContainerAutoTest::testSlideIn()
{
    TestWindow window;
    QSignalSpy inSpy(window.mContainer, &SlideContainer::slidedIn);
    QSignalSpy outSpy(window.mContainer, &SlideContainer::slidedOut);
    window.show();

    window.mContainer->slideIn();
    while (window.mContainer->slideHeight() != window.mContent->height()) {
        QTest::qWait(100);
    }
    QCOMPARE(window.mContainer->height(), window.mContent->height());
    QCOMPARE(inSpy.count(), 1);
    QCOMPARE(outSpy.count(), 0);
}

void SlideContainerAutoTest::testSlideOut()
{
    TestWindow window;
    window.show();

    window.mContainer->slideIn();
    while (window.mContainer->slideHeight() != window.mContent->height()) {
        QTest::qWait(100);
    }

    QSignalSpy inSpy(window.mContainer, &SlideContainer::slidedIn);
    QSignalSpy outSpy(window.mContainer, &SlideContainer::slidedOut);
    window.mContainer->slideOut();
    while (window.mContainer->slideHeight() != 0) {
        QTest::qWait(100);
    }
    QCOMPARE(window.mContainer->height(), 0);
    QCOMPARE(inSpy.count(), 0);
    QCOMPARE(outSpy.count(), 1);
}

void SlideContainerAutoTest::testSlideInDeleteSlideOut()
{
    // If content is deleted while visible, slideOut() should still produce an
    // animation
    TestWindow window;
    window.show();
    window.mContainer->slideIn();
    while (window.mContainer->slideHeight() != window.mContent->height()) {
        QTest::qWait(100);
    }
    window.mContent->deleteLater();
    window.mContainer->slideOut();
    while (window.mContainer->slideHeight() != 0) {
        QTest::qWait(100);
    }
    QCOMPARE(window.mContainer->height(), 0);
}

void SlideContainerAutoTest::testHiddenContentResize()
{
    // Resizing content should not trigger a slide if it is not visible.
    TestWindow window;
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    window.mContent->show();
    window.mContent->setFixedSize(150, 80);
    QTest::qWait(500);
    QCOMPARE(window.mContainer->height(), 0);
}

QTEST_MAIN(SlideContainerAutoTest)
