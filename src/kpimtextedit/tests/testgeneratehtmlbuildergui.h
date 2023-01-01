/*
    SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QWidget>

class TestGenerateHTMLBuilderGui : public QWidget
{
    Q_OBJECT
public:
    explicit TestGenerateHTMLBuilderGui(QWidget *parent = nullptr);
    ~TestGenerateHTMLBuilderGui() override;
};
