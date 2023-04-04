/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QList>
#include <QObject>
#include <QString>
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
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeFlagsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeFaceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeAnimalsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodePlantEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeScienceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeComputerEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeBookPaperEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeSoundMusicEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeWeatherEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeTimeEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeGameEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeClothingEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeEventEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeFoodEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeTransportEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodePersonEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodePlaceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeSymbolsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeSportEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeEmotionEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeMoneyEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeBodyEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeMailEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeOfficeEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeToolsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodePhoneEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeLockEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeDrinkEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeVideoEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeHouseEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeDishwareEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeHotelEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> unicodeAwardMedalEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QList<EmoticonStruct> allUnicode();
}
}

Q_DECLARE_METATYPE(KPIMTextEdit::EmoticonUnicodeUtils::EmoticonStruct::EmoticonType)
