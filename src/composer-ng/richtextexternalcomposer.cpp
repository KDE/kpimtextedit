/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "richtextcomposer.h"
#include "richtextexternalcomposer.h"
#include "richtextexternalcomposer.h"

#include <KProcess>
#include <KMacroExpander>
#include <KShell>
#include <KLocalizedString>
#include <KMessageBox>
#include <QTemporaryFile>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextExternalComposer::RichTextExternalComposerPrivate
{
public:
    RichTextExternalComposerPrivate(RichTextComposer *composer)
        : richTextComposer(composer)
    {
    }

    void cannotStartProcess(const QString &commandLine);
    QString extEditorPath;
    KProcess *externalEditorProcess = nullptr;
    QTemporaryFile *extEditorTempFile = nullptr;
    RichTextComposer *richTextComposer = nullptr;
    bool useExtEditor = false;
};

RichTextExternalComposer::RichTextExternalComposer(RichTextComposer *composer, QObject *parent)
    : QObject(parent)
    , d(new RichTextExternalComposerPrivate(composer))
{
}

RichTextExternalComposer::~RichTextExternalComposer()
{
    delete d;
}

bool RichTextExternalComposer::useExternalEditor() const
{
    return d->useExtEditor;
}

void RichTextExternalComposer::setUseExternalEditor(bool value)
{
    d->useExtEditor = value;
}

void RichTextExternalComposer::setExternalEditorPath(const QString &path)
{
    d->extEditorPath = path;
}

QString RichTextExternalComposer::externalEditorPath() const
{
    return d->extEditorPath;
}

void RichTextExternalComposer::startExternalEditor()
{
    if (d->useExtEditor && !d->externalEditorProcess) {
        const QString commandLine = d->extEditorPath.trimmed();
        if (d->extEditorPath.isEmpty()) {
            setUseExternalEditor(false);
            KMessageBox::error(d->richTextComposer, i18n("Command line is empty. Please verify settings."), i18n("Empty command line"));
            return;
        }

        d->extEditorTempFile = new QTemporaryFile();
        if (!d->extEditorTempFile->open()) {
            delete d->extEditorTempFile;
            d->extEditorTempFile = nullptr;
            setUseExternalEditor(false);
            return;
        }

        d->extEditorTempFile->write(d->richTextComposer->textOrHtml().toUtf8());
        d->extEditorTempFile->close();

        d->externalEditorProcess = new KProcess();
        // construct command line...
        QHash<QChar, QString> map;
        map.insert(QLatin1Char('l'), QString::number(d->richTextComposer->textCursor().blockNumber() + 1));
        map.insert(QLatin1Char('w'), QString::number((qulonglong)d->richTextComposer->winId()));
        map.insert(QLatin1Char('f'), d->extEditorTempFile->fileName());
        const QString cmd = KMacroExpander::expandMacrosShellQuote(commandLine, map);
        const QStringList arg = KShell::splitArgs(cmd);
        bool filenameAdded = false;
        if (commandLine.contains(QStringLiteral("%f"))) {
            filenameAdded = true;
        }
        QStringList command;
        if (!arg.isEmpty()) {
            command << arg;
        }
        if (command.isEmpty()) {
            d->cannotStartProcess(commandLine);
            return;
        }

        (*d->externalEditorProcess) << command;
        if (!filenameAdded) {   // no %f in the editor command
            (*d->externalEditorProcess) << d->extEditorTempFile->fileName();
        }

        connect(d->externalEditorProcess, QOverload<int, QProcess::ExitStatus>::of(&KProcess::finished), this, &RichTextExternalComposer::slotEditorFinished);
        d->externalEditorProcess->start();
        if (!d->externalEditorProcess->waitForStarted()) {
            d->cannotStartProcess(commandLine);
        } else {
            Q_EMIT externalEditorStarted();
        }
    }
}

void RichTextExternalComposer::RichTextExternalComposerPrivate::cannotStartProcess(const QString &commandLine)
{
    KMessageBox::error(richTextComposer, i18n("External editor cannot be started. Please verify command \"%1\"", commandLine));
    richTextComposer->killExternalEditor();
    richTextComposer->setUseExternalEditor(false);
}

void RichTextExternalComposer::slotEditorFinished(int codeError, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        // the external editor could have renamed the original file and recreated a new file
        // with the given filename, so we need to reopen the file after the editor exited
        QFile localFile(d->extEditorTempFile->fileName());
        if (localFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray f = localFile.readAll();
            d->richTextComposer->setTextOrHtml(QString::fromUtf8(f.data(), f.size()));
            d->richTextComposer->document()->setModified(true);
            localFile.close();
        }
        if (codeError > 0) {
            KMessageBox::error(d->richTextComposer, i18n("Error was found when we started external editor."), i18n("External Editor Closed"));
            setUseExternalEditor(false);
        }
        Q_EMIT externalEditorClosed();
    }

    killExternalEditor();   // cleanup...
}

bool RichTextExternalComposer::checkExternalEditorFinished()
{
    if (!d->externalEditorProcess) {
        return true;
    }

    int ret = KMessageBox::warningYesNoCancel(
        d->richTextComposer,
        xi18nc("@info",
               "The external editor is still running.<nl/>"
               "Do you want to stop the editor or keep it running?<nl/>"
               "<warning>Stopping the editor will cause all your "
               "unsaved changes to be lost.</warning>"),
        i18nc("@title:window", "External Editor Running"),
        KGuiItem(i18nc("@action:button", "Stop Editor")),
        KGuiItem(i18nc("@action:button", "Keep Editor Running")));

    switch (ret) {
    case KMessageBox::Yes:
        killExternalEditor();
        return true;
    case KMessageBox::No:
        return true;
    default:
        return false;
    }
}

void RichTextExternalComposer::killExternalEditor()
{
    if (d->externalEditorProcess) {
        d->externalEditorProcess->deleteLater();
    }
    d->externalEditorProcess = nullptr;
    delete d->extEditorTempFile;
    d->extEditorTempFile = nullptr;
}

bool RichTextExternalComposer::isInProgress() const
{
    return d->externalEditorProcess;
}
