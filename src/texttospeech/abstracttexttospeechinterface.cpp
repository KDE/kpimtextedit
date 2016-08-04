/*
   Copyright (C) 2014-2016 Laurent Montel <montel@kde.org>

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
    Q_UNUSED(text);
}

int AbstractTextToSpeechInterface::volume() const
{
    //TODO add default value ? 0 ?
    return 50;
}

void AbstractTextToSpeechInterface::setVolume(int value)
{
    Q_UNUSED(value);
}

void AbstractTextToSpeechInterface::reloadSettings()
{

}
