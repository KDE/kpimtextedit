/*
   Copyright (C) 2014-2019 Laurent Montel <montel@kde.org>

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

class QTextToSpeech;
namespace KPIMTextEdit {
/**
 * @brief The TextToSpeechConfigInterface class
 */
class KPIMTEXTEDIT_EXPORT TextToSpeechConfigInterface : public AbstractTextToSpeechConfigInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigInterface(QObject *parent = nullptr);
    ~TextToSpeechConfigInterface() override;

    QVector<QLocale> availableLocales() const override;
    QLocale locale() const override;

    QStringList availableEngines() const override;
    QStringList availableVoices() const override;
    void setEngine(const QString &engineName) override;
    QTextToSpeech *mTextToSpeech = nullptr;
};
}

#endif // TEXTTOSPEECHCONFIGINTERFACE_H
