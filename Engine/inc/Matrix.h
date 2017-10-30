/* ====================================================================
 * File: Matirx.h
 * Created: 01/17/09
 * Modified: 01/17/09
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef C_MATRIX_H
#define C_MATRIX_H

// INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <e32def.h>

// FORWARD DECLARATIONS
class CTetriminoBase;
class CMatrixAction;

// CLASS DECLARATION
/**
* CMatrix class
*
* @since S60 3.1
*/
NONSHARABLE_CLASS( CMatrix ) : public CBase
    {
public: // Constructor and destructor
    static CMatrix* NewL();
    ~CMatrix();

public: // New functions
    void Append( CTetriminoBase* aTetrimino );
    TInt Count() const;
    CTetriminoBase* At( TInt aIndex ) const;
    CMatrixAction* MatrixAction();

private: // Constructor
    CMatrix();
    void ConstructL();
    
private: // Member data
    RArray<CTetriminoBase*> iList;
    // Own
    CMatrixAction* iMatrixAction;
    };

#endif // C_MATRIX_H

// End of File
