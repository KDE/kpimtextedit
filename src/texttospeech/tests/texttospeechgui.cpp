/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechgui.h"
#include "texttospeech/texttospeech.h"
#include <KLocalizedString>
#include <QAction>
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QStandardPaths>
#include <QTextEdit>

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

    auto w = new TextToSpeechGui;

    w->show();
    app.exec();
    delete w;
    return 0;
}
