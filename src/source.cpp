#include "parser.hpp"
#include "visitors/printer.hpp"
#include "visitors/cnf_maker.hpp"
#include <iostream>
#include <string>
using namespace std;

int main() {
    string formula;
    getline(cin, formula);
    try {
        auto common_ast = parse(formula);
        Printer pr;
        common_ast.root->accept(pr);
        cout << endl;
        CNFMaker cm;
        auto cnf = cm.make_CNF(common_ast);
        for (auto const & conj : cnf) {
            for (auto const & disj : conj)
                cout << (disj.second ? "-" : "") << disj.first << ' ';
            cout << '\n';
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
