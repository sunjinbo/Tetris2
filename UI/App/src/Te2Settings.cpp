/* ====================================================================
 * File: Te2Settings.cpp
 * Created: 09/10/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <f32file.h>
#include <s32file.h>
#include <eikenv.h>
#include <eikapp.h>
#include <aknutils.h>
#include "Te2Settings.h"

// CONSTANTS
const TTe2Mode KDefaultMode = ETe2ModeNormal;
const TTe2Audio KDefaultAudio = ETe2AudioOn;

_LIT( KTe2SettingsFileName, "Tetris.txt" );

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// TTe2Settings::TTe2Settings()
// ---------------------------------------------------------------------------
//
TTe2Settings::TTe2Settings()
: iFs( CCoeEnv::Static()->FsSession() )
  , iMode( KDefaultMode )
  , iAudio( KDefaultAudio )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// TTe2Settings::TTe2Settings()
// ---------------------------------------------------------------------------
//
TTe2Settings::TTe2Settings( const TTe2Settings& aSettings )
: iFs( CCoeEnv::Static()->FsSession() )
    {
	iMode = aSettings.iMode;
	iAudio = aSettings.iAudio;
    }

// ---------------------------------------------------------------------------
// TTe2Settings::operator=()
// ---------------------------------------------------------------------------
//
TTe2Settings& TTe2Settings::operator=( const TTe2Settings& aSettings )
    {
	iMode = aSettings.iMode;
	iAudio = aSettings.iAudio;

    return *this;
    }

// ---------------------------------------------------------------------------
// TTe2Settings::InternalizedL()
// ---------------------------------------------------------------------------
//
void TTe2Settings::InternalizedL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    while ( file.Open( iFs, fileName, EFileRead ) != KErrNone )
        {
        CreateDefaultSettingsL();
        }

    RFileReadStream inStream( file );
    CleanupClosePushL( inStream );

    TInt32 value = 0;

    value = inStream.ReadInt32L();
    iMode = (TTe2Mode)value;
    value = inStream.ReadInt32L();
    iAudio = (TTe2Audio)value;

    CleanupStack::PopAndDestroy( &inStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// TTe2Settings::ExternalizedL()
// ---------------------------------------------------------------------------
//
void TTe2Settings::ExternalizedL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    while ( file.Open( iFs, fileName, EFileWrite ) != KErrNone )
        {
        CreateDefaultSettingsL();
        }

    RFileWriteStream outStream( file );
    CleanupClosePushL( outStream );

    outStream.WriteInt32L( (TInt32)iMode );
    outStream.WriteInt32L( (TInt32)iAudio );

    outStream.CommitL();
    CleanupStack::PopAndDestroy( &outStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// TTe2Settings::CreateDefaultSettingsL()
// ---------------------------------------------------------------------------
//
void TTe2Settings::CreateDefaultSettingsL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    User::LeaveIfError( file.Create( iFs, fileName, EFileWrite ) );
    RFileWriteStream outStream( file );
    CleanupClosePushL( outStream );

    outStream.WriteInt32L( (TInt32)KDefaultMode );
    outStream.WriteInt32L( (TInt32)KDefaultAudio );

    outStream.CommitL();
    CleanupStack::PopAndDestroy( &outStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// TTe2Settings::GetFileName()
// ---------------------------------------------------------------------------
//
TInt TTe2Settings::GetFileName( TFileName& aFileName )
    {
	TInt err = GetPrivatePath( aFileName );
    if ( err == KErrNone )
    	{
        aFileName.Append( KTe2SettingsFileName );
    	}
    return err;
    }

// ----------------------------------------------------------------------------
// TTe2Settings::GetPrivatePath
// ----------------------------------------------------------------------------
//
TInt TTe2Settings::GetPrivatePath( TFileName& aPrivatePath )
	{
	const TInt KPathNameLength = 100;

	TBuf< KPathNameLength > pathWithoutDrive;
	TBuf< KPathNameLength > driveAndPath;
	TBuf< 2 > appDrive;

	// Get application's private path
	//   This is e.g. '\Private\20001978\'
	//   Does not contain drive.
	iFs.PrivatePath( pathWithoutDrive );

	// Extract drive letter into appDrive:
	appDrive.Copy( CEikonEnv::Static()->EikAppUi()->Application()->AppFullName().Left(2));
	
	// Combine drive letter and private path
#if defined( __WINS__ )
    aPrivatePath.Copy( _L( "c:" ) );
#else
	aPrivatePath.Copy( appDrive );
#endif // __WINS__

	aPrivatePath.Append( pathWithoutDrive );

	return KErrNone;
	}

// End of file
