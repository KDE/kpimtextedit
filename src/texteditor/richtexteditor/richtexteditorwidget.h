/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <QWidget>

namespace KPIMTextEdit
{
class RichTextEditor;
class RichTextEditorWidgetPrivate;
/**
 * @brief The RichTextEditorWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextEditorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
public:
    explicit RichTextEditorWidget(QWidget *parent = nullptr);
    explicit RichTextEditorWidget(RichTextEditor *customEditor, QWidget *parent = nullptr);
    ~RichTextEditorWidget() override;

    void clear();

    Q_REQUIRED_RESULT RichTextEditor *editor() const;

    void setReadOnly(bool readOnly);
    bool isReadOnly() const;

    void setHtml(const QString &html);
    Q_REQUIRED_RESULT QString toHtml() const;

    void setPlainText(const QString &text);
    Q_REQUIRED_RESULT QString toPlainText() const;

    void setAcceptRichText(bool b);
    bool acceptRichText() const;

    void setSpellCheckingConfigFileName(const QString &_fileName);

    Q_REQUIRED_RESULT bool isEmpty() const;
public Q_SLOTS:
    void slotFindNext();
    void slotFind();
    void slotReplace();

private:
    void slotHideFindBar();
    void init(RichTextEditor *customEditor = nullptr);
    RichTextEditorWidgetPrivate *const d;
};
}

