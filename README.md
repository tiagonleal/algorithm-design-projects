# Algorithm Design Projects

Three projects from *Analysis and Synthesis of Algorithms* (ASA) at Instituto Superior Técnico (2025/26), done in a pair.
Each folder contains the solution and a random instance generator used for complexity benchmarking.

| Project | Technique | Language |
|---|---|---|
| [`p1-interval-dp`](p1-interval-dp) | Interval **dynamic programming**, O(n³): finds the removal order of a chain of amino acids that maximises total released energy, with order reconstruction | C++ |
| [`p2-dag-path-counting`](p2-dag-path-counting) | **Topological sort** (Kahn's algorithm) + path counting (mod *m*) in a DAG from every source node | C++ |
| [`p3-integer-programming`](p3-integer-programming) | **Integer linear programming** with PuLP/GLPK: decides, for each team, whether it can still finish first given the remaining matches | Python |

## Run
```bash
g++ -O3 -std=c++17 p1-interval-dp/solution.cpp -o p1 && ./p1 < input.txt
g++ -O3 -std=c++17 p2-dag-path-counting/solution.cpp -o p2 && ./p2 < input.txt
pip install pulp && python3 p3-integer-programming/solution.py < input.txt
```
