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
#include <eikstart.h>

#include "Ushahidiapplication.h"


// -----------------------------------------------------------------------------
// NewApplication()
// Constructs CAknExMenuApp.
// Create an application, and return a pointer to it
// -----------------------------------------------------------------------------
EXPORT_C CApaApplication* NewApplication()
    {
    return ( static_cast<CApaApplication*> ( new CUshahidiApplication ) );
    }

// -----------------------------------------------------------------------------
// E32Main()
// Entry point function for Symbian Apps.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

// End of File
