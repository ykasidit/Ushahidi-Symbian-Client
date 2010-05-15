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


#ifndef __Ushahidi_APPUI_H__
#define __Ushahidi_APPUI_H__

#include <aknviewappui.h>
#include "AzqLocationReader.h"
#include "AzqInternalGPSReader.h"
#include "FolderWatcher.h"
#include <sysutil.h>

class CUshahidiView;

class TUploadFile
	{
public:
	TBuf<128> iFile;
	TBuf<128> iCsvRefFile;
	TBuf<32> iNameAs;
	};

class CUshahidiAppUi : public CAknViewAppUi,
						public MBtGPSReaderObserver,
						public MFolderObserver
    {

    public: // Constructors and destructor
        void ConstructL();
        ~CUshahidiAppUi();

    public: // New functions from CEikAppUi
        void HandleCommandL( TInt aCommand );
        virtual void HandleResourceChangeL( TInt aType );

    public://derived or engine callbacks
    	void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);
    	void OnGPSStateUpdate(const TDesC& state, TAzqGPSData& aGPSData);
		static TInt OnUploadTimerCallback(TAny* caller);
		void OnStateEvent(TInt State, TInt err, const TDesC& desc);
		void OnFolderChange(const TDesC& aFolder);

    private: //views
        CUshahidiView* iAppView;

    private: //engine stuff
    	////////////funcs
    	void CreateInfoCsv(TDes& aFileName);
		TInt GetNWatingUploadL();
		void GenerateLogNameL(const TDesC& aFolderPath,TTime &aTestTime,TDes& aLogFile);
		void AddNewFiles(const TDesC& folder, const TDesC& match);
		void UploadAllInList();
		void LeaveIfPathDiskFullL(RFs &aFs, const TDesC& aLogFile, TInt aBytesToWrite);
		//////////////////

		//////////////vars
		CPeriodic* iPeriodic;
    	CAzqInternalGPSReader* iAzqInternalGPSReader;
		CFolderWatcher* iCDataImgFolderWatcher;
		CFolderWatcher* iEImgFolderWatcher;
		RArray<TUploadFile> iUploadList;
		TUploadFile iUploadingFile;
		TFileName iGpsStateStr;
		TAzqGPSData iGPSData;
		TTime iLastGpsDataTime;
		///////////////////
    };


#endif // __Ushahidi_APPUI_H__

// End of File
