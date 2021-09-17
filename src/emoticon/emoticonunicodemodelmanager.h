/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>
namespace KPIMTextEdit
{
class EmoticonUnicodeModel;
class EmoticonUnicodeModelManager : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeModelManager(QObject *parent = nullptr);
    ~EmoticonUnicodeModelManager() override;

    static EmoticonUnicodeModelManager *self();
    Q_REQUIRED_RESULT EmoticonUnicodeModel *emoticonUnicodeModel() const;

private:
    EmoticonUnicodeModel *const mEmoticonUnicodeModel;
};
}
