# Concept

* For the time being, simplifying things, we have an array of integers which need to tuned.
* Use simple cross-over and mutation, with favoritism.
* Using Roulette's method of selection

# [TODO]

* Implement the genetic algorithm for integral inputs.
* Introduce n-match algorithm and replace the Roulette method.
* Implement the same for floating point numbers.
* Use in combination with python-crush/crush/optimize.py
* Faster `assign_fitness()` by using threading.

# Task List

- [] Implement `remove_similar_chromo()`
- [] Implement `favoritism()`
- [] Implement probability-based correct selection.
- [] Needs code-refactoring.
- [x] Implement `assign_fitness()` [This is specific to Ceph]
