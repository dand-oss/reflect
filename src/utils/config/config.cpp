/* config.cpp                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 27 Apr 2014
   FreeBSD-style copyright and disclaimer apply

   Reflection-based configuration implementation.
*/

#include "includes.h"

namespace reflect {
namespace config {

/******************************************************************************/
/* CONFIG                                                                     */
/******************************************************************************/

bool
Config::
count(const Path& path) const
{
    auto it = keys_.find(path.front());
    if (it == keys_.end()) return false;

    if (path.size() == 1) return true;
    return has(it->second, path, 1);
}

Value
Config::
operator[] (const Path& path) const
{
    auto it = keys_.find(path.front());
    if (it == keys_.end())
        reflectError("path <%s> doesn't exist", path.toString());

    if (path.size() == 1) return it->second;;
    return config::get(it->second, path, 1);
}

std::vector<std::string>
Config::
keys() const
{
    std::vector<std::string> result;
    for (auto& key : keys_) result.push_back(key.first);
    return result;
}

void
Config::
propagate(const Path& path, Value value)
{
    auto sublinks = links.get(path)->subtree();

    for (auto& sub : sublinks)
        set(sub.second, value);

    links.erase(path);
}

void
Config::
set(const Path& path, Value value)
{
    auto it = keys_.find(path.front());

    if (it != keys_.end())
        config::set(it->second, path, 1, value);

    else {
        if (path.size() > 1)
            reflectError("unknown path <%s>", path.toString());

        keys_[path.front()] = value;
    }

    propagate(path, value);
}

void
Config::
relink(const Path& link, const Path& target)
{
    if (!links.has(link)) return;

    auto sublinks = links.get(link)->subtree();

    Node* node = links.get(target);
    for (auto& sub : sublinks)
        node->add(sub.second, sub.first);

    links.erase(link);
}

void
Config::
link(const Path& link, const Path& target)
{
    auto it = keys_.find(target.front());
    if (it == keys_.end()) {
        links.add(link, target);
        relink(link, target);
        return;
    }

    Value value = it->second;
    if (target.size() > 1)
        value = config::get(value, target, 1);
    set(link, value);
}


} // namespace config
} // namespace reflect
