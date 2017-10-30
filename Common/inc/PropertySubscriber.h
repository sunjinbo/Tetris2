/* ====================================================================
 * File: PropertySubscriber.h
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */


#ifndef C_PROPERTYSUBSCRIBER_H
#define C_PROPERTYSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class MPropertyObserver;

// CLASS DECLARATION
/**
 *  CPropertySubscriber is an acitve object that subscribes to a single
 *  Publish and Subscribe Property and notifies about the Property changes.
 */
class CPropertySubscriber : public CActive
    {
public:

	/**
    * Two-phased constructor.
    *
    * @param aPropertyObserver The object to be notified about 
    *      Property changes
    * @param aCategory The category of the Property to be observed
    * @param aKey The subkey of the Property to be observed
    */
	IMPORT_C static CPropertySubscriber* NewL( 
									MPropertyObserver* aPropertyObserver, 
									const TUid& aCategory, 
									TUint aKey );

	/**
    * Destructor.
    */
	IMPORT_C virtual ~CPropertySubscriber();

public: // new functions

	/**
    * Subscribes to the Property given in constructor
    */
	IMPORT_C void Subscribe();

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TInt& aValue );

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TDesC8& aValue );                     

	/**
    * Set value
    *
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( const TDesC16& aValue );      

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TInt& aValue );

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TDes8& aValue );

	/**
    * Get value
    *
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( TDes16& aValue );

private:

	/**
    * C++ default constructor.
    *
    * @param aPropertyObserver The object to be notified about 
    *      Property changes
    * @param aCategory The category of the Property to be observed
    * @param aKey The subkey of the Property to be observed
    */
	CPropertySubscriber( 
				MPropertyObserver* aPropertyObserver, 
				const TUid& aCategory, 
				TUint aKey );

	/**
    * By default Symbian 2nd phase constructor is private.
    */
	void ConstructL();

private: // from CActive

	/**
    *   RunL
    */
	void RunL();

	/**
    * RunError
    *
    * @param aError the error returned
    * @return error
    */
	TInt RunError( TInt aError );

	/**
    * DoCancel
    */
	void DoCancel();

private: // data

	// The object to be notified about Property changes
	MPropertyObserver* iPropertyObserver;

	// The property object that subscribes to the Property to be observed
	RProperty iProperty;

	// The category of the Property to be observed
	TUid iCategory;

	// The subkey of the Property to be observed
	TUint iKey;
    };

#endif // C_PROPERTYSUBSCRIBER_H

// End of File
