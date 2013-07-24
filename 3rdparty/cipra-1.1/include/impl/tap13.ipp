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

#ifndef CIPRA_TAP13_IPP
#define CIPRA_TAP13_IPP

#include <string>
#include <iostream>

namespace cipra {
    namespace tap13 {

        /* A note about implementation:
         *
         * Each of these functions returns a details::output object,
         * which is a simple object that contains a
         * details::output_helper pointer.  The details::output object
         * knows how to print its output_helper to a stream.  The
         * exact output is produced by operator() function of the
         * output_helper, which takes the stream as an argument and
         * outputs its own data.
         *
         * The output_helper implementations are local classes in each
         * of these functions.  The functions only declare a new such
         * implementation class, with the proper operator() and create
         * a details::output object using it.
         */

        details::output header() {
            class impl : public details::output_helper {
                std::ostream &operator()(std::ostream &out) {
                    out << "TAP version 13";
                    return out;
                }
            };
            return details::output(new impl);
        }

        details::output plan(int total) {
            class impl : public details::output_helper {
                int total;
                std::ostream &operator()(std::ostream &out) {
                    out << "1.." << total;
                    return out;
                }
            public:
                impl(int t) : total(t) {}
            };
            return details::output(new impl(total));
        }

        details::output diagnostic(std::string text) {
            class impl : public details::output_helper {
                std::string text;
                std::ostream &operator()(std::ostream &out) {
                    out << "# " << text;
                    return out;
                }
            public:
                impl(std::string t) : text(t) {}
            };
            return details::output(new impl(text));
        }

        details::output todo(std::string text) {
            class impl : public details::output_helper {
                std::string text;
                std::ostream &operator()(std::ostream &out) {
                    out << " # TODO " << text;
                    return out;
                }
            public:
                impl(std::string t) : text(t) {}
            };
            return details::output(new impl(text));
        }

        details::output skip(std::string text) {
            class impl : public details::output_helper {
                std::string text;
                std::ostream &operator()(std::ostream &out) {
                    out << " # SKIP " << text;
                    return out;
                }
            public:
                impl(std::string t) : text(t) {}
            };
            return details::output(new impl(text));
        }

        details::output ok(int number, std::string name) {
            class impl : public details::output_helper {
                int number;
                std::string name;
                std::ostream &operator()(std::ostream &out) {
                    out << "ok";
                    if (number != 0)
                        out << " " << number;
                    if (name != "")
                        out << " - " << name;
                    return out;
                }
            public:
                impl(int num, std::string n) : number(num), name(n) {}
            };
            return details::output(new impl(number, name));
        }

        details::output not_ok(int number, std::string name) {
            class impl : public details::output_helper {
                int number;
                std::string name;
                std::ostream &operator()(std::ostream &out) {
                    out << "not ok";
                    if (number != 0)
                        out << " " << number;
                    if (name != "")
                        out << " - " << name;
                    return out;
                }
            public:
                impl(int num, std::string n) : number(num), name(n) {}
            };
            return details::output(new impl(number, name));
        }

        details::output bail_out(std::string why) {
            class impl : public details::output_helper {
                std::string why;
                std::ostream &operator()(std::ostream &out) {
                    out << "Bail out! " << why;
                    return out;
                }
            public:
                impl(std::string w) : why(w) {}
            };
            return details::output(new impl(why));
        }

        namespace details {
            std::ostream &operator<<(std::ostream &out,
                                     details::output tap13) {
                return (*tap13)(out);
            }
        }

    }
}

#endif // #ifdef CIPRA_TAP13_IPP
