/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedit_private_export.h"
#include <QWidget>
class QSlider;
class QComboBox;
namespace KPIMTextEdit
{
class TextToSpeechLanguageComboBox;
class AbstractTextToSpeechConfigInterface;
class KPIMTEXTEDIT_TESTS_EXPORT TextToSpeechConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigWidget(QWidget *parent = nullptr);
    ~TextToSpeechConfigWidget() override;

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

