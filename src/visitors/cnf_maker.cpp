#include "cnf_maker.hpp"
using namespace std;

void add_disj(CNF & cnf, pair<size_t, bool> && a, pair<size_t, bool> && b) {
    if (a.first == b.first && a.second != b.second)
        return;
    cnf.push_back({move(a), move(b)});
}

void add_disj(CNF & cnf, pair<size_t, bool> && a, pair<size_t, bool> && b, pair<size_t, bool> && c) {
    bool p1 = a.first == b.first && a.second != b.second;
    bool p2 = a.first == c.first && a.second != c.second;
    bool p3 = c.first == b.first && c.second != b.second;
    if (p1 || p2 || p3)
        return;
    cnf.push_back({move(a), move(b), move(c)});
}


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
    add_disj(cnf, ret_literal.to_neg_pair(), l1.to_pair(), l2.to_pair());
    add_disj(cnf, ret_literal.to_pair(), l1.to_neg_pair());
    add_disj(cnf, ret_literal.to_pair(), l2.to_neg_pair());
}

void CNFMaker::visit(And & v) {
    auto p = next_free_id++;
    v.lhs->accept(*this);
    auto l1 = ret_literal;
    v.rhs->accept(*this);
    auto l2 = ret_literal;
    ret_literal = {p, false};
    add_disj(cnf, ret_literal.to_pair(), l1.to_neg_pair(), l2.to_neg_pair());
    add_disj(cnf, ret_literal.to_neg_pair(), l1.to_pair());
    add_disj(cnf, ret_literal.to_neg_pair(), l2.to_pair());
}
