#include "parser.hpp"
#include <stack>
#include <stdexcept>
using namespace std;

bool is_op(char c) {
	return c == '&' || c == '|' || c == '!';
}

bool is_unary(char c) {
    return c == '!';
}

int priority(char op) {
    switch (op) {
    case '!': return 2;
    case '&': return 1;
    case '|': return 0;
    default: return -1;
    }
}
 
void process_op(stack<Node_t> & nodes, char op) {
    auto l = move(nodes.top()); nodes.pop();
    if (op == '!') {
        nodes.push(make_unique<Not>(move(l)));
        return;
    }
    auto r = move(nodes.top()); nodes.pop();
    switch (op) {
        case '|':  nodes.push(make_unique<Or>(move(r), move(l)));   break;
        case '&':  nodes.push(make_unique<And>(move(r), move(l)));  break;
    }
}

bool is_var_symbol(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           c == '_';
}

string read_var(string_view s) {
    size_t len = 0;
    while (len < s.size() && is_var_symbol(s[len]))
        ++len;
    return string(s.substr(0, len));
}

CommonAST parse(std::string_view formula) {
    CommonAST ast;
    size_t next_free_id = 0;
    stack<Node_t> nodes;
    stack<char> ops;
    for (size_t i = 0; i < formula.size(); ++i) {
        auto c = formula[i];
        if (isspace(c))
            continue;

        if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                process_op(nodes, ops.top());
                ops.pop();
            }
            if (ops.empty()) 
                throw invalid_argument("There is no '(' for ')' at pos = " + to_string(i));
            ops.pop();
        } else if (is_op(c)) {
            while (!ops.empty() && priority(ops.top()) >= priority(c) && (!is_unary(ops.top()) || !is_unary(c))) {
                process_op(nodes, ops.top());
                ops.pop();
            }
            ops.push(c);
        } else { // prop variable
            auto var = read_var(formula.substr(i));
            if (var.empty())
                throw invalid_argument("Expected variable at pos = " + to_string(i));
            i += (var.size() - 1); // 1 would be added at the iteration end
            size_t var_id;
            if (auto it = ast.var_to_id.find(var); it != ast.var_to_id.end()) {
                var_id = it->second;
            } else {
                var_id = next_free_id;
                ast.var_to_id.emplace(move(var), next_free_id++);
            }
            nodes.push(make_unique<Var>(var_id));
        }
    }

    while (!ops.empty()) {
        process_op(nodes, ops.top());
        ops.pop();
    }
    if (nodes.size() != 1)
        throw invalid_argument("Too much variables, incorrect syntaxis");
    ast.root = move(nodes.top());
    return ast;
}