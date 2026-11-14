/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

// Module header for qdoc PCH
#include "composer-ng/klinkdialog_p.h"
#include "composer-ng/nestedlisthelper_p.h"
#include "composer-ng/richtextcomposer.h"
#include "composer-ng/richtextcomposeractions.h"
#include "composer-ng/richtextcomposercontroler.h"
#include "composer-ng/richtextcomposeremailquotedecorator.h"
#include "composer-ng/richtextcomposeremailquotehighlighter.h"
#include "composer-ng/richtextcomposerimages.h"
#include "composer-ng/richtextcomposerwidget.h"
#include "composer-ng/richtextexternalcomposer.h"
#include "grantleebuilder/abstractmarkupbuilder.h"
#include "grantleebuilder/markupdirector.h"
#include "grantleebuilder/markupdirector_p.h"
#include "grantleebuilder/plaintextmarkupbuilder.h"
#include "grantleebuilder/texthtmlbuilder.h"
#include "inserthtmldialog.h"
#include "inserthtmleditor.h"
#include "insertimagedialog.h"
#include "insertimagewidget.h"
#include "inserttabledialog.h"
#include "inserttablewidget.h"
#include "kpimtextedit_private_export.h"
#include "tableactionmenu.h"
#include "tablecellformatdialog.h"
#include "tableformatdialog.h"
#include "textutils.h"
