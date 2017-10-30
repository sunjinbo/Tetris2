/* ====================================================================
 * File: RemovableTetrimino.cpp
 * Created: 01/13/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32base.h> // CPeriodic
#include "RemovableTetrimino.h"
#include "Matrix.h"
#include "TetriminoObserver.h"
#include "Block.h"
#include "EngineCommon.h"
#include "Te2Engine.hrh"

// CONSTANTS
const TInt KDescendDelayTimeout = 0; // 0.0 secs
const TInt KDefaultDescendSpeedTimeout = 0;     ///< 0.0 secs
const TInt KLevel1DescendSpeedTimeout = 500000; ///< 0.5 secs
const TInt KLevel2DescendSpeedTimeout = 400000; ///< 0.4 secs
const TInt KLevel3DescendSpeedTimeout = 300000; ///< 0.3 secs
const TInt KLevel4DescendSpeedTimeout = 200000; ///< 0.2 secs
const TInt KLevel5DescendSpeedTimeout = 100000; ///< 0.1 secs
const TInt KFastDescendSpeedTimeout = 50000;    ///< 0.05 secs

// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CRemovableTetrimino::NewL
// ----------------------------------------------------------------------------
//
CRemovableTetrimino* CRemovableTetrimino::NewL( CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CRemovableTetrimino* self = 
        new( ELeave ) CRemovableTetrimino( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::~CRemovableTetrimino
// ----------------------------------------------------------------------------
//
CRemovableTetrimino::~CRemovableTetrimino()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        delete iPeriodic;   
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::CloneL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::CloneL( const CTetriminoBase& aTetrimino )
    {
    CTetriminoBase::CloneL( aTetrimino );
    
    iOffsetX = KPrimaryMatrixWidth / 2 - 2;
    iOffsetY = KPrimaryMatrixHeight;
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::Rotate
// Rotates( by 90 degrees clockwise )
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::RotateL()
    {
    NotifyAllChangeL( EMatrixActionRemoved );
    
    TBlock* block = NULL;
    TInt count( iList->Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        block = &iList->At( i );

        block->iY = block->iX + block->iY;
        block->iX = block->iY - block->iX;
        block->iY = 3 - block->iY + block->iX;
        }

    // Append these blocks of tetrimino in which 
    // was changed into the matrix action list.

    // Hit Test and Margin Test
    if ( HitTest( iOffsetX, iOffsetY ) && EdgeTest( iOffsetX, iOffsetY ) )
        {
        NotifyAllChangeL( EMatrixActionMoved );
        iObserver.StateChangedL( ERemovableTetriminoRotated );
        }
    else // Back rotate
        {
        for ( TInt i = 0; i < count; ++i )
            {
            block = &iList->At( i );
            block->iX = block->iY + block->iX;
            block->iY = block->iX - block->iY;
            block->iX = 3 - block->iX + block->iY;
            }
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::DescendL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::DescendL()
    {
    if ( HitTest( iOffsetX, iOffsetY - 1 ) 
        && EdgeTest( iOffsetX, iOffsetY - 1 ) )
        {
        NotifyAllChangeL( EMatrixActionRemoved );
        iOffsetY -= 1;
        NotifyAllChangeL( EMatrixActionMoved );
        iObserver.StateChangedL( ERemovableTetriminoDescend );
        }
    else
        {
        iObserver.StateChangedL( ERemovableTetriminoDescendEnd );
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::FastDescendL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::FastDescendL( TBool aState )
    {
    if ( iFastState != aState )
        {
        iFastState = aState;
        if ( iFastState )
            {
            iDescendSpeed = KFastDescendSpeedTimeout;
            DoStartL();
            }
        else
            {
            StartL( iLevel );
            }
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::DoubleDescendL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::DoubleDescendL()
    {
    while ( HitTest( iOffsetX, iOffsetY - 1 ) 
        && EdgeTest( iOffsetX, iOffsetY - 1 ) )
        {
        NotifyAllChangeL( EMatrixActionRemoved );
        iOffsetY -= 1;
        NotifyAllChangeL( EMatrixActionMoved );
        iObserver.StateChangedL( ERemovableTetriminoDescend );
        }

    iObserver.StateChangedL( ERemovableTetriminoDescendEnd );
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::LeftL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::LeftL()
    {
    if ( HitTest( iOffsetX - 1, iOffsetY ) 
        && EdgeTest( iOffsetX - 1, iOffsetY ) )
        {
        NotifyAllChangeL( EMatrixActionRemoved );
        iOffsetX -= 1;
        NotifyAllChangeL( EMatrixActionMoved );
        iObserver.StateChangedL( ERemovableTetriminoLeft );
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::RightL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::RightL()
    {
    if ( HitTest( iOffsetX + 1, iOffsetY ) 
        && EdgeTest( iOffsetX + 1, iOffsetY ) )
        {
        NotifyAllChangeL( EMatrixActionRemoved );
        iOffsetX += 1;
        NotifyAllChangeL( EMatrixActionMoved );
        iObserver.StateChangedL( ERemovableTetriminoRight );
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::StartL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::StartL( TInt aLevel )
    {
    if ( !iFastState )
        {
        SetLevel( aLevel );
        }
    DoStartL();
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::Stop
// ----------------------------------------------------------------------------
//
 void CRemovableTetrimino::Stop()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();  
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::Resume
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::Resume()
    {
    // Initializations that are not related directly to control framework.
	Stop();
    iPeriodic->Start(
        KDescendDelayTimeout,
        iDescendSpeed,
        TCallBack( TimerCallbackL, this ) ); 
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::UpdateAllL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::UpdateAllL()
    {
    NotifyAllChangeL( EMatrixActionUpdated );
    iObserver.StateChangedL( ERemovableTetriminoAllUpdated );
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::Reset
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::Reset()
    {
    CTetriminoBase::Reset();
    Stop();
    iDescendSpeed = KDefaultDescendSpeedTimeout;
    iFastState = EFalse;
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::CRemovableTetrimino
// ----------------------------------------------------------------------------
//
 CRemovableTetrimino::CRemovableTetrimino( CMatrix& aMatrix, 
                                          MTetriminoObserver& aObserver )
    : CTetriminoBase( aMatrix, aObserver )
    {
    // no implementation required 
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::ConstructL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::ConstructL()
    {
    CTetriminoBase::ConstructL();
    iOffsetX = KPrimaryMatrixWidth / 2 - 2;
    iOffsetY = KPrimaryMatrixHeight;
    iDescendSpeed = KDefaultDescendSpeedTimeout;
    iFastState = EFalse;

    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::SetLevel
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::SetLevel( TInt aLevel )
    {
    iLevel = aLevel;
    // Default descend speed timeout is set to level 1.
    switch( iLevel )
        {
        case ELevel1:
            iDescendSpeed = KLevel1DescendSpeedTimeout;
            break;
        case ELevel2:
            iDescendSpeed = KLevel2DescendSpeedTimeout;
            break;
        case ELevel3:
            iDescendSpeed = KLevel3DescendSpeedTimeout;
            break;
        case ELevel4:
            iDescendSpeed = KLevel4DescendSpeedTimeout;
            break;
        case ELevel5:
            iDescendSpeed = KLevel5DescendSpeedTimeout;
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::DoStartL
// ----------------------------------------------------------------------------
//
void CRemovableTetrimino::DoStartL()
    {
    // If timer is running, stop it fristly.
    Stop();
    iPeriodic->Start(
        KDescendDelayTimeout,
        iDescendSpeed,
        TCallBack( TimerCallbackL, this ) ); 
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::HitTest
// ----------------------------------------------------------------------------
//
TBool CRemovableTetrimino::HitTest( TInt aX, TInt aY )
    {
    TInt count( 0 );
    TBlock block, myBlock;
    for ( TInt i = 0; i < iMatrix.Count(); ++i )
        {
        CTetriminoBase* tetrimino = iMatrix.At( i );
        if ( tetrimino != this )
            {
            count = tetrimino->iList->Count();
            for ( TInt j = 0; j < count; ++j )
                {
                block = tetrimino->iList->At( j );
                block.iX += tetrimino->iOffsetX;
                block.iY += tetrimino->iOffsetY;
                for ( TInt m = 0; m < iList->Count(); ++m )
                    {
                    myBlock = iList->At( m );
                    if ( ( myBlock.iX + aX == block.iX )
                       && ( myBlock.iY + aY == block.iY ) )
                        {
                        return EFalse;
                        }
                    }
                }
            }
        }
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::EdgeTest
// ----------------------------------------------------------------------------
//
TBool CRemovableTetrimino::EdgeTest( TInt aX, TInt aY )
    {
    for ( TInt i = 0; i < iList->Count(); ++i )
        {
        if ( ( iList->At( i ).iX + aX > KPrimaryMatrixWidth - 1 ) 
             || ( iList->At( i ).iX + aX < 0 )
             || ( iList->At( i ).iY + aY < 0 ) )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CRemovableTetrimino::TimerCallbackL
// ----------------------------------------------------------------------------
//
TInt CRemovableTetrimino::TimerCallbackL( TAny* aPtr )
    {
    CRemovableTetrimino* self = static_cast<CRemovableTetrimino*> ( aPtr );
    self->DescendL();
    return 0;
    }

// End of File
