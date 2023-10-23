sudoku solver: the input is given in format of 81 (9x9) numbers which represent a sudoku board, where 0 in cell indicates empty cell.
the solver uses conversion of the sudoku problem into 3-sat problem, uses my own algorithm to solve 3-sat problems that gets up to O(1.6^n)
which beats the brute force solver that gets to O(2^n) time complexity. the problem is solved in 3-sat and the solution is re-converted into sudoku's world 
and prnits the solution for the board. 