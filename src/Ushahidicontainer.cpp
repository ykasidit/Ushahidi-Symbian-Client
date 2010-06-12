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

//see listbox example here: http://wiki.forum.nokia.com/index.php/List_box_example
#include <akniconarray.h>
#include <eikclbd.h>

CUshahidiContainer::~CUshahidiContainer()
{
	delete iListBox;
}

CUshahidiContainer::CUshahidiContainer(CUshahidiView& aView):iView(aView)
{

}

// -----------------------------------------------------------------------------
// CUshahidiContainer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiContainer* CUshahidiContainer::NewL( CUshahidiView& aView, const TRect& aRect )
    {
    CUshahidiContainer* self = CUshahidiContainer::NewLC(aView, aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUshahidiContainer::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUshahidiContainer* CUshahidiContainer::NewLC(CUshahidiView& aView,  const TRect& aRect )
    {
    CUshahidiContainer* self = new ( ELeave ) CUshahidiContainer(aView);
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



void CUshahidiContainer::SizeChanged()
{
	TRAP_IGNORE( MakeListBoxL() );
}


void CUshahidiContainer::HandleResourceChange(TInt aType)
{
	TRect rect;

	if ( aType==KEikDynamicLayoutVariantSwitch )
	{
		AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
		SetRect(rect);
	}

	CCoeControl::HandleResourceChange(aType);
}


void CUshahidiContainer::MakeListBoxL()
{
	TInt MySetIndex(0);

	if(iListBox)
	{
		MySetIndex = iListBox->CurrentItemIndex();
	}

	delete iListBox;
	iListBox = NULL;

	iListBox = new( ELeave ) CAknDoubleGraphicStyleListBox();
	iListBox->ConstructL(this,EAknListBoxSelectionList);

	CArrayPtr<CGulIcon>* icons =new( ELeave ) CAknIconArray(1);
	CleanupStack::PushL(icons);

	iListBox->Model()->SetItemTextArray(GetArrayL(icons));
   	iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);
         //
         // Inform the listbox of item additions.
         // This should be done if you want your items displayed correctly
         iListBox ->HandleItemAdditionL();

	CleanupStack::Pop(icons);
	iListBox->ItemDrawer()->ColumnData()->SetIconArray(icons);

	iListBox->CreateScrollBarFrameL( ETrue );
	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
		CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);

	TRect rect = Rect();
	iStatusRect = rect;

	if(rect.Height() > rect.Width()) //if vertical layout active
	{
		rect.iTl.iY += rect.Height()/4;
		iStatusRect.iBr.iY = rect.iTl.iY - 1;
	}
	else
	{
		rect.iTl.iX = (3*rect.Width())/4;
		iStatusRect.iTl.iX = rect.iTl.iX + 1;
	}

	iListBox->SetRect(rect);

	iListBox->View()->SetListEmptyTextL(_L("Nothing to Upload"));

	iListBox->ActivateL();

	TInt ItemsCount = iListBox->Model()->ItemTextArray()->MdcaCount();

	if(ItemsCount > MySetIndex && MySetIndex >= 0)
		iListBox->SetCurrentItemIndex(MySetIndex);
	else if(ItemsCount > 0)
		iListBox->SetCurrentItemIndex(0);

	UpdateScrollBar(iListBox);
	iListBox->DrawNow();
}

CDesCArray* CUshahidiContainer::GetArrayL(CArrayPtr<CGulIcon>* aIcon)
{
	CDesCArrayFlat* myArray = new(ELeave)CDesCArrayFlat(1);
	CleanupStack::PushL(myArray);

	((CUshahidiAppUi*)(CCoeEnv::Static()->AppUi()))->GetWaitingUploadArrayL(*aIcon,*myArray);
	// 1) Append Text string to myArray in here as "1\tMytext",
	// where 1 is the zero based icon index
         //
         // Second and third icon indexes can also be specified in the form:
         // "1\tMytext\t2\t3" where 2 and 3 are also zero-based icon indexes
         //
	// 2) Remember to load and add icons in here. Listbox will
	// automatically resize iconsthus no need to set any size for them.
	// Tip: you may find AknIconUtils::CreateIconLC and
	// AknIconUtils::AvkonIconFileName() useful. In that case,
	// include akniconutils.h and avkon.mbg

	CleanupStack::Pop(myArray);
	return myArray;
}

void CUshahidiContainer::UpdateScrollBar(CEikListBox* aListBox)
    {
    if (aListBox)
        {
        TInt pos(aListBox->View()->CurrentItemIndex());
        if (aListBox->ScrollBarFrame())
            {
            aListBox->ScrollBarFrame()->MoveVertThumbTo(pos);
            }
        }
    }

void CUshahidiContainer::Draw(const TRect& /*aRect*/) const
{
	CWindowGc& gc = SystemGc();
	gc.Clear();



	const CFont* font = CEikonEnv::Static()->DenseFont();
	        gc.UseFont(font);
	        	   	   	    	    gc.SetPenColor(KRgbBlack);
	        	   	   	    	    gc.SetPenStyle(CGraphicsContext::ESolidPen);

	        TPoint p(iStatusRect.iTl.iX+2,iStatusRect.iTl.iY+2);
	        p.iY += font->HeightInPixels();
	        gc.DrawText(iView.iGpsStr,p);
	        p.iY += font->HeightInPixels();
	        p.iY += 5;
	        gc.DrawText(iView.iText,p);

	        gc.DiscardFont();

	        font = LatinPlain12();
	        gc.UseFont(font);
	        p.iY = Rect().Height() - (font->HeightInPixels());
	        gc.DrawText(iView.iFtpStateStr,p);
	        gc.DiscardFont();
}

TKeyResponse CUshahidiContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aEventCode)
{
	TKeyResponse Ret = EKeyWasNotConsumed;

	if(iListBox)
	{
		Ret = iListBox->OfferKeyEventL(aKeyEvent,aEventCode);
	}

	return Ret;
}

CCoeControl* CUshahidiContainer::ComponentControl( TInt /*aIndex*/) const
{
	return iListBox;
}

TInt CUshahidiContainer::CountComponentControls() const
{
	if(iListBox)
		return 1;
	else
		return 0;
}


// End of File

