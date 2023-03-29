/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

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

private:
    KPIMTEXTEDIT_NO_EXPORT void slotFind();
    KPIMTEXTEDIT_NO_EXPORT void slotReplace();

    KPIMTEXTEDIT_NO_EXPORT void slotHideFindBar();
    KPIMTEXTEDIT_NO_EXPORT void init(PlainTextEditor *customEditor = nullptr);
    std::unique_ptr<PlainTextEditorWidgetPrivate> const d;
};
}
