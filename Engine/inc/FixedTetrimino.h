/* ====================================================================
 * File: FixedTetrimino.h
 * Created: 01/17/09
 * Modified: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_FIXEDTETRIMINO_H
#define C_FIXEDTETRIMINO_H

// INCLUDE FILES
#include "TetriminoBase.h" 


// CLASS DECLARATION
/**
* CFixedTetrimino class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CFixedTetrimino ) : public CTetriminoBase
    {
public:
    /// Creates a new fixed tetrimino object, 
    /// and leaves if there is insufficient memory to create it.
    static CFixedTetrimino* NewL( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual ~CFixedTetrimino();
    virtual void MergeL( const CTetriminoBase& aTetrimino );
    virtual void UpdateAllL();
    virtual void Start(){}
    virtual void Stop(){}
    virtual void Resume(){}

protected: // constructor.
    CFixedTetrimino( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    void ConstructL();

protected: // New functions
    virtual void RemoveL();
    void DoRemoveL( const RArray<TInt>& aLine );
    void EndTestL();
    };

#endif // C_FIXEDTETRIMINO_H

// End of File
