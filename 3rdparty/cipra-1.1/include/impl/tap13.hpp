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

#ifndef CIPRA_TAP13_HPP
#define CIPRA_TAP13_HPP

#include <string>
#include <iostream>
#include <memory>

namespace cipra {
    namespace tap13 {

        /// @cond DETAILS
        /**
         * Implementation of the iostream manipulators for TAP13
         * output.  This should not be documented in the Doxygen
         * output unless the internal flag is set in the Doxyfile.  It
         * usually won't be, but if it is, don't be surprised if this
         * section has minimal documentation.
         */
        namespace details {
            /**
             * A base class for classes that know how to print some
             * TAP13 output to an iostream.
             *
             * @version 1.0
             * @author  Patrick M. Niedzielski
             * @date    2012-09-30
             * @since   1.0
             */
            class output_helper {
            public:
                /**
                 * Prints the TAP13 output to the given std::ostream.
                 *
                 * @author Patrick M. Niedzielski
                 * @date   2012-09-30
                 * @since  1.0
                 */
                virtual std::ostream &operator()(std::ostream &)
                    = 0;
            };
            /**
             * When we return a local class from any of the functions
             * in the cipra::tap13 namespace, we need to `new` it.
             * Use a std::unique_ptr to `delete` it when needed.
             */
            typedef std::unique_ptr<output_helper> output;

            /**
             * Print some TAP13 output to a std::ostream.  This TAP13
             * output will be generated from one of the functions in
             * the cipra::tap13 namespace.
             *
             * @author Patrick M. Niedzielski
             * @date   2012-09-30
             * @since  1.0
             *
             * @return The stream after the TAP13 output was printed.
             */
            inline std::ostream &operator<<(std::ostream &,
                                            output);
        }
        /// @endcond

        /**
         * @name Output functions
         * These functions return opaque objects that can be sent to
         * the test output with the iostream insertion (`operator<<`)
         * operator.  The user does not need to worry about the
         * implementation of the return type; the user can just type,
         * for instance,
         *
         *     std::cout << cipra::tap13::header() << std::endl;
         *
         * which will print `TAP version 13` to `STDOUT`.
         */
        /// @{
        /**
         * Output a TAP13 stream header.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output header();
        /**
         * Output a TAP13 plan.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] total The number of tests you plan on running.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output plan(int total);
        /**
         * Output a TAP13 simple diagnostic.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] text The diagnostic text.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         *
         * @see cipra::tap13::yamlish_diagnostic()
         */
        inline details::output diagnostic(std::string text);
        /**
         * Output a TAP13 YAMLish diagnostic.
         * @todo Implement this.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output yamlish_diagnostic(/* ... */);
        /**
         * Output a TAP13 `TODO` directive after an `ok` or `not ok`
         * line.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] text The explanation of what needs to be done
         * to make the test succeed.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output todo(std::string text);
        /**
         * Output a TAP13 `SKIP` directive after an `ok` or `not ok`
         * line.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] text The explanation of why this test was
         * skipped.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output skip(std::string text);
        /**
         * Output a successful test.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] number The test number.
         * @param [in] name   What the user called this test.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output ok(int number, std::string name);
        /**
         * Output an unsuccessful test.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] number The test number.
         * @param [in] name   What the user called this test.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output not_ok(int number, std::string name);
        /**
         * Output a TAP13 `Bail out!` line.
         *
         * @author Patrick M. Niedzielski
         * @date   2012-09-30
         * @since  1.0
         *
         * @param [in] why The reason for stopping test execution
         * early.
         *
         * @return An opaque object that can be printed on a
         * std::ostream.
         */
        inline details::output bail_out(std::string why);
        /// @}
    }
}

#include "tap13.ipp"

#endif // #ifdef CIPRA_TAP13_HPP
