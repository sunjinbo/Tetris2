/* ====================================================================
 * File: RepositoryObserver.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_REPOSITORYOBSERVER_H
#define M_REPOSITORYOBSERVER_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION
/**
 *  MRepositoryObserver
 *  Repository callbacks
 */
class MRepositoryObserver
    {
public: // New methods
    
    /**
     * Inform about repository event
     *
     * @param aCategory
     * @param aKey the central repository key which value has changed
     */
    virtual void RepositoryValueChangedL( 
						const TUid& /* aCategory */, 
						const TUint32& /* aKey */ ) = 0;
    };

#endif // M_REPOSITORYOBSERVER_H

// End of File
