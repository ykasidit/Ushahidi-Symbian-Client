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


class CUshahidiContainer : public CCoeControl
    {
    public:
        static CUshahidiContainer* NewL( const TRect& aRect );
        static CUshahidiContainer* NewLC( const TRect& aRect );
        void ConstructL( const TRect& aRect );

    public: // from CoeControl
        TInt CountComponentControls() const;
        CCoeControl* ComponentControl( TInt aIndex ) const;
        void Draw( const TRect& aRect ) const;

    };

#endif // __Ushahidi_Container_H__


// End of File
