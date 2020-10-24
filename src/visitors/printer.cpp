#include "printer.hpp"
#include "../common_ast.hpp"
#include <iostream>
using namespace std;

void Printer::visit(Var & v) {
    cout << v.id;
}

void Printer::visit(Not & v) {
    cout << '!';
    v.expr->accept(*this);
}

void Printer::visit(Or & v)  {
    cout << '(';
    v.lhs->accept(*this);
    cout << " | ";
    v.rhs->accept(*this);
    cout << ')';
}

void Printer::visit(And & v) {
    cout << '(';
    v.lhs->accept(*this);
    cout << " & ";
    v.rhs->accept(*this);
    cout << ')';
}
