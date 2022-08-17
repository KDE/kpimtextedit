# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextEditorWidget
    INCLUDE_FILE "KPIMTextEdit/RichTextEditorWidget"
    TOOLTIP "A RichTextEditor (KDE-PIM)"
    WHATSTHIS "A RichTextEditor."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::PlainTextEditorWidget
    INCLUDE_FILE "KPIMTextEdit/PlainTextEditorWidget"
    TOOLTIP "A PlainTextEditor (KDE-PIM)"
    WHATSTHIS "A PlainTextEditor."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposer
    INCLUDE_FILE "KPIMTextEdit/RichTextComposer"
    TOOLTIP "A RichText Composer (KDE-PIM)"
    WHATSTHIS "A RichText Composer."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposerWidget
    INCLUDE_FILE "KPIMTextEdit/RichTextComposerWidget"
    TOOLTIP "A RichText Composer Widget(KDE-PIM)"
    WHATSTHIS "A RichText Composer Widget."
    GROUP "Input (KDE-PIM)"
    )

ecm_add_qtdesignerplugin(kpimtexteditwidgets
    NAME KPimTextEditWidgets
    OUTPUT_NAME kpimtexteditwidgets
    WIDGETS
    KPIMTextEdit::RichTextEditorWidget
    KPIMTextEdit::PlainTextEditorWidget
    KPIMTextEdit::RichTextComposer
    KPIMTextEdit::RichTextComposerWidget
    LINK_LIBRARIES
    KF5::PimTextEdit
    Qt::Widgets
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
    )
