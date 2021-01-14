/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstracttexttospeechconfiginterface.h"

using namespace KPIMTextEdit;

AbstractTextToSpeechConfigInterface::AbstractTextToSpeechConfigInterface(QObject *parent)
    : QObject(parent)
{
}

AbstractTextToSpeechConfigInterface::~AbstractTextToSpeechConfigInterface()
{
}

QVector<QLocale> AbstractTextToSpeechConfigInterface::availableLocales() const
{
    return QVector<QLocale>();
}

QLocale AbstractTextToSpeechConfigInterface::locale() const
{
    return QLocale();
}

void AbstractTextToSpeechConfigInterface::setLocale(const QLocale &locale)
{
    Q_UNUSED(locale)
}

QStringList AbstractTextToSpeechConfigInterface::availableEngines() const
{
    return QStringList();
}

QStringList AbstractTextToSpeechConfigInterface::availableVoices() const
{
    return QStringList();
}

void AbstractTextToSpeechConfigInterface::setEngine(const QString &engineName)
{
    Q_UNUSED(engineName)
}
