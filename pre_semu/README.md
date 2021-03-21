## Procedure

1. __Find the range of each mutable statement__: Call `MASS` for mutation of all statements, using only SDL, to later get the positions of each statement by analysing the SDL mutants through diff
2. Call MASS normally to generate the intended mutants files.
3. Use the diff of the original and mutants fo find the expressions and statement mutated and cluster.
4. For each expression, in if, while, for statement condition, use `select` expression to replace the original by all mutants switch. For each statement, or other expression, replace the corresponding expression by a switch statement.
5. For each obtained data aggregate into meta mu file.
