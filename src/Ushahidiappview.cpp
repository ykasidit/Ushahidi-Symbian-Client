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
#include "Ushahidiappview.h"


// -----------------------------------------------------------------------------
// CUshahidiAppView::CUshahidiAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUshahidiAppView::CUshahidiAppView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiAppView* CUshahidiAppView::NewL( const TRect& aRect )
    {
    CUshahidiAppView* self = CUshahidiAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiAppView* CUshahidiAppView::NewLC( const TRect& aRect )
    {
    CUshahidiAppView* self = new ( ELeave ) CUshahidiAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUshahidiAppView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CUshahidiAppView::~CUshahidiAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CUshahidiAppView::~CUshahidiAppView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiAppView::Draw() const
// Draws the display
// -----------------------------------------------------------------------------
//
void CUshahidiAppView::Draw( const TRect& /*aRect*/ ) const
    {
    // Clear the screen
    CWindowGc& gc = SystemGc();
    gc.Clear( Rect() );
    }

// End of File
