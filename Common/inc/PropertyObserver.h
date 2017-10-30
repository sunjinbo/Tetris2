/* ====================================================================
 * File: PropertyObserver.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_PROPERTYOBSERVER_H
#define M_PROPERTYOBSERVER_H

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class MPropertyObserver
    {

// New functions
public:

    /**
     * Through this method the Property subscribers notify of 
     * Property changes
     *
     * @param aCategory The category of the property to be observed
     * @param aKey The subkey of the property to be observed
     */
    virtual void HandlePropertyChangedL( 
                                    const TUid& aCategory, 
                                    TInt aKey ) = 0;
};

#endif //M_PROPERTYOBSERVER_H

// End of file

