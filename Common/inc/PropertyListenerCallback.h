/* ====================================================================
 * File: PropertyListenerCallback.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

#ifndef M_PROPERTYLISTENERCALLBACK_H
#define M_PROPERTYLISTENERCALLBACK_H

/**
* Property listener call back interface. 
*/
class MPropertyListenerCallBack
    {
public:

    /**
    * Notifies that property value has changed. 
    * @param None
    * @return None
    */
    virtual void HandlePropertyChangedL() = 0;

protected:

    // Deny destruction through this class
    virtual ~MPropertyListenerCallBack(){};
    };

#endif//M_PROPERTYLISTENERCALLBACK_H

// End of file
