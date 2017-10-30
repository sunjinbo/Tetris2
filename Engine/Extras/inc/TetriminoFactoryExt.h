/* ====================================================================
 * File: TetriminoFactoryExt.h
 * Created: 01/05/09
 * Modified:
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_TETRIMINOFACTORYEXT_H
#define C_TETRIMINOFACTORYEXT_H

//  INCLUDE FILES
#include <e32base.h>
#include "TetriminoFactory.h"


// CLASS DECLARATION
/**
*  Factory class for creating Tetrimion classes.
*
*  @lib Te2Engine.lib
*  @since Series60 3.2
*/
class CTetriminoFactoryExt : public CTetriminoFactory
    {
public: // Constructor and destructor
    static CTetriminoFactoryExt* NewL();
    virtual ~CTetriminoFactoryExt();

public: // from base class CTetriminoFactory.
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
    CTetriminoFactoryExt();
    virtual void ConstructL();
    };

#endif // C_TETRIMINOFACTORYEXT_H

// End of File
