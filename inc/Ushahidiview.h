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

#ifndef __Ushahidi_View_H__
#define __Ushahidi_View_H__

#include <aknview.h>

class CUshahidiContainer;

class CUshahidiView: public CAknView
    {
    public:   // Constructors and destructor
        static CUshahidiView* NewL();
        static CUshahidiView* NewLC();
        virtual ~CUshahidiView();

    public: // Functions from base classes from CAknView
        TUid Id() const;
        void HandleCommandL( TInt aCommand );
        void HandleSizeChange( TInt aType );
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
        void DoDeactivate();

    private:   // Constructors and destructor
        CUshahidiView();
        void ConstructL();

    private:
        CUshahidiContainer* iContainer;
        TUid iId;
    };


#endif // __Ushahidi_View_H__


// End of File
