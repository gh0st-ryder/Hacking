class Solution {
    // The dependencies (recipes/ingredients) for a recipe.
    unordered_map<string, unordered_set<string>> recipe_deps;

    enum Status {UNKNOWN=0, NOT_POSSIBLE, POSSIBLE, UNDER_EVAL};  // UNDER_EVAL is needed to detect stupid cycles... wtf leetcode?
    unordered_map<string, Status> recipe_possible;
    unordered_set<string> pure_supplies;  // to exclude these from the answer

    // Evaluate traverses the graph and checks if making that recipe is possible.
    bool evaluate(const string& recipe) {
        if (recipe_possible.find(recipe) == recipe_possible.end()) return false;
        if (recipe_possible[recipe] == POSSIBLE) return true;
        if (recipe_possible[recipe] == NOT_POSSIBLE) return false;
        if (recipe_possible[recipe] == UNDER_EVAL) return false;  // cycle detected

        recipe_possible[recipe] = UNDER_EVAL;  // mark to detect future cycle
        for (const string& dep : recipe_deps[recipe]) {
            if (!evaluate(dep)) {                
                recipe_possible[recipe] = NOT_POSSIBLE;
                return false;
            }
        }
        recipe_possible[recipe] = POSSIBLE;
        return true;
    }
public:
    vector<string> findAllRecipes(vector<string>& recipes, vector<vector<string>>& ingredients, vector<string>& supplies) {
        for (int i=0; i<recipes.size(); i++) {
            for (const string& dep : ingredients[i]) {
                recipe_deps[recipes[i]].insert(dep);
                recipe_possible[recipes[i]] = UNKNOWN;
            }
        }

        for (const string& supply : supplies) {
            recipe_possible[supply] = POSSIBLE;
            pure_supplies.insert(supply);
        }

        for (const auto& kv : recipe_deps) {
            if (recipe_possible[kv.first] != UNKNOWN) continue;
            evaluate(kv.first);
        }

        vector<string> ans;
        for (const auto& kv : recipe_possible) {
            if (kv.second == POSSIBLE) {
                if (pure_supplies.find(kv.first) != pure_supplies.end()) continue;
                ans.push_back(kv.first);
            }
        }
        return ans;
    }
};
