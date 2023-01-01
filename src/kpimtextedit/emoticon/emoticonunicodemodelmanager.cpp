/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodemodelmanager.h"
#include "emoticonunicodemodel.h"

#include <KConfigGroup>
#include <KSharedConfig>
using namespace KPIMTextEdit;
namespace
{
static const char myEmoticonRecentUsedGroupName[] = "EmoticonRecentUsed";
}
EmoticonUnicodeModelManager::EmoticonUnicodeModelManager(QObject *parent)
    : QObject(parent)
    , mEmoticonUnicodeModel(new EmoticonUnicodeModel(this))
{
    mEmoticonUnicodeModel->setEmoticonList(EmoticonUnicodeUtils::allUnicode());
    loadRecentUsed();
}

EmoticonUnicodeModelManager::~EmoticonUnicodeModelManager()
{
    writeRecentUsed();
}

EmoticonUnicodeModelManager *EmoticonUnicodeModelManager::self()
{
    static EmoticonUnicodeModelManager s_self;
    return &s_self;
}

EmoticonUnicodeModel *EmoticonUnicodeModelManager::emoticonUnicodeModel() const
{
    return mEmoticonUnicodeModel;
}

const QStringList &EmoticonUnicodeModelManager::recentIdentifier() const
{
    return mRecentIdentifier;
}

void EmoticonUnicodeModelManager::setRecentIdentifier(const QStringList &newRecentIdentifier)
{
    mRecentIdentifier = newRecentIdentifier;
    Q_EMIT usedIdentifierChanged(mRecentIdentifier);
}

void EmoticonUnicodeModelManager::addIdentifier(const QString &identifier)
{
    if (int i = mRecentIdentifier.indexOf(identifier)) {
        // Remove it for adding in top of list
        if (i != -1) {
            mRecentIdentifier.removeAt(i);
        }
    }
    mRecentIdentifier.prepend(identifier);
    writeRecentUsed();
    Q_EMIT usedIdentifierChanged(mRecentIdentifier);
}

void EmoticonUnicodeModelManager::loadRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
    mRecentIdentifier = group.readEntry("Recents", QStringList());
}

void EmoticonUnicodeModelManager::writeRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedGroupName);
    group.writeEntry("Recents", mRecentIdentifier);
    group.sync();
}