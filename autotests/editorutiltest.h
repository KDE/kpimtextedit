/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/
#ifndef EDITORUTILTEST_H
#define EDITORUTILTEST_H

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

#endif // EDITORUTILTEST_H
