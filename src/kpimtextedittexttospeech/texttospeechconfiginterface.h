/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedittexttospeech_export.h"
#include <KPIMTextEditTextToSpeech/AbstractTextToSpeechConfigInterface>

class QTextToSpeech;
namespace KPIMTextEditTextToSpeech
{
/**
 * @brief The TextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechConfigInterface : public AbstractTextToSpeechConfigInterface
{
    Q_OBJECT
public:
    struct EngineSettings {
        double rate = 0.0;
        double pitch = 0.0;
        double volume = 0.0;
        QString voice;
        QString localeName;
    };

    explicit TextToSpeechConfigInterface(QObject *parent = nullptr);
    ~TextToSpeechConfigInterface() override;

    Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const override;
    Q_REQUIRED_RESULT QLocale locale() const override;

    Q_REQUIRED_RESULT QStringList availableEngines() const override;
    Q_REQUIRED_RESULT QStringList availableVoices() const override;
    void setEngine(const QString &engineName) override;
    QTextToSpeech *mTextToSpeech = nullptr;
    void testEngine(const EngineSettings &engineSettings);
};
}
