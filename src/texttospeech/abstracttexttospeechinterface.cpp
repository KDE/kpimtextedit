/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstracttexttospeechinterface.h"
using namespace KPIMTextEdit;
AbstractTextToSpeechInterface::AbstractTextToSpeechInterface(QObject *parent)
    : QObject(parent)
{
}

AbstractTextToSpeechInterface::~AbstractTextToSpeechInterface()
{
}

bool AbstractTextToSpeechInterface::isReady() const
{
    return true;
}

void AbstractTextToSpeechInterface::say(const QString &text)
{
    Q_UNUSED(text)
}

int AbstractTextToSpeechInterface::volume() const
{
    // TODO add default value ? 0 ?
    return 50;
}

void AbstractTextToSpeechInterface::setVolume(int value)
{
    Q_UNUSED(value)
}

void AbstractTextToSpeechInterface::reloadSettings()
{
}
