/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "abstracttexttospeechconfiginterface.h"
#include "kpimtextedit_export.h"

class QTextToSpeech;
namespace KPIMTextEdit
{
/**
 * @brief The TextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT TextToSpeechConfigInterface : public AbstractTextToSpeechConfigInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigInterface(QObject *parent = nullptr);
    ~TextToSpeechConfigInterface() override;

    Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const override;
    Q_REQUIRED_RESULT QLocale locale() const override;

    Q_REQUIRED_RESULT QStringList availableEngines() const override;
    Q_REQUIRED_RESULT QStringList availableVoices() const override;
    void setEngine(const QString &engineName) override;
    QTextToSpeech *mTextToSpeech = nullptr;
};
}

