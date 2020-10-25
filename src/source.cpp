#include "parser.hpp"
#include "visitors/printer.hpp"
#include "visitors/cnf_maker.hpp"
#include "visitors/self_checker.hpp"
#include "dpll_solver.hpp"
#include <iostream>
#include <string>
#define NDEBUG
using namespace std;

int main() {
    string formula;
    getline(cin, formula);
    try {
        auto common_ast = parse(formula);
        CNFMaker cm;
        auto cnf = cm.make_CNF(common_ast);

#ifndef NDEBUG
        Printer pr;
        common_ast.root->accept(pr);
        cout << endl;
        for (auto const & conj : cnf) {
            for (auto const & disj : conj)
                cout << (disj.second ? "-" : "") << disj.first << ' ';
            cout << '\n';
        }
#endif
        auto model = solve(cnf, cm.get_total_vars_in_cnf());
        if (!model.has_value()) {
            cout << "UNSAT\n";
        } else {
            cout << "SAT\n";
            auto const & md = *model;

#ifndef NDEBUG
            for (size_t i = 0; i < md.size(); ++i)
                cout << i << " : " << md[i] << '\n';
            for (auto const & x : common_ast.var_to_id)
                cout << x.first << '[' << x.second << ']' << " = " << md[x.second] << '\n';
#else
            for (auto const & x : common_ast.var_to_id)
                cout << x.first << " = " << md[x.second] << '\n';
#endif
            SelfChecker sc;
            if (!sc.calc(common_ast.root, md))
                throw std::logic_error("SelfChecker: unsat model is detected :(");
        }
    } catch (exception const & e) {
        cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Unknown error :(\n";
        return 2;
    }
    return 0;
}
