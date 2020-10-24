#include "cnf_maker.hpp"
#include <iostream>
using namespace std;

CNF CNFMaker::make_CNF(CommonAST const & ast) {
    cnf = CNF();
    next_free_id = ast.var_to_id.size();
    if (ast.root) {
        ast.root->accept(*this);
        cnf.push_back({ret_literal.to_pair()});
    }
    return move(cnf);
}

void CNFMaker::visit(Var & v) {
    ret_literal.id = v.id;
    ret_literal.is_neg = false;
}

void CNFMaker::visit(Not & v) {
    v.expr->accept(*this);
    ret_literal.is_neg = !ret_literal.is_neg;
}

void CNFMaker::visit(Or & v)  {
    auto p = next_free_id++;
    v.lhs->accept(*this);
    auto l1 = ret_literal;
    v.rhs->accept(*this);
    auto l2 = ret_literal;
    ret_literal = {p, false};
    cnf.push_back({ret_literal.to_neg_pair(), l1.to_pair(), l2.to_pair()});
    cnf.push_back({ret_literal.to_pair(), l1.to_neg_pair()});
    cnf.push_back({ret_literal.to_pair(), l2.to_neg_pair()});
}

void CNFMaker::visit(And & v) {
    auto p = next_free_id++;
    v.lhs->accept(*this);
    auto l1 = ret_literal;
    v.rhs->accept(*this);
    auto l2 = ret_literal;
    ret_literal = {p, false};
    cnf.push_back({ret_literal.to_pair(), l1.to_neg_pair(), l2.to_neg_pair()});
    cnf.push_back({ret_literal.to_neg_pair(), l1.to_pair()});
    cnf.push_back({ret_literal.to_neg_pair(), l2.to_pair()});
}
