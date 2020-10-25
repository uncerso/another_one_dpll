#include "self_checker.hpp"
using namespace std;

bool SelfChecker::calc(Node_t const & node, Model const & model) {
    result = true;
    this->model = &model;
    if (node)
        node->accept(*this);
    return result;
}

void SelfChecker::visit(Var & v) {
    result = (*model)[v.id];
}

void SelfChecker::visit(Not & v) {
    v.expr->accept(*this);
    result = !result;
}

void SelfChecker::visit(Or & v)  {
    v.lhs->accept(*this);
    if (!result)
        v.rhs->accept(*this);
}

void SelfChecker::visit(And & v) {
    v.lhs->accept(*this);
    if (result)
        v.rhs->accept(*this);
}
