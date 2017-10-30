/* ====================================================================
 * File: BmpUtils.cpp
 * Created: 03/14/09
 * Modified: 03/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akniconutils.h>
#include <fbs.h>
#include "BmpUtils.h"


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CBmpUtils::NewL
//
// ----------------------------------------------------------------------------
//
EXPORT_C CBmpUtils* CBmpUtils::NewL( const TDesC& aFileName, TInt aFrom, TInt aTo  )
    {
    CBmpUtils* self = new(ELeave) CBmpUtils( aFrom, aTo );
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CBmpUtils::~CBmpUtils
//
// ----------------------------------------------------------------------------
//
EXPORT_C CBmpUtils::~CBmpUtils()
    {
    iBitmapArray.ResetAndDestroy();
    delete iFileName;
    }

// ----------------------------------------------------------------------------
// CBmpUtils::SetSize
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CBmpUtils::SetSize( TInt aIndex, TSize aSize )
    {
    CFbsBitmap* bitmap = Bitmap( aIndex );
    if ( bitmap )
        {
        AknIconUtils::SetSize( bitmap, aSize, EAspectRatioNotPreserved );    
        }
    }

// ----------------------------------------------------------------------------
// CBmpUtils::Rotate
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CBmpUtils::SetSizeAndRotation( TInt aIndex, TSize aSize, TInt aAngle )
    {
    CFbsBitmap* bitmap( Bitmap( aIndex ) );
    
    AknIconUtils::SetSizeAndRotation ( 
        bitmap, aSize, EAspectRatioNotPreserved, aAngle );
    }

// ----------------------------------------------------------------------------
// CBmpUtils::Bitmap
//
// ----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CBmpUtils::Bitmap( TInt aIndex ) const
    {
    TInt index = ( aIndex - iFrom ) / 2;
    return iBitmapArray[ index ];
    }

// ----------------------------------------------------------------------------
// CBmpUtils::CBmpUtils
//
// ----------------------------------------------------------------------------
//
CBmpUtils::CBmpUtils( TInt aFrom, TInt aTo  )
    : iFrom( aFrom ), iTo( aTo )
    { // no implementation required.
    }

// ----------------------------------------------------------------------------
// CBmpUtils::ConstructL
//
// ----------------------------------------------------------------------------
//
void CBmpUtils::ConstructL( const TDesC& aFileName )
    {
    iFileName = HBufC::NewL( aFileName.Length() );
    TPtr fileName( iFileName->Des() );
    fileName.Append( aFileName );
    LoadBitmapL();
    }

// ----------------------------------------------------------------------------
// CBmpUtils::LoadBitmapL
//
// ----------------------------------------------------------------------------
//
void CBmpUtils::LoadBitmapL ()
    {
    for (TInt index = iFrom ; index < iTo ; index += 2 )
        {
        CFbsBitmap* bitmap = AknIconUtils::CreateIconL( *iFileName, index );
        iBitmapArray.Append( bitmap );
        }
    }

// End of File
