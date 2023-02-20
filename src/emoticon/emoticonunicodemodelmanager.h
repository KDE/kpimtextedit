/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT const QStringList &recentIdentifier() const;
    void setRecentIdentifier(const QStringList &newRecentIdentifier);
    void addIdentifier(const QString &identifier);

Q_SIGNALS:
    void usedIdentifierChanged(const QStringList &lst);

private:
    EmoticonUnicodeModel *const mEmoticonUnicodeModel;
    QStringList mRecentIdentifier;
    void loadRecentUsed();
    void writeRecentUsed();
};
}
