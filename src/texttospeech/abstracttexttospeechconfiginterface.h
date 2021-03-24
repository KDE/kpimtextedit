/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QLocale>
#include <QObject>
#include <QVector>
namespace KPIMTextEdit
{
/**
 * @brief The AbstractTextToSpeechConfigInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT AbstractTextToSpeechConfigInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechConfigInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechConfigInterface() override;
    virtual Q_REQUIRED_RESULT QVector<QLocale> availableLocales() const;
    virtual Q_REQUIRED_RESULT QLocale locale() const;
    virtual void setLocale(const QLocale &locale);

    virtual Q_REQUIRED_RESULT QStringList availableEngines() const;
    virtual Q_REQUIRED_RESULT QStringList availableVoices() const;
    virtual void setEngine(const QString &engineName);
};
}

