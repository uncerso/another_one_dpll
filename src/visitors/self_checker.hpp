#pragma once
#include "visitors_fwd.hpp"
#include "../common_ast.hpp"
#include <vector>

using Model = std::vector<size_t>;

class SelfChecker : public Visitor {
public:
    void visit(Var&) override;
    void visit(Not&) override;
    void visit(Or&)  override;
    void visit(And&) override;
    bool calc(Node_t const & node, Model const & model);
private:
    bool result;
    Model const * model;
};
