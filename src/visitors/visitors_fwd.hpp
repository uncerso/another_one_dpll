#pragma once
#include <stddef.h>

struct Var;
struct Not;
struct Or;
struct And;

class Visitor {
public:
    virtual void visit(Var&) = 0;
    virtual void visit(Not&) = 0;
    virtual void visit(Or&)  = 0;
    virtual void visit(And&) = 0;
    virtual ~Visitor() = default;
};
