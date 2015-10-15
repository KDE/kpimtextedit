/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

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

#ifndef TEXTTOSPEECHINTERFACE_H
#define TEXTTOSPEECHINTERFACE_H

#include <QObject>
#include "texttospeechwidget.h"
#include "abstracttexttospeechinterface.h"
#include "kpimtextedit_export.h"
namespace KPIMTextEdit
{
class TextToSpeechWidget;
class KPIMTEXTEDIT_EXPORT TextToSpeechInterface : public AbstractTextToSpeechInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent = Q_NULLPTR);
    ~TextToSpeechInterface();

    bool isReady() const Q_DECL_OVERRIDE;
    void say(const QString &text) Q_DECL_OVERRIDE;
    int volume() const Q_DECL_OVERRIDE;
    void setVolume(int value) Q_DECL_OVERRIDE;
    void reloadSettings() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void stateChanged(KPIMTextEdit::TextToSpeechWidget::State state);

private:
    TextToSpeechWidget *mTextToSpeechWidget;

};
}

#endif // TEXTTOSPEECHINTERFACE_H