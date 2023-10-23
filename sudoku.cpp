#include <iostream>
#include <vector>

using std::vector;


bool check_sol(const vector<vector<int>>& cnf ,const vector<int>& sol);
bool check_clause(const vector<int>& clause,const vector<int>& sol);
int get_len(const vector<vector<int>>& cnf);
bool sat_aux(vector<vector<int>>& cnf, vector<int>& sol, vector<vector<int>> cmp,int n);
bool is_pure_literal(const vector<vector<int>>& cnf, int a);
void get_small_clause_to_front(vector<vector<int>>& cnf);
void remove_clauses_contain_lit(vector<vector<int>>& cnf, int lit);
void print_cnf(const vector<vector<int>>& cnf);
void print_clause(const vector<int>& clause);
bool remove_lit_from_clauses(vector<vector<int>>& cnf, int lit);
void copy_vec(vector<int>& v1, const vector<int>& v2);
void copy_vec2(vector<vector<int>>& v1, const vector<vector<int>>& v2);
void copy_vec3(vector<int>& v1, const vector<int>& v2);
void copy_vec4(vector<int>& v1, const vector<int>& v2);
void print_sol(const vector<int>& sol);
bool satisfy_clauses_with_one_lit(vector<vector<int>>& cnf, vector<int>& sol);
bool backtrack(vector<vector<int>>& cnf, vector<int>& sol,int n);
bool sat(const vector<vector<int>>& cnf, vector<int>& sol);

//---------------------------------------------------------------

vector<vector<int>> clause_3sat(vector<vector<int>>& sat,vector<int>& sol);
vector<vector<int>> sat_to_3sat(const vector<vector<int>>& sat,vector<int>& sol);
bool equal_clause(const vector<int>& a,const vector<int>& b);

//---------------------------------------------------------------
vector<int> box_value(vector<vector<int>>& cnf, int brow, int bcol , int k);
vector<int> col_value(vector<vector<int>>& cnf,int j, int k);
vector<int> row_value(vector<vector<int>>& cnf,int i, int k);
vector<int> cell_value(vector<vector<int>>& cnf,int i, int k);
void diff_set(vector<vector<int>>& cnf, vector<int> nums);
void set_cell(vector<vector<int>>& cnf,int i, int j);
void set_row(vector<vector<int>>& cnf,int i, int k);
void set_col(vector<vector<int>>& cnf,int j, int k);
void set_box(vector<vector<int>>& cnf,int brow,int bcol, int k);
void create_cnf(vector<vector<int>>& cnf);

//-----------------------------------------------------------------
bool board_input(vector<vector<int>>& cnf, vector<int>& sol);
void print_sodoku_sol(vector<int>& sol);
bool apply_sol(vector<vector<int>>& cnf, vector<int>& sol);
int steps = 0;

int main() {
    /*
sudoku board example: (0 means empty cell)
0 0 0 2 6 0 7 0 1
6 8 0 0 7 0 0 9 0
1 9 0 0 0 4 5 0 0
8 2 0 1 0 0 0 4 0
0 0 4 6 0 2 9 0 0
0 5 0 0 0 3 0 2 8
0 0 9 3 0 0 0 7 4
0 4 0 0 5 0 0 3 6
7 0 3 0 1 8 0 0 0

    */

    /*
     (0,0) (0,1) (0,2)   (0,3) (0,4) (0,5)   (0,6) (0,7) (0,8)
     (1,0) (1,1) (1,2)   (1,3) (1,4) (1,5)   (1,6) (1,7) (1,8)
     (2,0) (2,1) (2,2)   (2,3) (2,4) (2,5)   (2,6) (2,7) (2,8)

     (3,0) (3,1) (3,2)   (3,3) (3,4) (3,5)   (3,6) (3,7) (3,8)
     (4,0) (4,1) (4,2)   (4,3) (4,4) (4,5)   (4,6) (4,7) (4,8)
     (5,0) (5,1) (5,2)   (5,3) (5,4) (5,5)   (5,6) (5,7) (5,8)

     (6,0) (6,1) (6,2)   (6,3) (6,4) (6,5)   (6,6) (6,7) (6,8)
     (7,0) (7,1) (7,2)   (7,3) (7,4) (7,5)   (7,6) (7,7) (7,8)
     (8,0) (8,1) (8,2)   (8,3) (8,4) (8,5)   (8,6) (8,7) (8,8)


     * */
    clock_t start = clock();
    vector<vector<int>> aa;
    create_cnf(aa);
    vector<int> tmp,sol(get_len(sat_to_3sat(aa,tmp))+1);
    std::fill(sol.begin(), sol.end(), 0);
    if(!board_input(aa,sol)) {
        std::cout<<"input error"<<std::endl;
    }
    if(sat(aa,sol)) {
        print_sodoku_sol(sol);
    }
    else std::cout<<"Unsolvable sudoku"<<std::endl;
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    //std::cout<<"steps: "<<steps<<std::endl;


    return 0;
}
//--------------------------------------------------------------------
//sudoko
bool board_input(vector<vector<int>>& cnf,vector<int>& sol){
    int value,lit;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cin >> value;
            if(value > 0){
                lit = ((value-1)*81)+(i*9)+j+1;
                sol[lit] = 1;
                remove_clauses_contain_lit(cnf,lit);
                if(!remove_lit_from_clauses(cnf,-lit))
                    return false;
                for (int k = 0; k < 9; ++k) {
                    if(k == (value-1))
                        continue;
                    lit = (81*k)+(i*9)+j+1;
                    sol[lit] = -1;
                    remove_clauses_contain_lit(cnf,-lit);
                    if(!remove_lit_from_clauses(cnf,lit))
                        return false;
                }
            }
        }
    }

    return satisfy_clauses_with_one_lit(cnf,sol);
}

void print_sodoku_sol(vector<int>& sol){
    int board[9][9];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            board[i][j] = 0;
        }
    }
    for (int k = 0; k < 9; ++k) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if(sol[(81*k)+(9*i)+j+1] == 1){
                    board[i][j] = k+1;
                }
            }
        }
    }
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout<<board[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

bool apply_sol(vector<vector<int>>& cnf, vector<int>& sol){
    for (int k = 0; k < 9; ++k) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                int lit = (81*k)+(9*i)+j+1;
                if(sol[lit] == 1){
                    remove_clauses_contain_lit(cnf,lit);
                    if(!remove_lit_from_clauses(cnf,-lit))
                        return false;
                }
                if(sol[lit] == -1){
                    remove_clauses_contain_lit(cnf,-lit);
                    if(!remove_lit_from_clauses(cnf,lit))
                        return false;
                }
            }
        }
    }
    return satisfy_clauses_with_one_lit(cnf,sol);
}

//--------------------------------------------------------------------
//create the board formula

bool equal_clause(const vector<int>& a,const vector<int>& b){
    if(a.size() != b.size())
        return false;
    for (int i = 0; i < a.size(); ++i) {
        bool check = false;
        for (int j = 0; j < a.size(); ++j) {
            if(a[i]==b[j]){
                check = true;
                break;
            }
        }
        if(!check)
            return false;
    }
    return true;
}


void create_cnf(vector<vector<int>>& cnf){
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            set_row(cnf,i,k);
            set_cell(cnf,i,k);
        }
    }
    for (int j = 0; j < 9; ++j) {
        for (int k = 0; k < 9; ++k) {
            set_col(cnf,j,k);
        }
    }
    for (int k = 0; k < 9; ++k) {
        for (int brow = 0; brow < 3; ++brow) {
            for (int bcol = 0; bcol < 3; ++bcol) {
                set_box(cnf, brow, bcol, k);
            }
        }
    }
}
vector<int> row_value(vector<vector<int>>& cnf,int i, int k){
    vector<int> to_add;
    for (int j = 0; j < 9; ++j) {
        to_add.push_back((81*k)+(9*i)+j+1);
    }
    cnf.push_back(to_add);
    return to_add;
}
vector<int> col_value(vector<vector<int>>& cnf,int j, int k){
    vector<int> to_add;
    for (int i = 0; i < 9; ++i) {
        to_add.push_back((81*k)+(9*i)+j+1);
    }
    cnf.push_back(to_add);
    return to_add;
}
vector<int> box_value(vector<vector<int>>& cnf, int brow, int bcol , int k){
    vector<int> to_add;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j){
            to_add.push_back((81*k)+(9*(i+(brow*3)))+(j+(bcol*3))+1);
        }
    }
    cnf.push_back(to_add);
    return to_add;
}
vector<int> cell_value(vector<vector<int>>& cnf,int i, int j){
    vector<int> to_add;
    for (int k = 0; k < 9; ++k) {
        to_add.push_back((81*k)+(9*i)+j+1);
    }
    cnf.push_back(to_add);
    return to_add;

}
void set_cell(vector<vector<int>>& cnf,int i, int j){
    diff_set(cnf,cell_value(cnf,i,j));
}
void set_row(vector<vector<int>>& cnf,int i, int k){
    diff_set(cnf,row_value(cnf,i,k));
}
void set_col(vector<vector<int>>& cnf,int j, int k){
    diff_set(cnf,col_value(cnf,j,k));
}
void set_box(vector<vector<int>>& cnf,int brow,int bcol, int k){
    diff_set(cnf,box_value(cnf,brow,bcol,k));
}
void diff_set(vector<vector<int>>& cnf, vector<int> nums){
    vector<int> tmp;
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = 0; j < i; ++j){
            tmp = {-nums[j],-nums[i]};
            cnf.push_back(tmp);
        }
    }
}

//--------------------------------------------------------------------
//sat convert to 3sat
vector<vector<int>> clause_3sat(vector<vector<int>>& sat,vector<int>& sol){
    vector<vector<int>> sat3(sat[0].size()-2);
    int k = 0,i = sol.size();
    sol.push_back(0);
    sat3[0].push_back(sat[0][k++]);
    sat3[0].push_back(sat[0][k++]);
    sat3[0].push_back(i);
    for (int j = 1; j < sat3.size()-1; ++j) {
        sol.push_back(0);
        sat3[j].push_back(-i);
        sat3[j].push_back(sat[0][k++]);
        sat3[j].push_back(++i);
    }
    sat3[sat3.size()-1].push_back(-i);
    sat3[sat3.size()-1].push_back(sat[0][k++]);
    sat3[sat3.size()-1].push_back(sat[0][k++]);
    return sat3;

}
vector<vector<int>> sat_to_3sat(const vector<vector<int>>& sat,vector<int>& sol){
    vector<vector<int>> res;
    for(auto& clause : sat){
        if(clause.size() < 4){
            res.push_back(clause);
        }
        else {
            vector<vector<int>> tmp;
            tmp.push_back(clause);
            tmp = clause_3sat(tmp,sol);
            for (const auto& nclause: tmp) {
                res.push_back(nclause);
            }
        }
    }
    return res;
}

//--------------------------------------------------------------------
//3sat solution:
bool sat(const vector<vector<int>>& cnf, vector<int>& sol){
    int n = sol.size();
    vector<vector<int>> tmp = sat_to_3sat(cnf,sol);
    return sat_aux(tmp,sol,cnf,n);
}
bool sat_aux(vector<vector<int>>& cnf, vector<int>& sol,vector<vector<int>> cmp,int n){
    ++steps;
    //if the CNF has no clauses then it can be satisfied.
    if(cnf.empty())
        return true;

    //lit is the literal that we are currently working with.
    int lit = cnf[0][0];
    if(sol[abs(lit)] != 0) {
        return false;
    }

    sol.resize(n);
    if(!apply_sol(cmp,sol))
        return false;
    if(!satisfy_clauses_with_one_lit(cmp,sol))
        return false;
    if(check_sol(cmp,sol))
        return true;
    cnf = sat_to_3sat(cmp,sol);

    if(cnf[0].size() == 1){
        remove_clauses_contain_lit(cnf,lit);
        if(!remove_lit_from_clauses(cnf,-lit))
            return false;
        sol[abs(lit)] = abs(lit)/lit;
        return sat_aux(cnf, sol, cmp,n);
    }

    //if lit is pure literal; the literal only appears with the same sign in all occurrences, satisfy it and
    //remove all caluses that have it (they are all satisfied), and repeat with new CNF.
    if(is_pure_literal(cnf,lit)){
        remove_clauses_contain_lit(cnf,lit);
        sol[abs(lit)] = abs(lit)/lit;
        return sat_aux(cnf, sol, cmp,n);
    }


    /**
     * explanation: for every literal that we satisfy (if it appears in negation form, given -1. and if it appears
     * in positive form given 1.) all clauses that contain this literal with the same sign are removed from the CNF
     * because all those clauses are satisfied automatically by that literal.
     * for the clauses that contain the negative form of the literal, the literal is removed from the clause as its
     * not satisfied, and we have to satisfy the clause using the other literals in the clause.
     * both of those actions are made using the 'remove_lit_from_clauses()' function which is given the '-lit' as
     * argument to remove the negative form of lit from all clauses. and using 'remove_clauses_contain_lit()'
     * function which is given lit as argument so that every clause that contain lit is satisfied and we can remove
     * it from the CNF and find a way to satisfy remaining clauses.
     * */

    //means that lit appears with different signs in multiple clauses.
    else {

        if(cnf.size() > 0 && cnf[0].size() == 2){
            int lit1 = cnf[0][1];
            bool check0;
            vector<vector<int>> v0(cnf.size());
            vector<int> s0;
            copy_vec(s0, sol);
            copy_vec2(v0, cnf);
            //satisfy lit
            remove_clauses_contain_lit(v0, lit);
            check0 = remove_lit_from_clauses(v0, -lit);
            s0[abs(lit)] = abs(lit) / lit;
            get_small_clause_to_front(v0);

            if (check0 && sat_aux(v0, s0, cmp,n)) {
                if(check_sol(cmp,s0)) {
                    copy_vec3(sol, s0);
                    return true;
                }
            }

            bool check01;
            vector<vector<int>> v01(cnf.size());
            vector<int> s01;
            copy_vec(s01, sol);
            copy_vec2(v01, cnf);

            //dis-satisfy lit
            remove_clauses_contain_lit(v01, -lit);
            check01 = remove_lit_from_clauses(v01, lit);
            s01[abs(lit)] = -abs(lit) / lit;
            get_small_clause_to_front(v01);

            if(check01) {
                remove_clauses_contain_lit(v01, lit1);
                check01 = remove_lit_from_clauses(v01, -lit1);
                s01[abs(lit1)] = abs(lit1) / lit1;
            }
            if (check01 && sat_aux(v01, s01, cmp,n)) {
                if(check_sol(cmp,s01)) {
                    copy_vec3(sol, s01);
                    return true;
                }
            }

        }
        if (cnf.size() > 1) {
            bool check1 = true, check2 = true;

            vector<vector<int>> v1(cnf.size());
            vector<int> s1;
            copy_vec(s1, sol);
            copy_vec2(v1, cnf);

            //satisfy lit
            remove_clauses_contain_lit(v1, lit);
            check1 = remove_lit_from_clauses(v1, -lit);
            s1[abs(lit)] = abs(lit) / lit;
            get_small_clause_to_front(v1);

            vector<vector<int>> v2(v1.size());
            vector<int> s2;
            copy_vec(s2, s1);
            copy_vec2(v2, v1);
            check2 = check1;

            //satisfy the firs literal in the second clause.(which turns to be the first clause after removing
            //the clause that contained lit).
            int lit1,lit9;
            if (check1 && !v1.empty() && !v1[0].empty()) {
                lit1 = v1[0][0];

                //satisfy the first literal of the second clause.
                remove_clauses_contain_lit(v1, lit1);
                check1 = remove_lit_from_clauses(v1, -lit1);
                s1[abs(lit1)] = abs(lit1) / lit1;

                get_small_clause_to_front(v1);
                //call recursively with the updated solution
                if (check1 && sat_aux(v1, s1, cmp,n)) {
                    if(check_sol(cmp,s1)) {
                        copy_vec3(sol, s1);
                        return true;
                    }
                }

                //dis-satisfy the first literal of the second clause.
                if(check2 && !v2.empty() &&v2[0].size() > 1)
                    {
                    lit9 = v2[0][1];
                    //dis-satisfy the first literal of the second clause.
                    remove_clauses_contain_lit(v2, -lit1);
                    check2 = remove_lit_from_clauses(v2, lit1);
                    s2[abs(lit1)] = -abs(lit1) / lit1;
                    if(check2) {
                        //satisfy the second literal of the second clause.
                        remove_clauses_contain_lit(v2, lit9);
                        check2 = remove_lit_from_clauses(v2, -lit9);
                        s2[abs(lit9)] = abs(lit9) / lit9;
                    }
                    //call recursively with the updated solution
                    if (check2 && sat_aux(v2, s2, cmp,n)) {
                        if(check_sol(cmp,s2)) {
                            copy_vec3(sol, s2);
                            return true;
                        }
                    }

                }
            }

        }

        if (cnf[0].size() > 1) {
            bool check3 = true;
            vector<vector<int>> v3(cnf.size());
            vector<int> s3;
            copy_vec(s3, sol);
            copy_vec2(v3, cnf);
            int lit1 = cnf[0][1];

            //dis-satisfy lit.
            remove_clauses_contain_lit(v3, -lit);
            check3 = remove_lit_from_clauses(v3, lit);
            s3[abs(lit)] = -abs(lit) / lit;

            if (check3 && !v3[0].empty()) {
                remove_clauses_contain_lit(v3, lit1);
                check3 = remove_lit_from_clauses(v3, -lit1);
                s3[abs(lit1)] = abs(lit1) / lit1;

                //call recursively with the updated solution
                if (check3 && sat_aux(v3, s3, cmp,n)) {
                    if(check_sol(cmp,s3)) {
                        copy_vec3(sol, s3);
                        return true;
                    }
                }
            }
        }

        if (cnf[0].size() > 2) {
            bool check4 = true;
            vector<vector<int>> v4(cnf.size());
            vector<int> s4;
            copy_vec(s4, sol);
            copy_vec2(v4, cnf);

            int lit1 = cnf[0][1],lit2 = cnf[0][2];
            //dis-satisfy lit.
            remove_clauses_contain_lit(v4, -lit);
            check4 = remove_lit_from_clauses(v4, lit);
            s4[abs(lit)] = -abs(lit) / lit;

            //dis-satisfy the second literal.
            if (check4 && !v4[0].empty()) {
                remove_clauses_contain_lit(v4, -lit1);
                check4 = remove_lit_from_clauses(v4, lit1);
                s4[abs(lit1)] = -abs(lit1) / lit1;

            }

            if (check4 && !v4[0].empty()) {
                remove_clauses_contain_lit(v4, lit2);
                check4 = remove_lit_from_clauses(v4, -lit2);
                s4[abs(lit2)] = abs(lit2) / lit2;

                //call recursively with the updated solution
                if (check4 && sat_aux(v4, s4, cmp,n)) {
                    if(check_sol(cmp,s4)) {
                        copy_vec3(sol, s4);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool satisfy_clauses_with_one_lit(vector<vector<int>>& cnf, vector<int>& sol){
    int lit,m=0;
    vector<int> clause;
    while (cnf.begin()+m != cnf.end()) {
        clause = *(cnf.begin()+m);
        if(clause.empty())
            return false;
        if(clause.size() == 1){
            lit = clause[0];
            if(sol[abs(lit)] != 0 )
                return false;
            remove_clauses_contain_lit(cnf, lit);
            if(!remove_lit_from_clauses(cnf, -lit)) {
                return false;
            }
            sol[abs(lit)] = abs(lit) / lit;
            m = -1;
        }
        ++m;
    }
    return true;
}

bool backtrack(vector<vector<int>>& cnf, vector<int>& sol,int n){
    if(!n) {
        if(check_sol(cnf,sol)){
            print_sol(sol);
            return true;
        }
        else return false;
    }
    vector<int> sol1;
    copy_vec(sol1,sol);
    if(sol[n-1])
        return backtrack(cnf,sol,n-2);
    int k,i,j;
    k = (n-1)/81;//5
    i = ((n-1)%81)/9;//2
    j = (((n-1)%81)%9)-1;//1
    if(j<0){
        j+=9;
        --i;
    }
    if(i<0){
        i+=81;
        i%9;
        --k;
    }

    sol[n-1] = 1;
    sol1[n-1] = -1;
    for (int k1 = 0; k1 < 9 && k != k1; ++k1) {
        sol[(81*k)+(9*i)+j+1] = -1;
    }
    return backtrack(cnf,sol,n-1) || backtrack(cnf,sol1,n-1);
}

//remove the literal lit from every clause in the CNF.
bool remove_lit_from_clauses(vector<vector<int>>& cnf, int lit){
    int j = 0;
    while (cnf.begin()+j != cnf.end()){
        auto& clause = *(cnf.begin()+j);
        int i = 0;
        while (clause.begin()+i != clause.end()){
            if(*(clause.begin()+i) == lit){
                if(clause.size() == 1)
                    return false;
                clause.erase(clause.begin()+i);
                break;
            }
            ++i;
        }
        ++j;
    }
    return true;
}
//removes all clauses that contain the lit literal.
void remove_clauses_contain_lit(vector<vector<int>>& cnf, int lit){
    int i = 0;
    while (i < cnf.size()){
        for (int j = 0; j < cnf[i].size(); ++j){
            if(cnf[i][j] == lit){
                cnf.erase(cnf.begin()+i);
                --i;
                break;
            }
        }
        ++i;
    }
}
void get_small_clause_to_front(vector<vector<int>>& cnf){
    vector<int> to_swap;
    if (!cnf.empty() && cnf[0].size() > 2){
        int j = 0;
        while (cnf.begin()+j != cnf.end()) {
            auto& clause = *(cnf.begin()+j);
            if(clause.size() < 3) {
                copy_vec(to_swap, clause);
                cnf.erase(cnf.begin() + j);
                cnf.insert(cnf.begin(), to_swap);
                break;
            }
            ++j;
        }
    }
}

//checks if the literal only appears with the same sign in all clauses that appears in.
bool is_pure_literal(const vector<vector<int>>& cnf, int a) {
    for (auto &clause: cnf) {
        for (auto &lit: clause) {
            if(lit == -a)
                return false;
        }
    }
    return true;
}
//gets the number of literals in the CNF.
int get_len(const vector<vector<int>>& cnf){
    int max = 0;
    for (auto &clause : cnf){
        for (auto &lit : clause){
            if(lit > max)
                max = lit;
        }
    }
    return max+1;
}
//checks if the solution is valid.
bool check_sol(const vector<vector<int>>& cnf ,const vector<int>& sol){
    for (auto &item : cnf){
        if(!check_clause(item,sol))
            return false;
    }
    return true;
}
//checks if the clause is satisfied.
bool check_clause(const vector<int>& clause,const vector<int>& sol){

    for (auto &lit : clause){
        if(lit > 0 && sol[lit] > 0)
            return true;
        if(lit < 0 && sol[-lit] < 0)
            return true;
    }
    return false;
}
void print_cnf(const vector<vector<int>>& cnf){
    bool first = false;
    for (auto &clause: cnf) {
        if(!first)
            std::cout << "{";
        else std::cout << " , {";
        first = true;
        for (auto &lit: clause) {
            if(first){
                std::cout << lit;
                first = false;
            }
            else std::cout << " , " << lit ;
        }
        first = true;
        std::cout << "}";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
void print_clause(const vector<int>& clause){
    std::cout<<"{";
    for (int i = 0; i < clause.size(); ++i) {
        if(i == clause.size()-1)
            std::cout<<clause[i];
        else std::cout<<clause[i]<<", ";
    }
    std::cout<<"}"<<std::endl;
}

void print_sol(const vector<int>& sol){
    bool check = true;
    std::cout<<"[";
    for (int i = 1; i < sol.size(); ++i) {
        if(check)
            std::cout<< sol[i];
        else std::cout<< " , " <<sol[i];
        check = false;
    }
    std::cout<<"]"<<std::endl;
    std::cout << std::endl;
}
void copy_vec(vector<int>& v1, const vector<int>& v2){
    for(int i = 0; i < v2.size(); ++i){
        v1.push_back(v2[i]);
    }
}
void copy_vec2(vector<vector<int>>& v1, const vector<vector<int>>& v2){
    for(int i = 0; i < v2.size(); ++i){
        copy_vec(v1[i],v2[i]);
    }
}
void copy_vec3(vector<int>& v1, const vector<int>& v2){
    for(int i = 0; i < v2.size(); ++i){
        v1[i] = v2[i];
    }
}
void copy_vec4(vector<int>& v1, const vector<int>& v2){
    for(int i = 0; i < v1.size(); ++i){
        v1[i] = v2[i];
    }
}