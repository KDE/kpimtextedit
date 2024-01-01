/*
    SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QWidget>

class TestGeneratePlainTextBuilderGui : public QWidget
{
    Q_OBJECT
public:
    explicit TestGeneratePlainTextBuilderGui(QWidget *parent = nullptr);
    ~TestGeneratePlainTextBuilderGui() override;
};
