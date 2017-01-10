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

#ifndef TEXTTOSPEECHCONFIGINTERFACE_H
#define TEXTTOSPEECHCONFIGINTERFACE_H
#include "abstracttexttospeechconfiginterface.h"
#include "kpimtextedit_export.h"
#include "config-kpimtextedit.h"

#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
class QTextToSpeech;
#endif
namespace KPIMTextEdit
{
class KPIMTEXTEDIT_EXPORT TextToSpeechConfigInterface : public AbstractTextToSpeechConfigInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigInterface(QObject *parent = nullptr);
    ~TextToSpeechConfigInterface();

    QVector<QLocale> availableLocales() const Q_DECL_OVERRIDE;
    QLocale locale() const Q_DECL_OVERRIDE;

    QStringList availableEngines() const Q_DECL_OVERRIDE;
    QStringList availableVoices() const Q_DECL_OVERRIDE;
    void setEngine(const QString &engineName) Q_DECL_OVERRIDE;
#if KPIMTEXTEDIT_HAVE_TEXTTOSPEECH
    QTextToSpeech *mTextToSpeech;
#endif
};
}

#endif // TEXTTOSPEECHCONFIGINTERFACE_H

