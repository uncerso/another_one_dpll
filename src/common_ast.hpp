#pragma once
#include "visitors/visitors_fwd.hpp"
#include <unordered_map>
#include <string>
#include <memory>

struct Node {
    virtual ~Node() = default;
    virtual void accept(Visitor&) = 0;
};

using Node_t = std::unique_ptr<Node>;

struct Var : Node {
    size_t id;
    Var(size_t id) : id(id) {}
    void accept(Visitor & v) override { v.visit(*this); }
};

struct Not : Node {
    Node_t expr;
    Not(Node_t && e) : expr(move(e)) {}
    void accept(Visitor & v) override { v.visit(*this); }
};

struct Or : Node {
    Node_t lhs;
    Node_t rhs;
    Or(Node_t && l, Node_t && r) : lhs(move(l)), rhs(move(r)) {}
    void accept(Visitor & v) override { v.visit(*this); }
};

struct And : Node {
    Node_t lhs;
    Node_t rhs;
    And(Node_t && l, Node_t && r) : lhs(move(l)), rhs(move(r)) {}
    void accept(Visitor & v) override { v.visit(*this); }
};

struct CommonAST {
    Node_t root;
    std::unordered_map<std::string, size_t> var_to_id;
};
