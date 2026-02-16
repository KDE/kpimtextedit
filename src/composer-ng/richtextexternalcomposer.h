/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QObject>
#include <QProcess>
#include <memory>

namespace KPIMTextEdit
{
class RichTextComposer;
/*!
 * \class KPIMTextEdit::RichTextExternalComposer
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextExternalComposer
 *
 * \brief The RichTextExternalComposer class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextExternalComposer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextExternalComposer with the given \a composer and \a parent.
     */
    explicit RichTextExternalComposer(RichTextComposer *composer, QObject *parent = nullptr);
    /*!
     * Destroys the RichTextExternalComposer.
     */
    ~RichTextExternalComposer() override;

    /*!
     * Returns true if the external editor is enabled.
     */
    [[nodiscard]] bool useExternalEditor() const;
    /*!
     * Enables or disables the use of an external editor based on the \a value parameter.
     */
    void setUseExternalEditor(bool value);

    /*!
     * Starts the external editor process.
     */
    void startExternalEditor();

    /*!
     * Sets the path to the external editor executable.
     */
    void setExternalEditorPath(const QString &path);
    /*!
     * Returns the path to the external editor executable.
     */
    [[nodiscard]] QString externalEditorPath() const;
    /*!
     * Checks if the external editor process has finished.
     * Returns true if the editor has finished, false otherwise.
     */
    [[nodiscard]] bool checkExternalEditorFinished();
    /*!
     * Terminates the external editor process.
     */
    void killExternalEditor();

    /*!
     * Returns true if the external editor is currently running.
     */
    [[nodiscard]] bool isInProgress() const;

Q_SIGNALS:
    /*!
     * Emitted when the external editor is closed.
     */
    void externalEditorClosed();
    /*!
     * Emitted when the external editor is started.
     */
    void externalEditorStarted();

private:
    KPIMTEXTEDIT_NO_EXPORT void slotEditorFinished(int codeError, QProcess::ExitStatus exitStatus);
    class RichTextExternalComposerPrivate;
    std::unique_ptr<RichTextExternalComposerPrivate> const d;
};
}
