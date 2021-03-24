/* SPDX-FileCopyrightText: 2011 Torgny Nyblom <nyblom@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#pragma once

#include <QObject>

class TextUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit TextUtilsTest(QObject *parent = nullptr);

private Q_SLOTS:
    void test_flowText_data();
    void test_flowText();
};

