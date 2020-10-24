#pragma once
#include <list>
#include <unordered_map>

// {var_id -> is_negative_literal}
using Disj = std::unordered_map<size_t, bool>;

using CNF = std::list<Disj>;