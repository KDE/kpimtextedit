/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unicodeemoticonmanager.h"
#include "kpimtextedit_debug.h"
#include "unicodeemoticonparser.h"
#include <KLocalizedString>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
using namespace KPIMTextEdit;
UnicodeEmoticonManager::UnicodeEmoticonManager(QObject *parent)
    : QObject(parent)
{
    loadUnicodeEmoji();
}

UnicodeEmoticonManager::~UnicodeEmoticonManager() = default;

UnicodeEmoticonManager *UnicodeEmoticonManager::self()
{
    static UnicodeEmoticonManager s_self;
    return &s_self;
}

void UnicodeEmoticonManager::loadUnicodeEmoji()
{
    UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qCWarning(KPIMTEXTEDIT_LOG) << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    const QJsonObject obj = doc.object();
    mUnicodeEmojiList = unicodeParser.parse(obj);
}

QList<UnicodeEmoticon> UnicodeEmoticonManager::unicodeEmojiList() const
{
    return mUnicodeEmojiList;
}

int UnicodeEmoticonManager::count() const
{
    return mUnicodeEmojiList.count();
}

QString UnicodeEmoticonManager::i18nUnicodeCategory(const QString &name) const
{
    // Name "🚗"Category "travel", Name "🇿"Category "regional", Name "🏳️"Category "flags")
    if (name == QLatin1String("symbols")) {
        return i18n("Symbols");
    } else if (name == QLatin1String("activity")) {
        return i18n("Activity");
    } else if (name == QLatin1String("objects")) {
        return i18n("Objects");
    } else if (name == QLatin1String("nature")) {
        return i18n("Nature");
    } else if (name == QLatin1String("food")) {
        return i18n("Food");
    } else if (name == QLatin1String("people")) {
        return i18n("People");
    } else if (name == QLatin1String("travel")) {
        return i18n("Travel");
    } else if (name == QLatin1String("regional")) {
        return i18n("Regional");
    } else if (name == QLatin1String("flags")) {
        return i18n("Flags");
    } else {
        qCWarning(KPIMTEXTEDIT_LOG) << "Missing i18n translate " << name;
    }
    return {};
}

QList<EmoticonCategory> UnicodeEmoticonManager::categories() const
{
    QList<EmoticonCategory> categories;
    QSet<QString> seen;
    for (const UnicodeEmoticon &emo : std::as_const(mUnicodeEmojiList)) {
        // Pick the first icon in each category
        const QString category = emo.category();
        if (!seen.contains(category)) {
            seen.insert(category);
            if (category == QLatin1String("modifier")) { // pointless icons
                continue;
            }
            EmoticonCategory cat;
            cat.setCategory(category);
            cat.setName(emo.unicode());
            cat.setI18nName(i18nUnicodeCategory(category));
            cat.setOrder(UnicodeEmoticonParser::changeOrder(category));
            categories.append(std::move(cat));
        }
    }
    std::sort(categories.begin(), categories.end());
    return categories;
}

QList<UnicodeEmoticon> UnicodeEmoticonManager::emojisForCategory(const QString &category) const
{
    QList<UnicodeEmoticon> result;

    auto hasRequestedCategory = [category](const UnicodeEmoticon &emo) {
        return emo.category() == category;
    };
    std::copy_if(mUnicodeEmojiList.begin(), mUnicodeEmojiList.end(), std::back_inserter(result), hasRequestedCategory);
    return result;
}

UnicodeEmoticon UnicodeEmoticonManager::unicodeEmoticonForEmoji(const QString &emojiIdentifier) const
{
    for (const UnicodeEmoticon &emo : mUnicodeEmojiList) {
        if (emo.hasEmoji(emojiIdentifier)) {
            return emo;
        }
    }
    return {};
}
