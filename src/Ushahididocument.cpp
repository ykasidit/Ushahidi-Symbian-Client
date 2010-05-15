/*
    Copyright (C) 2010 Kasidit Yusuf.

    This file is part of "Ushahidi Symbian Uploader".

    "Ushahidi Symbian Uploader" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "Ushahidi Symbian Uploader" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Ushahidi Symbian Uploader".  If not, see <http://www.gnu.org/licenses/>.
*/


// INCLUDE FILES
#include "Ushahidiappui.h"
#include "Ushahididocument.h"





// -----------------------------------------------------------------------------
// CUshahidiDocument::CUshahidiDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUshahidiDocument::CUshahidiDocument( CEikApplication& aApp ) :
                                          CAknDocument( aApp )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiDocument* CUshahidiDocument::NewL( CEikApplication& aApp )
    {
    CUshahidiDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CUshahidiDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiDocument* CUshahidiDocument::NewLC( CEikApplication& aApp )
    {
    CUshahidiDocument* self = new ( ELeave ) CUshahidiDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUshahidiDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiDocument::~CUshahidiDocument()
// Destructor.
// -----------------------------------------------------------------------------
//
CUshahidiDocument::~CUshahidiDocument()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiDocument::CreateAppUiL()
// Create the application user interface, and return a pointer to it
// -----------------------------------------------------------------------------
//
CEikAppUi* CUshahidiDocument::CreateAppUiL()
    {
    return( static_cast<CEikAppUi*>( new ( ELeave ) CUshahidiAppUi ) );
    }

// End of File
