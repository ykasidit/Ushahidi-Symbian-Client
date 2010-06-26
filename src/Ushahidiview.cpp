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
#include <aknviewappui.h>
#include <aknconsts.h>
#include <aknutils.h>
#include <Ushahidi.rsg>

#include "Ushahidiview.h"
#include "Ushahidicontainer.h"
#include "Ushahidi.hrh"


// -----------------------------------------------------------------------------
// CUshahidiView::CUshahidiView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUshahidiView::CUshahidiView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CUshahidiView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiView* CUshahidiView::NewL()
    {
    CUshahidiView* self = CUshahidiView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiView* CUshahidiView::NewLC()
    {
    CUshahidiView* self = new ( ELeave ) CUshahidiView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUshahidiView::ConstructL()
    {
    BaseConstructL( R_USHAHIDI_VIEW );
    }

// -----------------------------------------------------------------------------
// CUshahidiView::~CUshahidiView()
// Destructor.
// -----------------------------------------------------------------------------
//
CUshahidiView::~CUshahidiView()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CUshahidiView::Id()
// Returns View's ID.
// -----------------------------------------------------------------------------
//
TUid CUshahidiView::Id() const
    {
    return TUid::Uid( EUshahidiMainView );
    }

// -----------------------------------------------------------------------------
// CUshahidiView::DoActivateL()
// Activate an MultiView
// -----------------------------------------------------------------------------
//
void CUshahidiView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
		iContainer = CUshahidiContainer::NewL(*this, ClientRect() );
		if(iContainer)
		{
			AppUi()->AddToStackL( *this, iContainer );
		}
    }

// -----------------------------------------------------------------------------
// CUshahidiView::DoDeactivate()
// DeActivate an MultiView
// -----------------------------------------------------------------------------
//
void CUshahidiView::DoDeactivate()
    {

    if ( iContainer )
        {
        // Remove View's container form controllStack
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CUshahidiView::HandleCommandL()
// Takes care of Command handling.
// -----------------------------------------------------------------------------
//
void CUshahidiView::HandleCommandL( TInt aCommand )
    {
        AppUi()->HandleCommandL( aCommand );

    }

// -----------------------------------------------------------------------------
// CUshahidiView::HandleSizeChange()
// Called by HandleResourceChangeL() from CUshahidiAppUi when layout is
// changed.
// -----------------------------------------------------------------------------
//
void CUshahidiView::HandleSizeChange( TInt aType )
    {
    if( iContainer )
        {
        iContainer->HandleResourceChange( aType );

        if ( aType==KEikDynamicLayoutVariantSwitch )
            {
            TRect rect;
            AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
            iContainer->SetRect(rect);
            }
        }
    }

void CUshahidiView::UpdateText(TTextEnum index,const TDesC& text)
{
	switch(index)
	{
	case EStatusStr: iText = text; break;
	case EGpsStr: iGpsStr = text; break;
	case ETransferState: iFtpStateStr = text; break;
	}

	if(iContainer)
		iContainer->DrawNow();
}

void CUshahidiView::RefreshULListL()
{
	if(iContainer)
		iContainer->MakeListBoxL();
}
// End of File
