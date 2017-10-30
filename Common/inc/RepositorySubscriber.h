/* ====================================================================
 * File: RepositorySubscriber.cpp
 * Created: 07/06/10
 * Modified: 07/06/10
 * Author: Sun Jinbo
 * Copyright (c): Tieto, All rights reserved
 * ==================================================================== */

// INCLUDE FILES

#ifndef C_REPOSITORYSUBSCRIBER_H
#define C_REPOSITORYSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "RepositoryObserver.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  CRepositorySubscriber
 *  Repository subscriber
 */
class CRepositorySubscriber : 
    public CBase,
    public MRepositoryObserver
    {
public: // Constructors, Destructor
    
	/**
    * Two-phased constructor
    *
    * @param aCategory
    * @param aKey
    * @param aType
    * @param aObserver Observer
    * @return Instance of CRepositorySubscriber
    */
	IMPORT_C static CRepositorySubscriber* NewL( 
								const TUid& aCategory,
								MRepositoryObserver* aObserver );

	/**
    * Destructor
    */
	IMPORT_C virtual ~CRepositorySubscriber();

public: // New methods
    
	/**
    * Set repository event observer
    *
    * @param aObserver Observer         
    */
	IMPORT_C void SetObserver( MRepositoryObserver* aObserver );

	/**
    * Set value
    *
    * @param aIndex Index
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( 
				const TUint32& aIndex, 
				const TInt& aValue );
		       
	/**
    * Set value
    *
    * @param aIndex Index
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( 
				const TUint32& aIndex, 
				const TDesC8& aValue );                     
	
	/**
    * Set value
    *
    * @param aIndex Index
    * @param aValue New value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt SetValue( 
				const TUint32& aIndex, 
				const TDesC16& aValue );      

	/**
    * Get value
    *
    * @param aIndex Index
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( 
				const TUint32& aIndex, 
				TInt& aValue );
		       
	/**
    * Get value
    *
    * @param aIndex Index
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue( 
				const TUint32& aIndex, 
				TDes8& aValue );
		       
	/**
    * Get value
    *
    * @param aIndex Index
    * @param aValue Value
    * @return KErrNone, or system wide error code
    */
	IMPORT_C TInt GetValue(
				const TUint32& aIndex, 
				TDes16& aValue );

	/**
    * Subscribe aKey
    *
    * @param aKey key for which we subscribe
    */
	IMPORT_C void SubscribeL( const TUint32& aKey );
	
	/**
    * Unsubscribe aKey
    *
    * @param aKey key for which we unsubscribe
    */
	IMPORT_C void Unsubscribe( const TUint32& aKey );   
        
private: // from MRepositoryObserver
    
	/**
    * Inform about repository event
    *
    * @param aCategory
    * @param aKey
    */    
	void RepositoryValueChangedL( 
							const TUid& /* aCategory */, 
							const TUint32& aKey );
    

private: // Constructors, Destructor
    
	/**
    * Constructor
    */
	CRepositorySubscriber( 
						const TUid& aCaterogy,
						MRepositoryObserver* aObserver );

	/**
    * Constructor
    */
	void ConstructL();

private: // Internal class
    
	class CCRepositorySubscriberKey
		: public CActive
		{
		public: // Constructor, destructor

			static CCRepositorySubscriberKey* NewL( 
										const TUint32& aKey,
										MRepositoryObserver& aObserver,
										CRepository& aCR );
										
			~CCRepositorySubscriberKey();
			
		public:
			
			TUint32 Key();

		private: // from CActive
		
			void RunL();
			void DoCancel();
		
		private: // New methods
		
			void Subscribe();
		
		private: // Constructors
		
			void ConstructL();
			CCRepositorySubscriberKey( 
									const TUint32& aKey,
									MRepositoryObserver& aObserver,
									CRepository& aCR );
									
		private: // Data
		
			//
			const TUint32 iKey;
			
			//
			MRepositoryObserver& iObserver;
			
			//
			CRepository& iCR;
		};

private: // member data

    // Category
    const TUid iCategory;

    // Observer, not owned
    MRepositoryObserver* iObserver;
    
    // Property, owned
    CRepository* iCR;
    
    // array of subscribed keys, owned
    CArrayPtr<CCRepositorySubscriberKey>* iKeyArray;
    };

#endif // C_REPOSITORYSUBSCRIBER_H

// End of File
