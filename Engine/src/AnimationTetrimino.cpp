/* ====================================================================
 * File: AnimationTetrimino.cpp
 * Created: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES
#include <e32base.h> // CPeriodic
#include <e32math.h>
#include "AnimationTetrimino.h"
#include "Matrix.h"
#include "EngineObserver.h"
#include "TetriminoObserver.h"
#include "Block.h"
#include "EngineCommon.h"

// CONSTANTS
const TInt KDefaultInitialLine = -1;
const TInt KAnimationDelayTimeout = 0; // 0 secs.
const TInt KAnimationUpdateTimeout = 150000; // 0.15 secs.


// ======== MEMBER FUNCTIONS ========

// ----------------------------------------------------------------------------
// CAnimationTetrimino::NewL
// ----------------------------------------------------------------------------
//
CAnimationTetrimino* CAnimationTetrimino::NewL( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    {
    CAnimationTetrimino* self = 
        new( ELeave ) CAnimationTetrimino( aMatrix, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::~CAnimationTetrimino
// ----------------------------------------------------------------------------
//
CAnimationTetrimino::~CAnimationTetrimino()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        delete iPeriodic;   
        }
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Play
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::PlayL( CTetriminoBase& aTetrimino )
    {
	if ( !Animation() )
	    {
		// Re-initialize all data.
		Reset();
		
		CTetriminoBase::CloneL( aTetrimino );
		iObserver.StateChangedL( EAnimationTetriminoStarted );
	
		iStatus = EAnimationAscend;
	
		iPeriodic->Cancel();
		// start playing animation
		iPeriodic->Start(
				KAnimationDelayTimeout,
				KAnimationUpdateTimeout,
				TCallBack( TimerCallbackL, this ) );
	    }
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Stop
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::Stop()
    {
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();  
        }
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Resume
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::Resume()
    {
    if ( iPeriodic && iStatus != EAnimationNone )
        {
        iPeriodic->Cancel();
        // start playing animation
        iPeriodic->Start(
                KAnimationDelayTimeout,
                KAnimationUpdateTimeout,
                TCallBack( TimerCallbackL, this ) );
        }
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Animation
// ----------------------------------------------------------------------------
//
TBool CAnimationTetrimino::Animation() const
    {
    return iStatus != EAnimationNone ? ETrue : EFalse;
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Reset
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::Reset()
    {
    CTetriminoBase::Reset();

    iStatus = EAnimationNone;
    iLine = KDefaultInitialLine;
    if ( iPeriodic )
        {  
        iPeriodic->Cancel();
        }
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::UpdateAllL
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::UpdateAllL()
    {
    NotifyAllChangeL( EMatrixActionUpdated );
    iObserver.StateChangedL( EAnimationTetriminoAllUpdated );
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::CAnimationTetrimino
// ----------------------------------------------------------------------------
//
CAnimationTetrimino::CAnimationTetrimino( 
    CMatrix& aMatrix, MTetriminoObserver& aObserver )
    : CTetriminoBase( aMatrix, aObserver ), iLine( KDefaultInitialLine )
    { // no implementation required
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::ConstructL
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::ConstructL()
    {
    CTetriminoBase::ConstructL();

    // Initializations that are not related directly to control framework.
    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::RunL
// ----------------------------------------------------------------------------
//
void CAnimationTetrimino::RunL()
    {
    NotifyAllReset();
    
    switch ( iStatus )
        {
        case EAnimationNone:
            break;

        case EAnimationAscend:
            iLine += 1;
            for ( TInt i = 0; i < KPrimaryMatrixWidth; ++i )
                {
                if ( KErrNotFound == Find( i, iLine ) )
                    {
                    TType type = ( TType )( Math::Random() % EGray );
                    NotifyChangeL( EMatrixActionUpdated, TBlock( i, iLine, type ) );
                    iList->AppendL( TBlock( i, iLine, type ) );
                    }
                }
            if ( iLine == KPrimaryMatrixHeight - 1 )
                {
                iStatus = EAnimationDescend;
                }
            break;

        case EAnimationDescend:
        	{
            TInt index = KErrNotFound;
            for ( TInt i = 0; i < KPrimaryMatrixWidth; ++i )
                {
                index = Find( i, iLine );
                NotifyChangeL( EMatrixActionRemoved, iList->At( index ) );
                iList->Delete( index );
                iList->Compress();
                }
            iLine -= 1;
        	}
            break;

        default:
            break;
        }

    iObserver.StateChangedL( EAnimationTetriminoUpdated );

    /// If the animation line back to its default position,
    /// stop itself and report to engine.
    if ( KDefaultInitialLine == iLine )
        {
        Reset();
        iObserver.StateChangedL( EAnimationTetriminoEnded );
        }    
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::Find
// ----------------------------------------------------------------------------
//
TInt CAnimationTetrimino::Find( TInt aX, TInt aY  )
    {
    TInt index ( KErrNotFound );
    TBlock block;
    for ( TInt i = 0; i < iList->Count(); ++i )
        {
        block = iList->At( i );
        if ( block.iX == aX && block.iY == aY )
            {
            index = i;
            break;
            }
        }
    return index;
    }

// ----------------------------------------------------------------------------
// CAnimationTetrimino::TimerCallbackL
// ----------------------------------------------------------------------------
//
TInt CAnimationTetrimino::TimerCallbackL( TAny* aPtr )
    {
    CAnimationTetrimino* self = static_cast<CAnimationTetrimino*> ( aPtr );
    self->RunL();
    return 0;
    }

// End of File
