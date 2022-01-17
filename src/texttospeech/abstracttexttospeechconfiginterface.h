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
    Q_REQUIRED_RESULT virtual QVector<QLocale> availableLocales() const;
    Q_REQUIRED_RESULT virtual QLocale locale() const;
    virtual void setLocale(const QLocale &locale);

    Q_REQUIRED_RESULT virtual QStringList availableEngines() const;
    Q_REQUIRED_RESULT virtual QStringList availableVoices() const;
    virtual void setEngine(const QString &engineName);
};
}

