/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <KActionCollection>
#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>
#include <QToolBar>

#include <../richtextcomposer.h>

#include <kpimtextedit/richtexteditorwidget.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);
    auto richtextcomposerwidget = new KPIMTextEdit::RichTextComposer;
    KActionCollection *ac = new KActionCollection(richtextcomposerwidget);
    richtextcomposerwidget->createActions(ac);
    auto editor = new KPIMTextEdit::RichTextEditorWidget(richtextcomposerwidget);
    editor->resize(800, 600);
    editor->show();

    QToolBar bar;
    for (QAction *action : richtextcomposerwidget->richTextActionList()) {
        bar.addAction(action);
    }
    bar.show();
    richtextcomposerwidget->switchToPlainText();
    richtextcomposerwidget->activateRichText();

    app.exec();
    delete editor;
    return 0;
}
