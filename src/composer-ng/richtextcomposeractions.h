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
 * \inheaderfile KPIMTextEdit/RichTextComposerActions
 *
 * \brief The RichTextComposerActions class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerActions : public QObject
{
    Q_OBJECT
public:
    /*!
     */
    explicit RichTextComposerActions(KPIMTextEdit::RichTextComposerControler *controller, QObject *parent = nullptr);
    /*!
     */
    ~RichTextComposerActions() override;

    /*!
     */
    void createActions(KActionCollection *ac);
    /*!
     */
    [[nodiscard]] int numberOfActions() const;

    /*!
     */
    [[nodiscard]] QList<QAction *> richTextActionList() const;

    /*!
     */
    void uncheckActionFormatPainter();
    /*!
     */
    void updateActionStates();
    /*!
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);

public Q_SLOTS:
    /*!
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
