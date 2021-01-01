/*
    SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TestGeneratePlainTextBuilderGui_H
#define TestGeneratePlainTextBuilderGui_H

#include <QWidget>

class TestGeneratePlainTextBuilderGui : public QWidget
{
    Q_OBJECT
public:
    explicit TestGeneratePlainTextBuilderGui(QWidget *parent = nullptr);
    ~TestGeneratePlainTextBuilderGui();
};

#endif // TestGeneratePlainTextBuilderGui_H
