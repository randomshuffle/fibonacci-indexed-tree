## Fibonacci Indexed Tree
This is a new data structure to calculate prefix sums of a sequence of integers. The data structure is an alternative to the Binary Indexed Tree, and works on the basis of Zeckendorf’s Theorem, a property of Fibonacci numbers. The new data structure has a faster Query Operation but a slower Update Operation. This trade-off is a favourable one for most practical purposes due to reasons discussed in this [paper](https://arxiv.org/abs/1612.09083). The fit.cpp file describes the data structure itself and the fit-vs-bit.cpp file benchmarks the query and the update operation on the two data structures. 
