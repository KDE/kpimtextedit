/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonunicodemodelmanager.h"
#include "emoticonunicodemodel.h"
using namespace KPIMTextEdit;
EmoticonUnicodeModelManager::EmoticonUnicodeModelManager(QObject *parent)
    : QObject(parent)
    , mEmoticonUnicodeModel(new EmoticonUnicodeModel(this))
{
    mEmoticonUnicodeModel->setEmoticonList(EmoticonUnicodeUtils::allUnicode());
}

EmoticonUnicodeModelManager::~EmoticonUnicodeModelManager()
{
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
