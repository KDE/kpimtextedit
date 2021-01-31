include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextEditorWidget
    INCLUDE_FILE "kpimtextedit/richtexteditorwidget.h"
    TOOLTIP "A RichTextEditor (KDE-PIM)"
    WHATSTHIS "A RichTextEditor."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::PlainTextEditorWidget
    INCLUDE_FILE "kpimtextedit/plaintexteditorwidget.h"
    TOOLTIP "A PlainTextEditor (KDE-PIM)"
    WHATSTHIS "A PlainTextEditor."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposer
    INCLUDE_FILE "kpimtextedit/richtextcomposer.h"
    TOOLTIP "A RichText Composer (KDE-PIM)"
    WHATSTHIS "A RichText Composer."
    GROUP "Input (KDE-PIM)"
    )

ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposerWidget
    INCLUDE_FILE "kpimtextedit/richtextcomposerwidget.h"
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
