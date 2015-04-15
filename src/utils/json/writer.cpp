/* writer.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 14 Apr 2015
   FreeBSD-style copyright and disclaimer apply

   Writer implementation details.
*/

namespace reflect {
namespace json {

/******************************************************************************/
/* WRITER                                                                     */
/******************************************************************************/

namespace { std::string spaces{4096, ' '}; }

void
Writer::
seperator(char c)
{
    put(c);
    if (pretty()) put(' ');
}

void
Writer::
newline()
{
    if (!pretty()) return;

    put("\n");
    write(spaces.c_str(), indent_ * 4);
}

} // namespace json
} // namespace reflect
