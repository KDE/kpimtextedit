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

#include "texttospeechgui.h"
#include "texttospeech/texttospeech.h"
#include <KLocalizedString>
#include <QApplication>
#include <QTextEdit>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>
#include <QStandardPaths>

TextToSpeechGui::TextToSpeechGui(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("text to speech window"));
    mEdit = new QTextEdit;
    setCentralWidget(mEdit);

    QMenu *editMenu = menuBar()->addMenu(QStringLiteral("Edit"));

    QAction *act = new QAction(i18n("Speech text"), this);
    connect(act, &QAction::triggered, this, &TextToSpeechGui::slotTextToSpeech);
    editMenu->addAction(act);
    qDebug() << " isReady ? " << KPIMTextEdit::TextToSpeech::self()->isReady();
}

TextToSpeechGui::~TextToSpeechGui()
{
}

void TextToSpeechGui::slotTextToSpeech()
{
    QString text;
    if (mEdit->textCursor().hasSelection()) {
        text = mEdit->textCursor().selectedText();
    } else {
        text = mEdit->toPlainText();
    }
    if (!text.isEmpty()) {
        KPIMTextEdit::TextToSpeech::self()->say(text);
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    TextToSpeechGui *w = new TextToSpeechGui;

    w->show();
    app.exec();
    delete w;
    return 0;
}
