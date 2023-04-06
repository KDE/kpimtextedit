/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

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
Q_REQUIRED_RESULT QString emojiFontName();

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
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeFlagsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeFaceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeAnimalsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodePlantEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeScienceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeComputerEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeBookPaperEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeSoundMusicEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeWeatherEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeTimeEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeGameEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeClothingEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeEventEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeFoodEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeTransportEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodePersonEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodePlaceEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeSymbolsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeSportEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeEmotionEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeMoneyEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeBodyEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeMailEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeOfficeEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeToolsEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodePhoneEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeLockEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeDrinkEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeVideoEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeHouseEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeDishwareEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeHotelEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> unicodeAwardMedalEmoji();
Q_REQUIRED_RESULT KPIMTEXTEDIT_EXPORT QVector<EmoticonStruct> allUnicode();
}
}

Q_DECLARE_METATYPE(KPIMTextEdit::EmoticonUnicodeUtils::EmoticonStruct::EmoticonType)
