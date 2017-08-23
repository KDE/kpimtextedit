/*
   Copyright (C) 2014-2017 Laurent Montel <montel@kde.org>

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

#ifndef TEXTTOSPEECHCONFIGWIDGET_H
#define TEXTTOSPEECHCONFIGWIDGET_H
#include <QWidget>
#include "kpimtextedit_private_export.h"
class QSlider;
class QComboBox;
namespace KPIMTextEdit {
class TextToSpeechLanguageComboBox;
class AbstractTextToSpeechConfigInterface;
class KPIMTEXTEDIT_TESTS_EXPORT TextToSpeechConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigWidget(QWidget *parent = nullptr);
    ~TextToSpeechConfigWidget();

    void writeConfig();
    void readConfig();

    void setTextToSpeechConfigInterface(AbstractTextToSpeechConfigInterface *interface);
    void restoreDefaults();

Q_SIGNALS:
    void configChanged(bool state);

private:
    void valueChanged();
    void slotUpdateSettings();
    void slotEngineChanged();
    void slotLanguageChanged();
    void updateVoice();
    void updateLocale();
    void updateEngine();
    void updateAvailableLocales();
    void updateAvailableEngine();
    void updateAvailableVoices();
    QSlider *mVolume = nullptr;
    QSlider *mRate = nullptr;
    QSlider *mPitch = nullptr;
    TextToSpeechLanguageComboBox *mLanguage = nullptr;
    AbstractTextToSpeechConfigInterface *mAbstractTextToSpeechConfigInterface = nullptr;
    QComboBox *mAvailableEngine = nullptr;
    QComboBox *mVoice = nullptr;
};
}

#endif // TEXTTOSPEECHCONFIGWIDGET_H
