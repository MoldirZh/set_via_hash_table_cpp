/*
 * set.cpp
 *
 *  Created on: Mar 18, 2019
 *      Author: benja
 */


#include "set.h"

bool set::simp_insert( const std::string& s )
{
    size_t h = hash( s ) % buckets.size(); // find index (initially size is 4)
    std::list< std::string >::iterator it; // iterator

    it = buckets[h].begin();
    while( it != buckets[h].end() )
    {
        if( eq( *it, s ) ) // if s is found in the table
            return false; // do not insert
        else
            ++it;
    }

    buckets[h].push_back( s ); // insert to back of vector
    set_size++; // increment size
    return true;
}

std::ostream& set::print( std::ostream& out ) const
{
    for( size_t i = 0; i < buckets.size(); ++i )
    {
        out << i << " -> ";
        std::list< std::string >::const_iterator it;

        for( it = buckets[i].begin(); it != buckets[i].end(); ++it )
        {
            if( it == buckets[i].begin() )
                out << *it;
            else
                out << ", " << *it;
        }
        out << "\n";
    }
    return out;
}

bool set::contains( const std::string& s ) const
{
    size_t h = hash( s ) % buckets.size(); // index
    std::list< std::string >::const_iterator it; // iterator

    for( it = buckets[h].begin(); it != buckets[h].end(); ++it )
    {
        if( eq( *it, s ) == true) // s is found
            return true;
    }
    return false; // s is not found
}

bool set::remove( const std::string& s )
{
    size_t h = hash( s ) % buckets.size();
    std::list< std::string >::iterator it;

    it = buckets[h].begin();
    while( it != buckets[h].end() )
    {
        if( eq( *it, s ) == true) // s is found, so we need to remove it
            break;
        else
            ++it;
    }
    if ( it != buckets[h].end() ) // if s is found
    {
        buckets[h].erase( it );
        set_size--;
        return true;
    } else
        return false;
}


void set::clear()
{
    buckets.clear();
    set_size = 0;
}

void set::rehash( size_t newbucketsize )
{
    if( newbucketsize < 4 )
        newbucketsize == 4;
    std::vector< std::list< std::string >> newbuckets = std::vector< std::list< std::string >> ( newbucketsize );

    for (size_t i = 0; i < buckets.size(); ++i )
    {
        std::list< std::string >::iterator it;
        for( it = buckets[i].begin(); it != buckets[i].end(); ++it )
        {
            std::string temp = *it; // temporary var that holds the value of current string
            size_t h = hash(temp) % newbuckets.size(); // new index
            newbuckets[h].push_back(temp); // push into newbuckets until the values in buckets end
        }
    }
    std::swap(buckets, newbuckets);
}

bool set::insert( const std::string& s )
{
    if( simp_insert( s ) == true )
    {
        if( loadfactor() >= max_load_factor )
            rehash( 2 * buckets.size() );
        return true;
    } else
        return false;
}

set::set( std::initializer_list<std::string > init ) :
    set_size( 0 ),
    max_load_factor( 3.0 ),
    buckets( std::vector< std::list< std::string >> ( 2 * init.size() ) )
{
    size_t i = 0;
    for( std::string s : init )
    {
        insert( s );
        i++;
    }
}

std::ostream& set::printstatistics( std::ostream& out ) const
{
   out << "set size =            " << size( ) << "\n";
   out << "load factor =         " << loadfactor( ) << " ( max = " << max_load_factor << " )\n";
   out << "standard deviation =  " << standarddev( ) << "\n";
   return out;
}
