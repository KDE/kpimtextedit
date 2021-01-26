/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHLANGUAGECOMBOBOX_H
#define TEXTTOSPEECHLANGUAGECOMBOBOX_H

#include <QComboBox>
#include <QLocale>
namespace KPIMTextEdit
{
class TextToSpeechLanguageComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TextToSpeechLanguageComboBox(QWidget *parent = nullptr);
    ~TextToSpeechLanguageComboBox();
    void selectLocaleName(const QString &localeName);
    void updateAvailableLocales(const QVector<QLocale> &locales, const QLocale &current);
};
}
#endif // TEXTTOSPEECHLANGUAGECOMBOBOX_H
