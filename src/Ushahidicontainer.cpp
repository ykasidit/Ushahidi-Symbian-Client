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
#include "Ushahidicontainer.h"





// -----------------------------------------------------------------------------
// CUshahidiContainer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiContainer* CUshahidiContainer::NewL( const TRect& aRect )
    {
    CUshahidiContainer* self = CUshahidiContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiContainer::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiContainer* CUshahidiContainer::NewLC( const TRect& aRect )
    {
    CUshahidiContainer* self = new ( ELeave ) CUshahidiContainer;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiContainer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUshahidiContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    SetRect( aRect );
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CUshahidiContainer::CountComponentControls() const
// returns number of controls inside this container.
// -----------------------------------------------------------------------------
//
TInt CUshahidiContainer::CountComponentControls() const
    {
    return 0;
    }


// -----------------------------------------------------------------------------
// CUshahidiContainer::ComponentControl() const
// returns pointer of controls inside this container
// -----------------------------------------------------------------------------
//
CCoeControl* CUshahidiContainer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CUshahidiContainer::Draw() const
// Draws the display
// -----------------------------------------------------------------------------
//
void CUshahidiContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbBlue );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.DrawRect( aRect );
    }


// End of File

