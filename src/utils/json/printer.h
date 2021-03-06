/* printer.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 14 Apr 2015
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

namespace reflect {
namespace json {

/******************************************************************************/
/* GENERIC PRINTER                                                            */
/******************************************************************************/

void printNull(Writer& writer);
void printBool(Writer& writer, bool value);
void printInt(Writer& writer, int64_t value);
void printFloat(Writer& writer, double value);
void printString(Writer& writer, const std::string& value);

void printBool(Writer& writer, const Value& value);
void printInt(Writer& writer, const Value& value);
void printFloat(Writer& writer, const Value& value);
void printString(Writer& writer, const Value& value);

template<typename Keys, typename Fn>
void printObject(Writer& writer, const Keys& keys, const Fn& printFn);

template<typename Keys, typename PrintFn, typename SkipFn>
void printObject(Writer& writer, const Keys& keys, const PrintFn& printFn, const SkipFn& skipFn);

template<typename Fn>
void printArray(Writer& writer, size_t n, const Fn& printFn);

template<typename PrintFn, typename SkipFn>
void printArray(Writer& writer, size_t n, const PrintFn& printFn, const SkipFn& skipFn);


/******************************************************************************/
/* VALUE PRINTER                                                              */
/******************************************************************************/

void print(Writer& writer, const Value& value);
template<typename T> Error print(Writer& writer, const T& value);
template<typename T> Error print(std::ostream& stream, const T& value);
template<typename T> std::pair<std::string, Error> print(const T& value);

} // namespace json
} // namespace reflect
