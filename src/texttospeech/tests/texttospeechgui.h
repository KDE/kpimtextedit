/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHGUI_H
#define TEXTTOSPEECHGUI_H

#include <QMainWindow>
class QTextEdit;
class TextToSpeechGui : public QMainWindow
{
    Q_OBJECT
public:
    explicit TextToSpeechGui(QWidget *parent = nullptr);
    ~TextToSpeechGui();

private Q_SLOTS:
    void slotTextToSpeech();

private:
    QTextEdit *mEdit = nullptr;
};

#endif // TEXTTOSPEECHGUI_H
