/*
   Copyright (C) 2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef EMOTICONUNICODEUTILS_H
#define EMOTICONUNICODEUTILS_H

#include <QString>
#include <QVector>
#include "kpimtextedit_export.h"
namespace KPIMTextEdit {
namespace EmoticonUnicodeUtils
{
struct EmoticonStruct {
    EmoticonStruct(const QString &name, const QString &code)
        : emoticonName(name),
          emoticonCode(code)
    {
    }
    QString emoticonName;
    QString emoticonCode;
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
}
}

#endif // EMOTICONUNICODEUTILS_H
