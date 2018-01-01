/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "texttospeechlanguagecombobox.h"
using namespace KPIMTextEdit;

TextToSpeechLanguageComboBox::TextToSpeechLanguageComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

TextToSpeechLanguageComboBox::~TextToSpeechLanguageComboBox()
{
}

void TextToSpeechLanguageComboBox::selectLocaleName(const QString &localeName)
{
    const int countItem(count());
    for (int i = 0; i < countItem; ++i) {
        if (itemData(i).toLocale().name() == localeName) {
            setCurrentIndex(i);
            break;
        }
    }
}

void TextToSpeechLanguageComboBox::updateAvailableLocales(const QVector<QLocale> &locales, const QLocale &current)
{
    clear();
    for (const QLocale &locale : locales) {
        QVariant localeVariant(locale);
        addItem(QLocale::languageToString(locale.language()), localeVariant);
        if (locale.name() == current.name()) {
            setCurrentIndex(count() - 1);
        }
    }
}
