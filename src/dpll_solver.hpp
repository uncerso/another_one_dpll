#pragma once
#include "cnf.hpp"
#include <optional>
#include <vector>

using Model = std::vector<size_t>;

std::optional<Model> solve(CNF & cnf, size_t total_vars);