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
#include "AzqBtGPSReader.h"
#include "FolderWatcher.h"
#include <sysutil.h>
#include <badesca.h>
#include "ClientEngine.h"

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
						public MFolderObserver,
						public MClientObserver
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

		//MClientObserver http engine callbacks
		void ClientEvent(TInt aError, const TDesC& aEventDescription);
	    void ClientBodyReceived(const TDesC8& aBodyData);
	    ////////////

    private: //views
        CUshahidiView* iAppView;

    private: //engine stuff
    	////////////funcs
    	void CreateInfoCsv(TDes& aFileName);
		TInt GetNWatingUpload();
		void GenerateLogNameL(const TDesC& aFolderPath,TTime &aTestTime,TDes& aLogFile);
		void AddNewFiles(const TDesC& folder, const TDesC& match);

    public:
    	void RefreshUploadList();
		void UploadAllInList();
		void LeaveIfPathDiskFullL(RFs &aFs, const TDesC& aLogFile, TInt aBytesToWrite);

		void StartBtGPS();
		void StopBtGPS();

		void GetWaitingUploadArrayL(CArrayPtr<CGulIcon>& updateIcons, CDesCArrayFlat& updateText);
		//////////////////

		//////////////vars
		CPeriodic* iPeriodic;
    	CAzqInternalGPSReader* iAzqInternalGPSReader;
    	CAzqBtGPSReader* iBtGPSReader;
		CFolderWatcher* iCDataImgFolderWatcher;
		CFolderWatcher* iEImgFolderWatcher;
		RArray<TUploadFile> iUploadList;
		TFileName iGpsStateStr;
		TAzqGPSData iGPSData;
		TTime iLastGpsDataTime;

		void PrepareQueryAndStartHTTPPostUploadL();
		HBufC8* iPostDataImage;
		CClientEngine* iClientEngine;//http post upload engine
		///////////////////

		//below api post info from http://wiki.ushahidi.com/doku.php?id=ushahidi_api

		/////incident info
		enum TIncidentCreateParams
		{
		EIncident_task,// - the task to be performed. Its report in this case.
		EIncident_incident_title,// - Required. The title of the incident/report.
		Eincident_description,// - Required. The description of the incident/report.
		Eincident_date,// - Required. The date of the incident/report. It usually in the format mm/dd/yyyy.
		Eincident_hour,// - Required. The hour of the incident/report. In the 12 hour format.
		Eincident_minute,// - Required. The minute of the incident/report.
		Eincident_ampm,// - Required. Is the incident/report am or pm. It of the form, am or pm.
		Eincident_category,// - Required. The categories the incident/report belongs to. It should be a comma separated value csv
		EIncident_latitude,// - Required. The latitude of the location of the incident report.
		EIncident_longitude,// - Required. The longitude of the location of the incident/report.
		EIncident_location_name,// - Required. The location of the incident/report.
		EIncident_person_first,// - Optional. The first name of the person submitting the incident/report.
		EIncident_person_last,// - Optional. The last name of the person submitting the incident/report.
		EIncident_person_email,// - Optional. The email address of the person submitting the incident/report.
		EIncident_incident_photo,//[] - Optional. Photos to accompany the incident/report.
		Eincident_news,// - Optional. A news source regarding the incident/report. A news feed.
		Eincident_video,// - Optional. A video link regarding the incident/report. Video services like youtube.com, video.google.com, metacafe.com,etc
		EIncident_resp,// - Optional. The data exchange, either XML or JSON. When not specified, JSON is used.
		NIncident_params
		};
		//////////////
		CDesC16ArrayFlat* iIncidentCreParams;


		/////////////photo upload info
		enum TPhotoUploadParams
		{
		EPhoto_id,//- Required. The id of the incident to add the video.
		EPhoto_photo,// - Required. The path to the photo.
		EPhoto_task,// - Required. The task to be performed. Its tagphoto in this case.
		NPhoto_params
		};
		CDesC16ArrayFlat* iPhotoUploadParams;

		//video upload info
		enum TVideoUploadParams
		{
		EVideo_id,// - Required. The id of the incident to add the video.
		EVideo_url,// - Required. The URL of the video.
		EVideo_task,// - Required. The task to be performed. Its tagvideo in this case.
		NVideo_params
		};
		///////////////////

		TBuf8<256> iServerPostURI;
    };


#endif // __Ushahidi_APPUI_H__

// End of File
