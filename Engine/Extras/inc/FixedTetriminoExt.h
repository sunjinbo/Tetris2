/* ====================================================================
 * File: FixedTetriminoExt.h
 * Created: 01/04/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_FIXEDTETRIMINOEXT_H
#define C_FIXEDTETRIMINOEXT_H

// INCLUDE FILES
#include "FixedTetrimino.h" 
#include "Block.h"
#include "Ticks.h"

// FORWARD DECLARATIONS
class CPeriodic;

// CLASS DECLARATION
/**
* CFixedTetriminoExt class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CFixedTetriminoExt ) : public CFixedTetrimino
                ,public MTicksCallback
    {
public: // Constructor and destructor
    static CFixedTetriminoExt* NewL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CFixedTetriminoExt();
    virtual void MergeL( const CTetriminoBase& aTetrimino );

protected: // from base class CTetriminoBase/CFixedTetrimino
    virtual void RemoveL();
    virtual void Reset();
    virtual void Start();
    virtual void Stop();
    virtual void Resume();

protected: // from MTicksCallback
    virtual void TicksCompletedL();

protected: // New function
    void AutoGrowL();
    void DoAutoGrowL( TInt aLine );
    TType AutoGrowType() const;
    void BombL( const TBlock& aBomb );
    TBool HasBlock( TInt aX, TInt aY ) const;

private: // constructor.
    CFixedTetriminoExt( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    void ConstructL();    

private: // Member data
    // The Timer for auto growing
    CTicks* iTicks;
    TBool iGrowState;
    TBool iBlank;
    };

#endif // C_FIXEDTETRIMINOEXT_H

// End of File
