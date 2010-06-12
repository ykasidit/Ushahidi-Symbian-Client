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

#ifndef __Ushahidi_Container_H__
#define __Ushahidi_Container_H__

#include <aknview.h>
#include <akndef.h>
#include <aknlists.h>
#include "Ushahidiappui.h"
#include "Ushahidiview.h"

class CUshahidiContainer : public CCoeControl
    {
    public:
        static CUshahidiContainer* NewL(CUshahidiView& aView, const TRect& aRect );
        static CUshahidiContainer* NewLC(CUshahidiView& aView, const TRect& aRect );
        void ConstructL( const TRect& aRect );
        virtual ~CUshahidiContainer();
        CUshahidiContainer(CUshahidiView& aView);

    public: // from CoeControl
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl( TInt aIndex ) const;
        void Draw( const TRect& aRect ) const;

        void SizeChanged();


        void HandleResourceChange(TInt aType);

        void MakeListBoxL();

        CDesCArray* GetArrayL(CArrayPtr<CGulIcon>* aIcon);

        void UpdateScrollBar(CEikListBox* aListBox);

        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aEventCode);


    protected:

        CAknDoubleGraphicStyleListBox* iListBox;
        TRect iStatusRect;
        CUshahidiView& iView;


    };

#endif // __Ushahidi_Container_H__


// End of File
