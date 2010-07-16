/*
 * Copyright (c) 2009 Nokia Corporation.
 */

#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <aknlists.h> // CAknSinglePopupMenuStyleListBox
#include <uri8.h>
#include <http.h>
#include <stringloader.h>
#include <chttpformencoder.h>
#include <httpstringconstants.h>
#include <http/rhttptransaction.h>
#include <http/rhttpsession.h>
#include <http/rhttpheaders.h>

#include <commdbconnpref.h>

#include "ClientEngine.h"

// Used user agent for requests
_LIT8(KUserAgent, "UshahidiSymbianUploader 1.0");

// This client accepts all content types.
// (change to e.g. "text/plain" for plain text only)
_LIT8(KAccept, "*/*");

_LIT(KTextPreferredCarrierAvailable,"Preferred carrier available");
_LIT(KTextNewCarrierActive,         "New carrier active");

const TInt KStatustextBufferSize = 32;
const TInt KInfotextBufferSize = 64;
const TInt KURIBufferSize = 128;

// ----------------------------------------------------------------------------
// CClientEngine::NewL()
// ----------------------------------------------------------------------------
CClientEngine* CClientEngine::NewL(MClientObserver& aObserver)
  {
    CClientEngine* self = CClientEngine::NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
  }


// ----------------------------------------------------------------------------
// CClientEngine::NewLC()
// ----------------------------------------------------------------------------
CClientEngine* CClientEngine::NewLC(MClientObserver& aObserver)
  {
    CClientEngine* self = new (ELeave) CClientEngine(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
  }


// ----------------------------------------------------------------------------
// CClientEngine::CClientEngine()
// ----------------------------------------------------------------------------
CClientEngine::CClientEngine(MClientObserver& aObserver)
: CActive(CActive::EPriorityStandard),
  iObserver(aObserver),
  iPostData(NULL),
  iConnectionSetupDone(EFalse),
  iPrevProfileId(-1),
  iTransactionOpen(EFalse)
  {
  }


// ----------------------------------------------------------------------------
// CClientEngine::~CClientEngine()
// ----------------------------------------------------------------------------
CClientEngine::~CClientEngine()
    {
    Cancel();

    if (iTransactionOpen)
        {
        iTransaction.Close();
        iTransactionOpen = EFalse;
        }

    if (iMobility)
        {
        iMobility->Cancel();
        }
    delete iMobility;

    iSession.Close();
    iConnection.Close();
    iSocketServ.Close();

    delete iPostData;

    delete iUri;
    delete iContentType;
    delete iBody;
    }


// ----------------------------------------------------------------------------
// CClientEngine::ConstructL()
// ----------------------------------------------------------------------------
void CClientEngine::ConstructL()
  {
  CActiveScheduler::Add(this);
  }


// ----------------------------------------------------------------------------
// CClientEngine::SetupConnectionL()
// ----------------------------------------------------------------------------
void CClientEngine::SetupConnectionL()
    {
    if (iConnectionSetupDone)
        {
        // Connection setup is done
        User::Leave(KErrAlreadyExists);
        }
    if (IsActive())
        {
        User::Leave(KErrInUse);
        }

    // Open HTTP Session
    iSession.OpenL();

    // Install this class as the callback for authentication requests. When
    // page requires authentication the framework calls GetCredentialsL to get
    // user name and password.
    InstallAuthenticationL(iSession);

    // Open CCmApplicationSettingsUi
    TCmSettingSelection userSelection;
    CCmApplicationSettingsUi* settings = CCmApplicationSettingsUi::NewL();
    CleanupStack::PushL(settings);
    TUint listedItems = CMManager::EShowAlwaysAsk |
                        CMManager::EShowDefaultConnection |
                        CMManager::EShowDestinations |
                        CMManager::EShowConnectionMethods;
    TBearerFilterArray filter;
    TBool selected = settings->RunApplicationSettingsL(userSelection,listedItems,filter);
    CleanupStack::PopAndDestroy(settings);

    // Check selection
    if (selected) {
        switch (userSelection.iResult)
            {
            case CMManager::EDestination:
                {
                TConnSnapPref prefs;
                prefs.SetSnap(userSelection.iId);

                User::LeaveIfError(iSocketServ.Connect());
                User::LeaveIfError(iConnection.Open(iSocketServ));
                iConnection.Start(prefs, iStatus);
                break;
                }
            case CMManager::EConnectionMethod:
                {
                TCommDbConnPref prefs;
                prefs.SetIapId(userSelection.iId);
                prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);

                User::LeaveIfError(iSocketServ.Connect());
                User::LeaveIfError(iConnection.Open(iSocketServ));
                iConnection.Start(prefs, iStatus);
                break;
                }
            case CMManager::EDefaultConnection:
                {
                User::LeaveIfError(iSocketServ.Connect());
                User::LeaveIfError(iConnection.Open(iSocketServ));
                iConnection.Start(iStatus);
                break;
                }
            default: // EAlwaysAsk
                {
                TCommDbConnPref prefs;
                prefs.SetDialogPreference(ECommDbDialogPrefPrompt);

                User::LeaveIfError(iSocketServ.Connect());
                User::LeaveIfError(iConnection.Open(iSocketServ));
                iConnection.Start(prefs, iStatus);
                }
            }

        SetActive();
        }
    }

// ----------------------------------------------------------------------------
// CClientEngine::FindExistingConnection()
// ----------------------------------------------------------------------------
TBool CClientEngine::FindExistingConnection()
    {
    // NOTE: this method is not in use

    // Enumerates through existing connections and tries to find one that matches
    // the selected IAP. If one is found, ETrue is returned; otherwise, EFalse.
    TBool connected = EFalse;
    TConnectionInfoBuf connInfo;
    TUint count;
    if (iConnection.EnumerateConnections(count) == KErrNone)
        {
        for (TUint i=1; i<=count; i++)
            {
            // Note: GetConnectionInfo expects 1-based index.
            if (iConnection.GetConnectionInfo(i, connInfo) == KErrNone)
                {
                if (connInfo().iIapId == iSelectedIap)
                    {
                    connected = ETrue;
                    break;
                    }
                }
            }
        }
    return connected;
    }

// ----------------------------------------------------------------------------
// CClientEngine::SetHeaderL()
// ----------------------------------------------------------------------------
void CClientEngine::SetHeaderL(RHTTPHeaders aHeaders,
               TInt aHdrField,
               const TDesC8& aHdrValue)
  {
  RStringF valStr = iSession.StringPool().OpenFStringL(aHdrValue);
  CleanupClosePushL(valStr);
  THTTPHdrVal val(valStr);
  aHeaders.SetFieldL(iSession.StringPool().StringF(aHdrField,
    RHTTPSession::GetTable()), val);
  CleanupStack::PopAndDestroy();  // valStr
  }


// ----------------------------------------------------------------------------
// CClientEngine::IssueHTTPGetL()
// ----------------------------------------------------------------------------
void CClientEngine::IssueHTTPGetL(const TDesC8& aUri)
  {
  if (IsActive())
      {
      return;
      }

  iEngineState = EGet;

  delete iUri; iUri = NULL;
  delete iContentType; iContentType = NULL;
  delete iBody; iBody = NULL;

  iUri = aUri.AllocL();

  // Create HTTP connection
  TRAPD(err, SetupConnectionL());
  if (err == KErrAlreadyExists)
      {
      DoHTTPGetL();
      }
  else if (err != KErrNone)
      {
      HBufC* resTxCancelled = StringLoader::LoadLC(R_HTTP_TX_CANCELLED);
      iObserver.ClientEvent(*resTxCancelled);
      CleanupStack::PopAndDestroy(resTxCancelled);
      return;
      }
  }


// ----------------------------------------------------------------------------
// CClientEngine::DoHTTPGetL()
// ----------------------------------------------------------------------------
void CClientEngine::DoHTTPGetL()
  {
  // Parse string to URI (as defined in RFC2396)
  TUriParser8 uri;
  uri.Parse(*iUri);

  // Get request method string for HTTP GET
  RStringF method = iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable());

  // Open transaction with previous method and parsed uri. This class will
  // receive transaction events in MHFRunL and MHFRunError.
  iTransaction = iSession.OpenTransactionL(uri, *this, method);
  iTransactionOpen = ETrue;

  // Set headers for request; user agent and accepted content type
  RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
  SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
  SetHeaderL(hdr, HTTP::EAccept, KAccept);

  // Submit the transaction. After this the framework will give transaction
  // events via MHFRunL and MHFRunError.
  iTransaction.SubmitL();

  HBufC* resConnecting = StringLoader::LoadLC(R_HTTP_CONNECTING);
  iObserver.ClientEvent(*resConnecting);
  CleanupStack::PopAndDestroy(resConnecting);
  }


// ----------------------------------------------------------------------------
// CClientEngine::IssueHTTPPostL()
// ----------------------------------------------------------------------------
void CClientEngine::IssueHTTPPostL(const TDesC8& aUri,
                 const TDesC8& aContentType,
                 const TDesC8& aBody)
  {
  if (IsActive())
      {
      return;
      }

  iEngineState = EPost;

  delete iUri; iUri = NULL;
  delete iContentType; iContentType = NULL;
  delete iBody; iBody = NULL;

  iUri = aUri.AllocL();
  iContentType = aContentType.AllocL();
  iBody = aBody.AllocL();

  // Create HTTP connection
  TRAPD(err, SetupConnectionL());
  if (err == KErrAlreadyExists)
      {
      DoHTTPPostL();
      }
  else if (err != KErrNone)
      {
      HBufC* resTxCancelled = StringLoader::LoadLC(R_HTTP_TX_CANCELLED);
      iObserver.ClientEvent(*resTxCancelled);
      CleanupStack::PopAndDestroy(resTxCancelled);
      return;
      }
}

// ----------------------------------------------------------------------------
// CClientEngine::DoHTTPPostL()
// ----------------------------------------------------------------------------
void CClientEngine::DoHTTPPostL()
  {
  // Parse string to URI
  TUriParser8 uri;
  uri.Parse(*iUri);

  // Copy data to be posted into member variable; iPostData is used later in
  // methods inherited from MHTTPDataSupplier.
  delete iPostData;
  iPostData = 0;
  iPostData = iBody->AllocL();

  // Get request method string for HTTP POST
  RStringF method = iSession.StringPool().StringF(HTTP::EPOST,RHTTPSession::GetTable());

  // Open transaction with previous method and parsed uri. This class will
  // receive transaction events in MHFRunL and MHFRunError.
  iTransaction = iSession.OpenTransactionL(uri, *this, method);
  iTransactionOpen = ETrue;

  // Set headers for request; user agent, accepted content type and body's
  // content type.
  RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
  SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
  SetHeaderL(hdr, HTTP::EAccept, KAccept);
  SetHeaderL(hdr, HTTP::EContentType, *iContentType);

  // Set this class as an data supplier. Inherited MHTTPDataSupplier methods
  // are called when framework needs to send body data.
  MHTTPDataSupplier* dataSupplier = this;
  iTransaction.Request().SetBody(*dataSupplier);

  // Submit the transaction. After this the framework will give transaction
  // events via MHFRunL and MHFRunError.
  iTransaction.SubmitL();

  HBufC* resConnecting = StringLoader::LoadLC(R_HTTP_CONNECTING);
  iObserver.ClientEvent(*resConnecting);
  CleanupStack::PopAndDestroy(resConnecting);
  }

// ----------------------------------------------------------------------------
// CClientEngine::CancelTransaction()
// ----------------------------------------------------------------------------
void CClientEngine::CancelTransaction()
  {
  iEngineState = EIdle;
  delete iUri; iUri = NULL;
  delete iContentType; iContentType = NULL;
  delete iBody; iBody = NULL;

  // Close() also cancels transaction (Cancel() can also be used but
  // resources allocated by transaction must be still freed with Close())
  if (iTransactionOpen)
      {
      iTransaction.Close();
      iTransactionOpen = EFalse;

      HBufC* resTxCancelled = StringLoader::LoadLC(R_HTTP_TX_CANCELLED);
      iObserver.ClientEvent(*resTxCancelled);
      CleanupStack::PopAndDestroy(resTxCancelled);
      }
  }


// ----------------------------------------------------------------------------
// CClientEngine::MHFRunL()
// ----------------------------------------------------------------------------
void CClientEngine::MHFRunL(RHTTPTransaction aTransaction,
              const THTTPEvent& aEvent)
  {
  switch (aEvent.iStatus)
    {
    case THTTPEvent::EGotResponseHeaders:
      {
      // HTTP response headers have been received. Use
      // aTransaction.Response() to get the response. However, it's not
      // necessary to do anything with the response when this event occurs.

      // Get HTTP status code from header (e.g. 200)
      RHTTPResponse resp = aTransaction.Response();
      TInt status = resp.StatusCode();

      // Get status text (e.g. "OK")
      TBuf<KStatustextBufferSize> statusText;
      statusText.Copy(resp.StatusText().DesC());

      HBufC* resHeaderReceived = StringLoader::LoadLC(R_HTTP_HEADER_RECEIVED, statusText, status);
      iObserver.ClientEvent(*resHeaderReceived);
      CleanupStack::PopAndDestroy(resHeaderReceived);
      break;
      }

    case THTTPEvent::EGotResponseBodyData:
      {
      // Part (or all) of response's body data received. Use
      // aTransaction.Response().Body()->GetNextDataPart() to get the actual
      // body data.

      // Get the body data supplier
      MHTTPDataSupplier* body = aTransaction.Response().Body();
      TPtrC8 dataChunk;

      // GetNextDataPart() returns ETrue, if the received part is the last
      // one.
      TBool isLast = body->GetNextDataPart(dataChunk);
      iObserver.ClientBodyReceived(dataChunk);

      HBufC* resBytesReceived = StringLoader::LoadLC(R_HTTP_BYTES_RECEIVED, dataChunk.Length());
      iObserver.ClientEvent(*resBytesReceived);
      CleanupStack::PopAndDestroy(resBytesReceived);

      // NOTE: isLast may not be ETrue even if last data part received.
      // (e.g. multipart response without content length field)
      // Use EResponseComplete to reliably determine when body is completely
      // received.
      if (isLast)
        {
        HBufC* resBodyReceived = StringLoader::LoadLC(R_HTTP_BODY_RECEIVED);
        iObserver.ClientEvent(*resBodyReceived);
        CleanupStack::PopAndDestroy(resBodyReceived);
        }

      // Always remember to release the body data.
      body->ReleaseData();
      break;
      }

    case THTTPEvent::EResponseComplete:
      {
      // Indicates that header & body of response is completely received.
      // No further action here needed.
      HBufC* resTxComplete = StringLoader::LoadLC(R_HTTP_TX_COMPLETE);
      iObserver.ClientEvent(*resTxComplete);
      CleanupStack::PopAndDestroy(resTxComplete);
      break;
      }

    case THTTPEvent::ESucceeded:
      {
      // Indicates that transaction succeeded.
      HBufC* resTxSuccessful = StringLoader::LoadLC(R_HTTP_TX_SUCCESSFUL);
      iObserver.ClientEvent(*resTxSuccessful);
      CleanupStack::PopAndDestroy(resTxSuccessful);

      // Transaction can be closed now. It's not needed anymore.
      aTransaction.Close();
      iTransactionOpen = EFalse;
      break;
      }

    case THTTPEvent::EFailed:
      {
      // Transaction completed with failure.
      HBufC* resTxFailed = StringLoader::LoadLC(R_HTTP_TX_FAILED);
      iObserver.ClientEvent(*resTxFailed);
      CleanupStack::PopAndDestroy(resTxFailed);
      aTransaction.Close();
      iTransactionOpen = EFalse;
      break;
      }

    default:
      // There are more events in THTTPEvent, but they are not usually
      // needed. However, event status smaller than zero should be handled
      // correctly since it's error.
      {
      if (aEvent.iStatus < 0)
        {
          HBufC* resNoInternetConnection = StringLoader::LoadLC(
            R_HTTP_NO_INTERNET_CONNECTION, aEvent.iStatus);
          iObserver.ClientEvent(*resNoInternetConnection);
          CleanupStack::PopAndDestroy(resNoInternetConnection);

          // Close the transaction on errors
          aTransaction.Close();
          iTransactionOpen = EFalse;
        }
      break;
      }
    }
  }


// ----------------------------------------------------------------------------
// CClientEngine::IsRunning()
// ----------------------------------------------------------------------------
TBool CClientEngine::IsRunning()
    {
    if (IsActive())
        return ETrue;
    else
        return EFalse;
    }

// ----------------------------------------------------------------------------
// CClientEngine::MHFRunError()
// ----------------------------------------------------------------------------
TInt CClientEngine::MHFRunError(TInt aError,
                RHTTPTransaction /*aTransaction*/,
                const THTTPEvent& /*aEvent*/)
  {
  // Just notify about the error and return KErrNone.
  HBufC* resMHFRunError = StringLoader::LoadLC(R_HTTP_MHFRUN_ERROR, aError);
  iObserver.ClientEvent(*resMHFRunError);
  CleanupStack::PopAndDestroy(resMHFRunError);
  return KErrNone;
  }


// ----------------------------------------------------------------------------
// CClientEngine::GetNextDataPart()
// ----------------------------------------------------------------------------
TBool CClientEngine::GetNextDataPart(TPtrC8& aDataPart)
  {
  if(iPostData)
    {
    // Provide pointer to next chunk of data (return ETrue, if last chunk)
    // Usually only one chunk is needed, but sending big file could require
    // loading the file in small parts.
    aDataPart.Set(iPostData->Des());
    }
  return ETrue;
  }


// ----------------------------------------------------------------------------
// CClientEngine::ReleaseData()
// ----------------------------------------------------------------------------
void CClientEngine::ReleaseData()
  {
  // It's safe to delete iPostData now.
  delete iPostData;
  iPostData = NULL;
  }

// ----------------------------------------------------------------------------
// CClientEngine::Reset()
// ----------------------------------------------------------------------------
TInt CClientEngine::Reset()
  {
  // Nothing needed since iPostData still exists and contains all the data.
  // (If a file is used and read in small parts we should seek to beginning
  // of file and provide the first chunk again in GetNextDataPart() )
  return KErrNone;
  }


// ----------------------------------------------------------------------------
// CClientEngine::OverallDataSize()
// ----------------------------------------------------------------------------
TInt CClientEngine::OverallDataSize()
  {
  if(iPostData)
    return iPostData->Length();
  else
    return KErrNotFound ;
  }

// ----------------------------------------------------------------------------
// CClientEngine::GetCredentialsL()
// ----------------------------------------------------------------------------
TBool CClientEngine::GetCredentialsL(const TUriC8& aURI,
                   RString aRealm,
                   RStringF aAuthenticationType,
                   RString& aUsername,
                   RString& aPassword)
  {
  // aURI, aReal and aAuthenticationType are informational only. We only need
  // to set aUsername and aPassword and return ETrue, if aUsername and
  // aPassword are provided by user.

  // Buffer for string parameters that may change order
  CDesCArrayFlat* strings = new CDesCArrayFlat(2);
  CleanupStack::PushL(strings);

  // Initialize parameter buffer

  // First parameter
  HBufC* uri16 = HBufC::NewLC(aURI.UriDes().Length());
  uri16->Des().Copy(aURI.UriDes());
  strings->AppendL(*uri16);
  CleanupStack::PopAndDestroy(uri16);

  // Second parameter
  HBufC* authType16 = HBufC::NewLC(aAuthenticationType.DesC().Length());
  authType16->Des().Copy(aAuthenticationType.DesC());
  strings->AppendL(*authType16);
  CleanupStack::PopAndDestroy(authType16);

  HBufC* resAuthNote = StringLoader::LoadLC(R_HTTP_AUTH_NOTE, *strings);
  HBufC* resAuthRequired = StringLoader::LoadLC(R_HTTP_AUTH_REQUIRED);
  CEikonEnv::Static()->InfoWinL(*resAuthRequired, *resAuthNote);

  CleanupStack::PopAndDestroy(resAuthNote);
  CleanupStack::PopAndDestroy(resAuthRequired);

  // Query user name and password
  TBuf<KDefaultBufferSize> userName;
  TBuf<KDefaultBufferSize> password;
  CAknMultiLineDataQueryDialog* dlg =
    CAknMultiLineDataQueryDialog::NewL(userName, password);

  if (!dlg->ExecuteLD(R_DIALOG_USER_PASSWORD_QUERY))
    return EFalse; // No credentials given; must return EFalse

  // Set aUsername and aPassword
  TBuf8<KDefaultBufferSize> temp;
  temp.Copy(userName);
  TRAPD(err, aUsername = aRealm.Pool().OpenStringL(temp));
  if (!err)
    {
    temp.Copy(password);
    TRAP(err, aPassword = aRealm.Pool().OpenStringL(temp));
    if (!err) return ETrue;
    }

  // Return ETrue if user has given credentials (username and password),
  // otherwise EFlase
  return EFalse;
  }

// ----------------------------------------------------------------------------
// CClientEngine::PreferredCarrierAvailable()
// ----------------------------------------------------------------------------
void CClientEngine::PreferredCarrierAvailable(TAccessPointInfo /*aOldAPInfo*/,
                                                  TAccessPointInfo /*aNewAPInfo*/,
                                                  TBool /*aIsUpgrade*/,
                                                  TBool aIsSeamless)
    {
    if (!aIsSeamless)
        {
        iMobility->MigrateToPreferredCarrier();
        }
    }

// ----------------------------------------------------------------------------
// CClientEngine::NewCarrierActive()
// ----------------------------------------------------------------------------
void CClientEngine::NewCarrierActive(TAccessPointInfo /*aNewAPInfo*/,
                                         TBool aIsSeamless)
    {
    if (!aIsSeamless)
        {
        iMobility->NewCarrierAccepted();
        }
    }

// ----------------------------------------------------------------------------
// CClientEngine::Error()
// ----------------------------------------------------------------------------
void CClientEngine::Error(TInt /*aError*/)
    {
    }

// ----------------------------------------------------------------------------
// CClientEngine::DoCancel()
// ----------------------------------------------------------------------------
void CClientEngine::DoCancel()
    {
    iConnection.Stop();
    }

// ----------------------------------------------------------------------------
// CClientEngine::RunL()
// ----------------------------------------------------------------------------
void CClientEngine::RunL()
    {
    TInt statusCode = iStatus.Int();

    if (!iConnectionSetupDone && statusCode == KErrNone)
        {
        // Connection done ok
        iConnectionSetupDone = ETrue;
        // Register for mobility API
        iMobility = CActiveCommsMobilityApiExt::NewL(iConnection, *this);
        // Start selected HTTP action
        switch (iEngineState)
           {
            case EIdle:
                {
                CancelTransaction();
                break;
                }
           case EGet:
               {
               DoHTTPGetL();
               break;
               }
           case EPost:
               {
               DoHTTPPostL();
               break;
               }
           };
        }
    }


// end of file
