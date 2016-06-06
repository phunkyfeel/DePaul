/*-------------------------------------------------------------------------*
 *---									---*
 *---		NumberList.h						---*
 *---									---*
 *---	    This file declares and defines a class that manages a list	---*
 *---	of instances of type 'double'.  It allows computes the product	---*
 *---	of the list.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2016 May 18		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<list>

//  PURPOSE:  To keep track of a list of numbers.
class	NumberList
{
    //  I.  Member vars:
    //  PURPOSE:  To keep track of the list of numbers:
    std::list<double>		list_;
    
    //  II.  Disallowed auto-generated methods:
    
    protected :
    //  III.  Protected:
    
    public :
    //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
    //  PURPOSE:  To make '*this' into an empty list.  No return value.
    NumberList			() :
				list_()
				{ }
    
    //  PURPOSE:  To make '*this' into a copy of 'rhs'.  No return value.
    NumberList			(const NumberList&	rhs
                         ) :
				list_(rhs.list_)
				{ }
    
    //  PURPOSE:  To release the resources of '*this', make it into a copy of
    //	'rhs', and to return a reference to '*this'.
    NumberList	operator=	(const NumberList&	rhs
                             )
				{
                    if  (this == &rhs)
                        return(*this);
                    
                    list_	= rhs.list_;
                    return(*this);
                }
    
    //  V.  Accessors:
    
    //  VI.  Mutators:
    
    //  VII.  Methods that do main and misc work of class:
    //  PURPOSE:  To put 'number' into '*this' list.  No return value.
    void		putIn  		(double			number
                             )
				{
                    list_.push_front(number);
                }
    
    //  PURPOSE:  To compute and return the product of the numbers in '*this'.
    //	No parameters.
    double	product		()
    const
    {
        double		product	= 1.0;
        std::list<double>::const_iterator
        iter	= list_.begin();
        std::list<double>::const_iterator
        end	= list_.end();
        
        for  ( ;  iter != end;  iter++)
            product *= *iter;
        
        return(product);
				}
    
};



