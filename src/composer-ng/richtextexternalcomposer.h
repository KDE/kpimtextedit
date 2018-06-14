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

#ifndef KPIMTEXTEDITRICHTEXTEXTERNALCOMPOSER_H
#define KPIMTEXTEDITRICHTEXTEXTERNALCOMPOSER_H

#include "kpimtextedit_export.h"
#include <QObject>
#include <QProcess>

namespace KPIMTextEdit {
class RichTextComposer;
class KPIMTEXTEDIT_EXPORT RichTextExternalComposer : public QObject
{
    Q_OBJECT
public:
    explicit RichTextExternalComposer(RichTextComposer *composer, QObject *parent = nullptr);
    ~RichTextExternalComposer();

    Q_REQUIRED_RESULT bool useExternalEditor() const;
    void setUseExternalEditor(bool value);

    void startExternalEditor();

    void setExternalEditorPath(const QString &path);
    Q_REQUIRED_RESULT QString externalEditorPath() const;
    Q_REQUIRED_RESULT bool checkExternalEditorFinished();
    void killExternalEditor();

    Q_REQUIRED_RESULT bool isInProgress() const;

Q_SIGNALS:
    void externalEditorClosed();
    void externalEditorStarted();

private:
    void slotEditorFinished(int codeError, QProcess::ExitStatus exitStatus);
    class RichTextExternalComposerPrivate;
    RichTextExternalComposerPrivate *const d;
};
}

#endif // RICHTEXTEXTERNALCOMPOSER_H
