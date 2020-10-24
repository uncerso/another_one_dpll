#pragma once
#include "common_ast.hpp"
#include <string_view>

CommonAST parse(std::string_view formula);
