#pragma once
#include "visitors_fwd.hpp"

class Printer : public Visitor {
public:
    void visit(Var&) override;
    void visit(Not&) override;
    void visit(Or&)  override;
    void visit(And&) override;
};
