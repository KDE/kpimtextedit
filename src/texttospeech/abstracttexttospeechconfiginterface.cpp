/*
  Copyright (c) 2014-2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
    Q_UNUSED(locale);
}

QStringList AbstractTextToSpeechConfigInterface::availableEngines() const
{
    return QStringList();
}

QStringList AbstractTextToSpeechConfigInterface::availableVoices() const
{
    return QStringList();
}
