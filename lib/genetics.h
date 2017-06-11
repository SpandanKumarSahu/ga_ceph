#ifndef GENETICS_H
#define GENETICS_H

#include <bits/stdc++.h>

using namespace std;

// Genetic Algorithms Part
float CROSSOVER_RATE = 0.7;
float MUTATION_RATE = 0.001;
int POP_SIZE;           //must be an even number
int CHROMO_LENGTH;
int GENE_LENGTH;
int MAX_ALLOWABLE_GENERATIONS;
int CORRECT_SELECTION_PROBABILITY = 0.9;

//Ceph Part
int num_OSD;
vector<int>weights_OSD;
vector<float>norm_weights;
int replica_count;

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

//Ceph specific
string generateChromosome();
void population_init(chromo_t *curr_population);
void assign_individual_fitness(chromo_t &chromo);
void assign_fitness(chromo_t *curr_population, float &total_fitness);
void set_norm_weights();
vector<float> get_expected_distribution(vector<int>num);
vector<float> calculate_diff(vector<float> expected_distribution);

//Generic Algorithm speific
float floatRand();
string convert_to_chromo(vector<int> num);
vector<int> convert_to_num(string chromo);
void favoritism(chromo_t *curr_population, chromo_t *next_population, int &count_npop);
void crossover(chromo_t &parent1, chromo_t &parent2, chromo_t &offspring1, chromo_t &offspring2);
void mutate(chromo_t &offspring);
chromo_t Roulette(int total_fitness, chromo_t *curr_population);
void fill_offspring(chromo_t &parent1, chromo_t &parent2, chromo_t &offspring1, chromo_t &offspring2);
void remove_similar_chromo(chromo_t *next_population, int &count_npop);
void add_random_chromo(chromo_t *next_population, int &count_npop);
void find_best_solution(chromo_t *curr_population);

#endif
