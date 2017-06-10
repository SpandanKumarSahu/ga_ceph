#ifndef GENETICS_H
#define GENETICS_H

#include <string>
#include <stdlib.h>
#include <iostream.h>
#include <time.h>
#include <math.h>

using namespace std;

// Genetic Algorithms Part
float CROSSOVER_RATE = 0.7;
float MUTATION_RATE = 0.001;
int POP_SIZE;           //must be an even number
int CHROMO_LENGTH;
int GENE_LENGTH;
int MAX_ALLOWABLE_GENERATIONS;
int CORRECT_SELECTION_PROBABILITY = 0.9;

class chromo_t{
public:
  string    bits;
  float     fitness;

  chromo_t(){
    bits = string("");
    fitness = 0.0f;
  }
  chromo_t(string bts, float ftns): bits(bts), fitness(ftns){}
};


string convert_to_chromo(vector<int> num);
vector<int> convert_to_num(string chromo);
string generateChromosome();
void population_init(chromo_t *curr_population);
void assign_fitness(chromo_t *curr_population, int &total_fitness);
void favoritism(chromo_t *curr_population, chromo_t *next_population, int &count_npop);
void crossover(string &parent1, string &parent2, string &offspring1, string &offspring2);
void mutate(string &offspring);
string Roulette(int total_fitness, chromo_t *curr_population)
void fill_offspring(string &parent1, string &parent2, string &offspring1, string &offspring2);
void remove_similar_chromo(chromo_t *next_population, int &count_npop);
void add_random_chromo(chromo_t *next_population, int &count_npop);
void find_best_solution(chromo_t *curr_population);

#endif
