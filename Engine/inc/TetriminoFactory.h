/* ====================================================================
 * File: TetriminoFactory.h
 * Created: 01/05/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TETRIMINOFACTORY_H
#define C_TETRIMINOFACTORY_H

//  INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CFixedTetrimino;
class CNextRemovableTetrimino;
class CRemovableTetrimino;
class CAnimationTetrimino;
class CMatrix;
class MTetriminoObserver;

// CLASS DECLARATION
/**
*  Factory class for creating Tetrimion classes.
*
*  @lib Te2Engine.lib
*  @since Series60 3.2
*/
class CTetriminoFactory : public CBase
    {
public: // Constructor and destructor
    static CTetriminoFactory* NewL();
    virtual ~CTetriminoFactory();

public: // New functions
    /**
    * Creates fixed tetrimino object.
    * @return pointer to class, ownership is transferred.
    */
    virtual CFixedTetrimino* CreateFixedTetriminoL(
        CMatrix& aMatrix, MTetriminoObserver& aObserver );

    /**
    * Creates fixed tetrimino object.
    * @return pointer to class, ownership is transferred.
    */
    virtual CNextRemovableTetrimino* CreateNextRemovableTetriminoL( 
        CMatrix& aMatrix, MTetriminoObserver& aObserver );

    /**
    * Creates fixed tetrimino object.
    * @return pointer to class, ownership is transferred.
    */
    virtual CRemovableTetrimino* CreateRemovableTetriminoL(
        CMatrix& aMatrix, MTetriminoObserver& aObserver );

    /**
    * Creates fixed tetrimino object.
    * @return pointer to class, ownership is transferred.
    */
    virtual CAnimationTetrimino* CreateAnimationTetriminoL(
        CMatrix& aMatrix, MTetriminoObserver& aObserver );

protected: // Constructor
    CTetriminoFactory();
    virtual void ConstructL();
    };

#endif // C_TETRIMINOFACTORY_H

// End of File
