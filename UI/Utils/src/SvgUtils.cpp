/* ====================================================================
 * File: SvgUtils.cpp
 * Created: 03/11/09
 * Modified: 03/14/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <akniconutils.h>
#include <fbs.h>
#include "SvgUtils.h"

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CSvgUtils::NewL
//
// ----------------------------------------------------------------------------
//
EXPORT_C CSvgUtils* CSvgUtils::NewL( const TDesC& aFileName, TInt aFrom, TInt aTo )
    {
    CSvgUtils* self = new(ELeave) CSvgUtils( aFrom, aTo );
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CSvgUtils::~CSvgUtils
//
// ----------------------------------------------------------------------------
//
EXPORT_C CSvgUtils::~CSvgUtils()
    {
    iBitmapArray.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// CSvgUtils::SetSize
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CSvgUtils::SetSize( TInt aIndex, TSize aSize )
    {
    CFbsBitmap* bitmap( Bitmap( aIndex ) );
    CFbsBitmap* mask( Bitmap( aIndex + 1 ) );

    if ( bitmap && mask )
        {
        AknIconUtils::SetSize( bitmap, aSize, EAspectRatioNotPreserved );    
        AknIconUtils::SetSize( mask, aSize, EAspectRatioNotPreserved );
        }
    }

// ----------------------------------------------------------------------------
// CSvgUtils::Rotate
//
// ----------------------------------------------------------------------------
//
EXPORT_C void CSvgUtils::SetSizeAndRotation( TInt aIndex, TSize aSize, TInt aAngle )
    {
    CFbsBitmap* bitmap( Bitmap( aIndex ) );
    CFbsBitmap* mask( Bitmap( aIndex + 1 ) );

    if ( bitmap && mask )
        {
        AknIconUtils::SetSizeAndRotation ( 
            bitmap, aSize, EAspectRatioNotPreserved, aAngle );
        AknIconUtils::SetSizeAndRotation ( 
            mask, aSize, EAspectRatioNotPreserved, aAngle );
        }
    }

// ----------------------------------------------------------------------------
// CSvgUtils::Bitmap
//
// ----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CSvgUtils::Bitmap( TInt aIndex ) const
    {
    TInt index = aIndex - iFrom;
    return iBitmapArray[ index ];
    }

// ----------------------------------------------------------------------------
// CSvgUtils::CSvgUtils
//
// ----------------------------------------------------------------------------
//
CSvgUtils::CSvgUtils( TInt aFrom, TInt aTo )
    : CBmpUtils( aFrom, aTo )
    { // no implementation required.
    }

// ----------------------------------------------------------------------------
// CSvgUtils::CSvgUtils
//
// ----------------------------------------------------------------------------
//
void CSvgUtils::ConstructL( const TDesC& aFileName )
    {
    CBmpUtils::ConstructL( aFileName );
    }

// ----------------------------------------------------------------------------
// CSvgUtils::LoadBitmapL
//
// ----------------------------------------------------------------------------
//
void CSvgUtils::LoadBitmapL ()
    {
    for (TInt index = iFrom ; index < iTo ; index += 2 )
        {
        CFbsBitmap* bitmap( NULL );
        CFbsBitmap* mask( NULL );
        AknIconUtils::CreateIconL( bitmap, mask, *iFileName, index, index + 1 );
        iBitmapArray.Append( bitmap );
        iBitmapArray.Append( mask );
        }
    }

// End of File
