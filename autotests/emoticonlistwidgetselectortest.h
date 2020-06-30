/*
  SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef EMOTICONLISTWIDGETSELECTORTEST_H
#define EMOTICONLISTWIDGETSELECTORTEST_H

#include <QObject>

class EmoticonListWidgetSelectorTest : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonListWidgetSelectorTest(QObject *parent = nullptr);
    ~EmoticonListWidgetSelectorTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // EMOTICONLISTWIDGETSELECTORTEST_H
