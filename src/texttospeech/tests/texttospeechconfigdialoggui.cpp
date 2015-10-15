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

#include "texttospeech/texttospeechconfigdialog.h"
#include "texttospeech/texttospeech.h"
#include <KLocalizedString>
#include <QApplication>
#include <KAboutData>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    KAboutData aboutData(QStringLiteral("texttospeechconfigdialog_gui"), i18n("texttospeechconfigdialog_Gui"), QStringLiteral("1.0"));
    aboutData.setShortDescription(i18n("Test for config dialog text to speech"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KPIMTextEdit::TextToSpeechConfigDialog *w = new KPIMTextEdit::TextToSpeechConfigDialog;

    w->show();
    app.exec();
    delete w;
    return 0;
}
