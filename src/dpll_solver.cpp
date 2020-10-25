#include "dpll_solver.hpp"
#include <cassert>
using namespace std;

namespace {

using Literal = Disj::value_type;

class CNFManipulator {
public:
    CNFManipulator(CNF & cnf) : cnf(cnf) {}

    bool PropagateUnits(Model & model, Literal  literal);
    void EliminatePureLiterals(Model & model);
    void Rollback();
    CNF & cnf;
private:
    CNF removed_disjs;
    unordered_map<Disj*, vector<Disj::node_type>> removed_literals;
    CNF::iterator RemoveDisj(CNF::iterator const & it);
};

bool solve(CNF & cnf, Model & model, Literal forced_literal) {
    CNFManipulator S(cnf);
    if (S.PropagateUnits(model, forced_literal)) {
        S.Rollback();
        return false;
    }
    S.EliminatePureLiterals(model);
    if (cnf.empty())
        return true;
    auto literal = *cnf.front().begin();
    if (solve(cnf, model, literal))
        return true;
    literal.second = !literal.second;
    if (solve(cnf, model, literal))
        return true;
    S.Rollback();
    return false;
}

CNF::iterator CNFManipulator::RemoveDisj(CNF::iterator const & it) {
    auto next_pos = next(it);
    removed_disjs.splice(removed_disjs.end(), cnf, it);
    return next_pos;
}

bool CNFManipulator::PropagateUnits(Model & model, Literal  literal) {
    Disj units = {literal};
    model[literal.first] = !literal.second;
    while (!units.empty()) {
        Disj new_units;
        auto it = cnf.begin();
        while (it != cnf.end()) {
            bool remove_disj = false;
            for (auto const & unit : units) {
                auto var = it->find(unit.first);
                if (var == it->end())
                    continue;
                if (var->second != unit.second) {
                    removed_literals[&*it].push_back(it->extract(var));
                    continue;
                }
                remove_disj = true;
                break;
            }
            if (remove_disj) {
                it = RemoveDisj(it);
                continue;
            }
            if (it->empty())
                return true;
            if (it->size() == 1) {
                auto &new_unit = *it->begin();
                auto [other, inserted] = new_units.insert(new_unit);
                if (!inserted && other->second != new_unit.second)
                    return true;
                model[new_unit.first] = !new_unit.second;
                it = RemoveDisj(it);
                continue;
            }
            ++it;
        }
        units = move(new_units);
    }
    return false;
}

bool contains(Disj const & disj, vector<size_t> const & vars) {
    for (auto id : vars) {
        if (disj.find(id) != disj.end())
            return true;
    }
    return false;
} 

void CNFManipulator::EliminatePureLiterals(Model & model) {
    unordered_map<size_t, pair<bool, bool>> vars; // {id -> (used_as_neg, is_pure)}
    for (auto const & disj : cnf) {
        for (auto const & lit : disj) {
            auto it = vars.find(lit.first);
            if (it == vars.end()) {
                vars.emplace(lit.first, make_pair(lit.second, true));
            } else {
                it->second.second &= (lit.second == it->second.first);
            }
        }
    }

    vector<size_t> pure_lits;
    for (auto const & var : vars) {
        if (!var.second.second)
            continue;
        pure_lits.push_back(var.first);
        model[var.first] = !var.second.first;
    }
    if (pure_lits.empty())
        return;

    auto it = cnf.begin();
    while (it != cnf.end()) {
        if (contains(*it, pure_lits))
            it = RemoveDisj(it);
        else
            ++it;
    }
}

void CNFManipulator::Rollback() {
    cnf.splice(cnf.end(), removed_disjs);
    for (auto & literals : removed_literals) {
        for (auto & literal : literals.second)
            literals.first->insert(move(literal));
    }
}


} // namespace

std::optional<Model> solve(CNF & cnf, size_t total_vars) {
    Model model(total_vars, false);
    if (cnf.empty())
        return {move(model)};
    assert(cnf.back().size() == 1);
    if (solve(cnf, model, *cnf.back().begin()))
        return {move(model)};
    return {};
}