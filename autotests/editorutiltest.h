/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class EditorUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit EditorUtilTest(QObject *parent = nullptr);
    ~EditorUtilTest();
private Q_SLOTS:
    void testUpperCase();
    void testUpperCase_data();
    void testLowerCase();
    void testLowerCase_data();
    void testSentenceCase();
    void testSentenceCase_data();
    void testReverseCase();
    void testReverseCase_data();
};

