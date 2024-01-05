In this final project, we take a take a look at how we can try to optimize our Minisat to work for inputs with Vertices <= 50. Usual minisat solver works till Vertices=15 but beyond that the problem can take hours to solve complex graphs. We took an approach to reduce the graph size instead and bring it under Vertices=15.
Careful optimizations brought us closer to this result. Report Attached.
=> Reducing number of clauses to be considered:
   -> Prune Single Noded Edges.
   -> Find Triangle Patterns and Optimize them. 
   -> Find Hexagonal Patterns and Optimize them.
=> Scaling down Vertex Values to reduce clauses.
