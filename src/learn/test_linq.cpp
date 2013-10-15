#include "stdafx.h"
#include "test_linq.h"

/*
 * In VC++ you have to define NOMINMAX in order to avoid including the
 * min/max macros. If you use NuGet, this is automatically defined.
 */
#include "linq/cpplinq.hpp"

int computes_a_sum ()
{
    using namespace cpplinq;    
    int ints[] = {3,1,4,1,5,9,2,6,5,4};

    // Computes the sum of all even numbers in the sequence above
    return 
        from_array (ints)
        >>  where ([](int i) {return i%2 ==0;})     // Keep only even numbers
        >>  sum ()                                  // Sum remaining numbers
        ;
}

std::vector<int> creates_a_vector ()
{
    using namespace cpplinq;    
    int ints[] = {3,1,4,1,5,9,2,6,5,4};

    // Computes the sum of all even numbers in the sequence above
    return 
        from_array (ints)
        >>  where ([](int i) {return i%2 ==0;})     // Keep only even numbers
        >>  to_vector ()                            // Create vector
        ;
}

std::vector<int> creates_a_sorted_vector ()
{
    using namespace cpplinq;    
    int ints[] = {3,1,4,1,5,9,2,6,5,4};

    // Computes the sum of all even numbers in the sequence above
    return 
        from_array (ints)
        >>  where ([](int i) {return i%2 ==0;})         // Keep only even numbers
        >>  orderby_ascending ([](int i) {return i;})   // Sort ascending
        >>  to_vector ()                                // Create vector
        ;
}

void test_linq()
{
    assert(computes_a_sum() == 16);
    auto vec1 = creates_a_vector();
    assert(vec1.size() == 4);
    auto vec2 = creates_a_sorted_vector();
    assert(vec2.size() == 4);
    assert(vec2[0] == 2);
    assert(vec2[3] == 6);
}
