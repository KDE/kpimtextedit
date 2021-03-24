/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QObject>
#include <QProcess>

namespace KPIMTextEdit
{
class RichTextComposer;
/**
 * @brief The RichTextExternalComposer class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextExternalComposer : public QObject
{
    Q_OBJECT
public:
    explicit RichTextExternalComposer(RichTextComposer *composer, QObject *parent = nullptr);
    ~RichTextExternalComposer() override;

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

