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


#ifndef PLAINTEXTEMAILBUILDER_H
#define PLAINTEXTEMAILBUILDER_H

#include "plaintextmarkupbuilder.h"
#include "abstractemailbuilder.h"


/**
@brief Builds a plain text representation of an email.

This class builds a plain text email. The content is mostly the same as that of PlainTextMarkupBuilder, but with additional support for quote blocks.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class PlainTextEmailBuilder : public AbstractEmailBuilder, public PlainTextMarkupbuilder
{
    PlainTextEmailBuilder() {}
    
    virtual void beginQuote() {
        // Put one more '>' at the beginning of paragraphs.
    }
    
    virtual void endQuote() {
        // Put one fewer '>' at the beginning of paragraphs.
    }

    
    //TODO: Make an -- attachments -- section similar to references?
    // Override getResult to do that.
    
    
};

#endif
