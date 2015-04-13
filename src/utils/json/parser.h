/* parser.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 12 Apr 2015
   FreeBSD-style copyright and disclaimer apply

   Parser interface.
*/

#pragma once

namespace reflect {
namespace json {


/******************************************************************************/
/* PARSER FN                                                                  */
/******************************************************************************/

template<typename Fn> void object(Reader& reader, const Fn& fn);
template<typename Fn> void array(Reader& reader, const Fn& fn);



} // namespace json
} // namespace reflect
