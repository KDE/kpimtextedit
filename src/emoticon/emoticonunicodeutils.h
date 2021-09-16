/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QObject>
#include <QString>
#include <QVector>
namespace KPIMTextEdit
{
namespace EmoticonUnicodeUtils
{
struct EmoticonStruct {
    enum EmoticonType {
        Unknown = 0,
        FlagsEmoji,
        FaceEmoji,
        AnimalsEmoji,
        PlantEmoji,
        ScienceEmoji,
        ComputerEmoji,
        BookPaperEmoji,
        SoundMusicEmoji,
        WeatherEmoji,
        TimeEmoji,
        GameEmoji,
        ClothingEmoji,
        EventEmoji,
        FoodEmoji,
        TransportEmoji,
        PersonEmoji,
        PlaceEmoji,
        SymbolsEmoji,
        SportEmoji,
        EmotionEmoji,
        MoneyEmoji,
        BodyEmoji,
        MailEmoji,
        OfficeEmoji,
        ToolsEmoji,
        PhoneEmoji,
        LockEmoji,
        DrinkEmoji,
        VideoEmoji,
        HouseEmoji,
        DishwareEmoji,
        HotelEmoji,
        AwardMedalEmoji,
    };

    EmoticonStruct(const QString &name, const QString &code, EmoticonType category)
        : emoticonName(name)
        , emoticonCode(code)
        , emoticonCategory(category)
    {
    }

    QString emoticonName;
    QString emoticonCode;
    EmoticonType emoticonCategory = EmoticonType::Unknown;
};
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeFlagsEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeFaceEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeAnimalsEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodePlantEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeScienceEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeComputerEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeBookPaperEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeSoundMusicEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeWeatherEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeTimeEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeGameEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeClothingEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeEventEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeFoodEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeTransportEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodePersonEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodePlaceEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeSymbolsEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeSportEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeEmotionEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeMoneyEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeBodyEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeMailEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeOfficeEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeToolsEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodePhoneEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeLockEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeDrinkEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeVideoEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeHouseEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeDishwareEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeHotelEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> unicodeAwardMedalEmoji();
KPIMTEXTEDIT_EXPORT Q_REQUIRED_RESULT QVector<EmoticonStruct> allUnicode();
}
}

Q_DECLARE_METATYPE(KPIMTextEdit::EmoticonUnicodeUtils::EmoticonStruct::EmoticonType)
