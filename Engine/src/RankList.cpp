/* ====================================================================
 * File: RankList.cpp
 * Created: 09/10/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <f32file.h>
#include <s32file.h>
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>
#include "RankList.h"
#include "EnginePanic.h"

#include "Logger.h"

// CONSTANTS
_LIT( KTe2RankFileName, "Te2Rank.txt" );

const TInt64 KDefaultTime = 0;
const TInt KDefaultScore = 0;

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
// CRankList::NewL
// ----------------------------------------------------------------------------
//
CRankList* CRankList::NewL()
    {
    CRankList* self = new( ELeave ) CRankList;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CRankList::~CRankList
// ----------------------------------------------------------------------------
//
CRankList::~CRankList()
    {
    iRankArray.Close();
    }

// ---------------------------------------------------------------------------
// CRankList::InternalizedL()
// ---------------------------------------------------------------------------
//
void CRankList::InternalizedL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    while ( file.Open( iFs, fileName, EFileRead ) != KErrNone )
        {
        CreateDefaultRankL();
        }

    RFileReadStream inStream( file );
    CleanupClosePushL( inStream );

    for ( TInt i = 0; i < KMaxRankItemNbr; i++ )
        {
        TDateTime dateTime;
        dateTime.SetYear( (TInt)inStream.ReadInt32L() );
        dateTime.SetMonth( (TMonth)inStream.ReadInt32L() );
        dateTime.SetDay( (TInt)inStream.ReadInt32L() );
        dateTime.SetHour( (TInt)inStream.ReadInt32L() );
        dateTime.SetMinute( (TInt)inStream.ReadInt32L() );
        dateTime.SetSecond( (TInt)inStream.ReadInt32L() );

        TTime time( dateTime );

        TInt32 score = KDefaultScore;
        score = inStream.ReadInt32L();
        iRankArray.Append( TRankItem( time, score ) );
        }

    CleanupStack::PopAndDestroy( &inStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// CRankList::ExternalizedL()
// ---------------------------------------------------------------------------
//
void CRankList::ExternalizedL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    while ( file.Open( iFs, fileName, EFileWrite ) != KErrNone )
        {
        CreateDefaultRankL();
        }

    RFileWriteStream outStream( file );
    CleanupClosePushL( outStream );

    for ( TInt i = 0; i < KMaxRankItemNbr && i < iRankArray.Count(); i++ )
        {
        TDateTime dateTime = iRankArray[ i ].iTime.DateTime();
        outStream.WriteInt32L( (TInt32)( dateTime.Year() ) );
        outStream.WriteInt32L( (TInt32)( dateTime.Month() ) );
        outStream.WriteInt32L( (TInt32)( dateTime.Day() ) );
        outStream.WriteInt32L( (TInt32)( dateTime.Hour() ) );
        outStream.WriteInt32L( (TInt32)( dateTime.Minute() ) );
        outStream.WriteInt32L( (TInt32)( dateTime.Second() ) );
        outStream.WriteInt32L( (TInt32)( iRankArray[ i ].iScore ) );
        }

    outStream.CommitL();
    CleanupStack::PopAndDestroy( &outStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// CRankList::ClearL()
// ---------------------------------------------------------------------------
//
void CRankList::ClearL()
    {
    iRankArray.Reset();

    // Creates 5 default data to rank list
    for ( TInt i = 0; i < KMaxRankItemNbr; i++ )
        {
        TRankItem item;
        item.iTime = TTime( KDefaultTime );
        item.iScore = KDefaultScore;
        iRankArray.Append( item );
        }
    ExternalizedL();
    }

// ---------------------------------------------------------------------------
// CRankList::Sort()
// ---------------------------------------------------------------------------
//
void CRankList::Sort()
    {
    TLinearOrder<TRankItem> order( TRankItem::Compare );
    iRankArray.Sort( order );
    }

// ---------------------------------------------------------------------------
// CRankList::GetRankItem()
// ---------------------------------------------------------------------------
//
TInt CRankList::GetRankItem( TInt aRankIx, TRankItem& aRankItem  )
    {
    TInt err = KErrNone;

    // The number one is from zero in rank list,
    // so minus one here.
    aRankIx -= 1;

    if ( aRankIx >= 0 && aRankIx < iRankArray.Count() )
        {
        if ( ( iRankArray[ aRankIx ].iTime != KDefaultTime )
            && ( iRankArray[ aRankIx ].iTime != KDefaultScore ) )
            {
            aRankItem = iRankArray[ aRankIx ];
            }
        else
            {
            err = KErrNotFound;
            }
        }
    else
        {
        err = KErrArgument;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CRankList::AppendRankItem()
// ---------------------------------------------------------------------------
//
void CRankList::AppendRankItem( const TRankItem& aRankItem )
    {
	iRankArray.Append( aRankItem );
	Sort();
    }

// ---------------------------------------------------------------------------
// CRankList::CreateDefaultRankL()
// ---------------------------------------------------------------------------
//
void CRankList::CreateDefaultRankL()
    {
    RFile file;
    TBuf<KMaxFileName> fileName;
    TInt err = GetFileName( fileName );
    User::LeaveIfError( err );
    User::LeaveIfError( file.Create( iFs, fileName, EFileWrite ) );
    RFileWriteStream outStream( file );
    CleanupClosePushL( outStream );

    for ( TInt i = 0; i < KMaxRankItemNbr; i++ )
        {
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // year
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // month
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // day
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // hour
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // minute
		outStream.WriteInt32L( (TInt32)( KDefaultTime ) ); // second
        outStream.WriteInt32L( (TInt32)( KDefaultScore ) ); // score
        }

    outStream.CommitL();
    CleanupStack::PopAndDestroy( &outStream );
    file.Close();
    }

// ---------------------------------------------------------------------------
// CRankList::CRankList()
// ---------------------------------------------------------------------------
//
CRankList::CRankList() : iFs( CCoeEnv::Static()->FsSession() )
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CRankList::ConstructL()
// ---------------------------------------------------------------------------
//
void CRankList::ConstructL()
    {
	InternalizedL();

	Sort();
    }


// ---------------------------------------------------------------------------
// CRankList::GetFileName()
// ---------------------------------------------------------------------------
//
TInt CRankList::GetFileName( TFileName& aFileName )
    {
	TInt err = GetPrivatePath( aFileName );
    if ( err == KErrNone )
    	{
        aFileName.Append( KTe2RankFileName );
    	}
    return err;
    }

// ----------------------------------------------------------------------------
// CRankList::GetPrivatePath
// ----------------------------------------------------------------------------
//
TInt CRankList::GetPrivatePath( TFileName& aPrivatePath )
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
