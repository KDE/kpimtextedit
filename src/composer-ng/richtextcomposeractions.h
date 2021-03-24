/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

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
/**
 * @brief The RichTextComposerActions class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerActions : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerActions(KPIMTextEdit::RichTextComposerControler *controler, QObject *parent = nullptr);
    ~RichTextComposerActions() override;

    void createActions(KActionCollection *ac);
    Q_REQUIRED_RESULT int numberOfActions() const;

    Q_REQUIRED_RESULT QVector<QAction *> richTextActionList() const;

    void uncheckActionFormatPainter();
    void updateActionStates();
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);

public Q_SLOTS:
    void setActionsEnabled(bool enabled);

private:
    void slotUpdateCharFormatActions(const QTextCharFormat &format);
    void slotUpdateMiscActions();

    void setHeadingLevel(int level);
    void setListStyle(int _styleindex);
    class RichTextComposerActionsPrivate;
    RichTextComposerActionsPrivate *const d;
};
}

