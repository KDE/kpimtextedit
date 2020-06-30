/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHConfigGUI_H
#define TEXTTOSPEECHConfigGUI_H

#include <QWidget>
class TextToSpeechConfigGui : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigGui(QWidget *parent = nullptr);
    ~TextToSpeechConfigGui();
};

#endif // TEXTTOSPEECHConfigGUI_H
