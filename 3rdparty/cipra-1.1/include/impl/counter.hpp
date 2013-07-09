// -*- c++ -*-
/* This file is a part of the cipra Unit Testing Framework.
 * Copyright (C) 2012, Patrick M. Niedzielski.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  3. The names of its contributors may not be used to endorse or
 *     promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CIPRA_COUNTER_HPP
#define CIPRA_COUNTER_HPP

#include <type_traits>
#include <atomic>

namespace cipra {

    /**
     * Manages the test numbers in test output.  Guarantees that each
     * test has a different number, even if multiple threads are
     * testing at the same time.
     *
     * @version 1.0
     * @author  Patrick M. Niedzielski
     * @date    2012-09-26
     * @since   1.0
     *
     * @tparam integralT The type that the test numbers should have.
     * This must be a POD integer type.  The actual return value will
     * be the unsigned counterpart of this type if it is not already
     * unsigned.
     */
    template <typename integralT = int>
    class counter {
    public:
        static_assert(std::is_integral<integralT>::value,
                      "Counter type is not integral.");

        /// The index type of the counter.  The actual index type is
        /// going to be the unsigned counterpart of the template
        /// parameter `T`.
        typedef typename std::make_unsigned<integralT>::type
            index_type;

        /**
         * Initializes the atomic counter.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-26
         * @since  1.0
         */
        inline counter();

        /**
         * Returns a unique test number.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-26
         * @since  1.0
         *
         * @return A number that corresponds to the order in which the
         * test was run.
         */
        inline index_type new_test_number();

    private:
        /// The current test count, atomic for thread safety.
        std::atomic<index_type> count;
    };
}

#include "counter.ipp"

#endif // #ifdef CIPRA_COUNTER_HPP
