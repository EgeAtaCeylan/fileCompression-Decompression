//MODIFIED BY : EGE ATA CEYLAN
#ifndef hash_Table_h
#define hash_Table_h

#include <vector>
using namespace std;


template <class HashedObj>
class HashTable
{
    public:
       explicit HashTable( const HashedObj & notFound, int size = 101 );
               HashTable( const HashTable & rhs )
                 : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                   arr( rhs.arr ), currentSize( rhs.currentSize ) { }
        
        const HashedObj & find( const HashedObj & x ) const;
        void insert( const HashedObj & x );
        void remove( const HashedObj & x );
        

        enum EntryType { ACTIVE, EMPTY, DELETED };
    
    private:
            struct HashEntry
            {
                HashedObj element;
                EntryType info;
                

                HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                  : element( e ), info( i ) { }
            };
            
            vector<HashEntry> arr;
            int currentSize;
            const HashedObj ITEM_NOT_FOUND;

            bool isActive( int currentPos ) const;
            int findPos( const HashedObj & x ) const;
            void rehash( );
            bool isPrime( int n );
            int nextPrime( int n );
            int hash( const HashedObj & x , int tableSize) const; //CREATED AN HASH FUNCTION IN ORDER TO HASH THE OBJECTS TO BE STORED
            
    
};
template <class HashedObj>
bool HashTable<HashedObj>::isPrime( int n )
{
    if( n == 2 || n == 3 )
            return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

template <class HashedObj>
int HashTable<HashedObj>::nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size )
    : ITEM_NOT_FOUND( notFound ), arr( nextPrime( size ) )
{
}
 

template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
       int i = 0;   // number of collusions
       
       int p = hash( x, arr.size( ) ); // current position

       while( arr[ p ].info != EMPTY && arr[ p ].element != x ) {

           p += ++i;  // IN ORDER TO ACHIEVE LINEAR PROBING PRE-INCREMENT i AND INCREMENT THE p BY i

           if( p >= arr.size( ) )             // perform the mod
               p -= arr.size( );                // if necessary
        }

   return p;
}


template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
       return arr[ currentPos ].info == ACTIVE;
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
    int currentPos = findPos( x );

    if( isActive( currentPos ) )
        arr[ currentPos ].info = DELETED;
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
{
    int currentPos = findPos( x );

    if (isActive( currentPos ))
       return arr[ currentPos ].element;

    return   ITEM_NOT_FOUND;
}

template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x )
{
    // Insert x as active
    int currentPos = findPos( x );

    if( isActive( currentPos ) )
        return;

    arr[ currentPos ] = HashEntry( x, ACTIVE );

    // enlarge the hash table if necessary
    if( ++currentSize >= arr.size( ) / 2 )
        rehash( );
}

template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{
    vector<HashEntry> oldArray = arr;

    // Create new double-sized, empty table
    arr.resize( nextPrime( 2 * oldArray.size( ) ));

    for( int j = 0; j < arr.size( ); j++ )
        arr[ j ].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for( int i = 0; i < oldArray.size( ); i++ )
        if( oldArray[ i ].info == ACTIVE )
            insert( oldArray[ i ].element );
}

//THIS FUNCTION TAKES AN HASHEDO-OBJ AND RETURNS AND INTEGER VALUE WHICH IS THE INITIAL HASH POSITION
template <class HashedObj>
int HashTable<HashedObj>::hash( const HashedObj & x , int tableSize) const
{
    string currentKey = x.key;
    int sum =0;
//  THIS LOOP SUMS UP THE ASCII VALUES OF EACH CHARECTER IN THE STRING
    for (int i =0; i< currentKey.length(); i++)
    {
        sum = sum + currentKey[i];
    }
//  MODULO THE SUM WITH THE TABLE SIZE
    sum = sum % tableSize;
    
//  IF THE IS A OVERFLOW CASE WE SOLVE IT
    if (sum < 0)
    {
        sum = sum + tableSize;
    }
    return sum;
}


#endif /* hash_Table_h */
