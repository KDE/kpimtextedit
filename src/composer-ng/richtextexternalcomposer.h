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
     */
    explicit RichTextExternalComposer(RichTextComposer *composer, QObject *parent = nullptr);
    /*!
     */
    ~RichTextExternalComposer() override;

    /*!
     */
    [[nodiscard]] bool useExternalEditor() const;
    /*!
     */
    void setUseExternalEditor(bool value);

    /*!
     */
    void startExternalEditor();

    /*!
     */
    void setExternalEditorPath(const QString &path);
    /*!
     */
    [[nodiscard]] QString externalEditorPath() const;
    /*!
     */
    [[nodiscard]] bool checkExternalEditorFinished();
    /*!
     */
    void killExternalEditor();

    /*!
     */
    [[nodiscard]] bool isInProgress() const;

Q_SIGNALS:
    /*!
     */
    void externalEditorClosed();
    /*!
     */
    void externalEditorStarted();

private:
    KPIMTEXTEDIT_NO_EXPORT void slotEditorFinished(int codeError, QProcess::ExitStatus exitStatus);
    class RichTextExternalComposerPrivate;
    std::unique_ptr<RichTextExternalComposerPrivate> const d;
};
}
