/* ====================================================================
 * File: TetriminoBase.h
 * Created: 01/17/09
 * Modified:01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */
 
#ifndef C_TETRIMINOBASE_H
#define C_TETRIMINOBASE_H

// INCLUDE FILES
#include <e32base.h>
#include <e32def.h>
#include "Block.h"
#include "EngineAction.h"

// FORWARD DECLARATIONS
class TBlock;
class CMatrix;
class MTetriminoObserver;


// CLASS DECLARATION
/**
* CTetriminoBase class
*
* @since S60 3.1
*/
class CTetriminoBase : public CBase
    {
public: // Friend class
    friend class CTetriminoBase;

public: // Destructor
    virtual ~CTetriminoBase();

public: // New functions
    virtual void CloneL( const CTetriminoBase& aTetrimino );
    virtual void Reset();
    virtual void UpdateAllL() = 0;

protected: // Constrctor
    CTetriminoBase( CMatrix& aMatrix, MTetriminoObserver& aObserver );
    virtual void ConstructL();

protected: // New functions
    void NotifyChangeL( TMatrixActionType aType, TBlock aBlock );
    void NotifyAllChangeL( TMatrixActionType aType );
    void NotifyAllReset();

public: // Memeber data
    /**
    The relative x co-ordinate
    */
    TInt iOffsetX;

    /**
    The relative y co-ordinate
    */
    TInt iOffsetY;

    /**
    The block data of Tetrimino
    Own
    */
    CArrayFixFlat<TBlock>* iList;

    /**
    The matrix is that Tetrimino object is belong to.
    Not own
    */
    CMatrix& iMatrix;

    /**
    The observer of Tetrimino object
    Not own
    */
    MTetriminoObserver& iObserver;
    };

#endif // C_TETRIMINOBASE_H

// End of File
