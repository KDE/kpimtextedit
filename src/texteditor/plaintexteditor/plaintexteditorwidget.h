/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QWidget>

namespace KPIMTextEdit
{
class PlainTextEditor;
class PlainTextEditorWidgetPrivate;
/**
 * @brief The PlainTextEditorWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT PlainTextEditorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
public:
    explicit PlainTextEditorWidget(QWidget *parent = nullptr);
    explicit PlainTextEditorWidget(PlainTextEditor *customEditor, QWidget *parent = nullptr);
    ~PlainTextEditorWidget() override;

    Q_REQUIRED_RESULT PlainTextEditor *editor() const;

    void setReadOnly(bool readOnly);
    Q_REQUIRED_RESULT bool isReadOnly() const;

    void setPlainText(const QString &text);
    Q_REQUIRED_RESULT QString toPlainText() const;

    void clear();
    void setSpellCheckingConfigFileName(const QString &_fileName);

    Q_REQUIRED_RESULT bool isEmpty() const;
private Q_SLOTS:
    void slotFind();
    void slotReplace();

    void slotHideFindBar();

private:
    void init(PlainTextEditor *customEditor = nullptr);
    PlainTextEditorWidgetPrivate *const d;
};
}

