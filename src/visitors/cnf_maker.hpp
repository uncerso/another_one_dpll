#pragma once
#include "visitors_fwd.hpp"
#include "../common_ast.hpp"
#include "../cnf.hpp"

class CNFMaker : public Visitor {
public:
    void visit(Var&) override;
    void visit(Not&) override;
    void visit(Or&)  override;
    void visit(And&) override;
    CNF make_CNF(CommonAST const & ast);
private:
    CNF cnf;
    CommonAST * ast;

    struct {
        size_t id;
        bool is_neg;
        std::pair<size_t, bool> to_pair() const noexcept { return {id, is_neg}; }
        std::pair<size_t, bool> to_neg_pair() const noexcept { return {id, !is_neg}; }
    } ret_literal;

    size_t next_free_id;
};
