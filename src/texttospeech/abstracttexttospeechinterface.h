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

#ifndef ABSTRACTTEXTTOSPEECHINTERFACE_H
#define ABSTRACTTEXTTOSPEECHINTERFACE_H
#include "kpimtextedit_export.h"
#include <QObject>
#include <QVector>
#include <QLocale>
namespace KPIMTextEdit {
class KPIMTEXTEDIT_EXPORT AbstractTextToSpeechInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechInterface();

    virtual bool isReady() const;
    virtual void say(const QString &text);
    virtual int volume() const;
    virtual void setVolume(int value);
    virtual void reloadSettings();
};
}

#endif // ABSTRACTTEXTTOSPEECHINTERFACE_H
