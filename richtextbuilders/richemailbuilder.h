/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/


#ifndef RICHEMAILBUILDER_H
#define RICHEMAILBUILDER_H

#include "htmlbuilder.h"
#include "abstractemailbuilder.h"

/**
@brief Builds a html representation of an email.

This class builds html email content. The content is mostly the same as that of HTMLBuilder, but with additional support for quote blocks.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2
*/
class RichEmailBuilder : public AbstractEmailBuilder, public HTMLBuilder
{

    RichEmailBuilder() {}
    virtual ~RichEmailBuilder();

    // TODO: begin blockquote with the right style
    // Will the style come from the kmail settings?
    
    // A different background might be the desired effect here, but qt text classes do not work properly with background color  to allow that.
    // http://trolltech.com/developer/task-tracker/index_html?method=entry&id=226328
    // Stephen Kelly, 9th Sept 2008    
    virtual void beginQuote() { m_text.append("\n<blockquote style\"color:red;\">\n"); }
    
    virtual void endQuote() { m_text.append("\n</blockquote>\n"); }
    

};

#endif
