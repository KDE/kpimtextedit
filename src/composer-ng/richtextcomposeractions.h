/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include "richtextcomposer.h"
#include <QObject>
class KActionCollection;
class QTextCharFormat;
class QAction;
namespace KPIMTextEdit
{
class RichTextComposerControler;
/*!
 * \class KPIMTextEdit::RichTextComposerActions
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerActions
 *
 * \brief The RichTextComposerActions class manages formatting actions for rich text composition.
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerActions : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposerActions with the given \a controller and \a parent.
     */
    explicit RichTextComposerActions(KPIMTextEdit::RichTextComposerControler *controller, QObject *parent = nullptr);
    /*!
     * Destroys the RichTextComposerActions.
     */
    ~RichTextComposerActions() override;

    /*!
     * Creates actions and adds them to the given \a ac action collection.
     */
    void createActions(KActionCollection *ac);
    /*!
     * Returns the number of rich text actions.
     */
    [[nodiscard]] int numberOfActions() const;

    /*!
     * Returns the list of rich text actions.
     */
    [[nodiscard]] QList<QAction *> richTextActionList() const;

    /*!
     * Unchecks the format painter action.
     */
    void uncheckActionFormatPainter();
    /*!
     * Updates the state of all actions based on the current text format.
     */
    void updateActionStates();
    /*!
     * Updates actions for the given text \a mode change.
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);

public Q_SLOTS:
    /*!
     * Enables or disables all actions based on the \a enabled parameter.
     */
    void setActionsEnabled(bool enabled);

private:
    KPIMTEXTEDIT_NO_EXPORT void slotUpdateCharFormatActions(const QTextCharFormat &format);
    KPIMTEXTEDIT_NO_EXPORT void slotUpdateMiscActions();

    KPIMTEXTEDIT_NO_EXPORT void setHeadingLevel(int level);
    KPIMTEXTEDIT_NO_EXPORT void setListStyle(int _styleindex);
    class RichTextComposerActionsPrivate;
    std::unique_ptr<RichTextComposerActionsPrivate> const d;
};
}
