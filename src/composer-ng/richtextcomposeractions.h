/*
   Copyright (C) 2015-2017 Laurent Montel <montel@kde.org>

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

#ifndef KPIMTEXTEDITRICHTEXTCOMPOSERACTIONS_H
#define KPIMTEXTEDITRICHTEXTCOMPOSERACTIONS_H

#include <QObject>
#include "kpimtextedit_export.h"
#include "richtextcomposer.h"
class KActionCollection;
class QTextCharFormat;
class QAction;
namespace KPIMTextEdit {
class RichTextComposerControler;
class KPIMTEXTEDIT_EXPORT RichTextComposerActions : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerActions(KPIMTextEdit::RichTextComposerControler *controler, QObject *parent = nullptr);
    ~RichTextComposerActions();

    void createActions(KActionCollection *ac);
    int numberOfActions() const;

    QList<QAction *> richTextActionList() const;

    void uncheckActionFormatPainter();
    void updateActionStates();
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);

public Q_SLOTS:
    void setActionsEnabled(bool enabled);

private:
    void slotUpdateCharFormatActions(const QTextCharFormat &format);
    void slotUpdateMiscActions();

    void setListStyle(int _styleindex);
    class RichTextComposerActionsPrivate;
    RichTextComposerActionsPrivate *const d;
};
}

#endif // RICHTEXTCOMPOSERACTIONS_H
