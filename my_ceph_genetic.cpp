#include <string>
#include <stdlib.h>
#include <iostream.h>
#include <time.h>
#include <math.h>
#include <bits/stdc++.h>
#include "genetics.h"

using namespace std;

// Genetic Algorithms Part
float CROSSOVER_RATE = 0.7;
float MUTATION_RATE = 0.001;
int POP_SIZE;           //must be an even number
int CHROMO_LENGTH;
int GENE_LENGTH;
int MAX_ALLOWABLE_GENERATIONS;

//Ceph Part
int num_OSD;
vector<int>weights_OSD;
int replica_count;

//Returns a float between 0 & 1
float floatRand(){
  return ((float)rand()/(RAND_MAX+1));
}

//Sets the global constants.
void init(){
  GENE_LENGTH = 7;
  cout<< "Enter number of OSDs"<<endl;
  cin>> num_OSD;
  cout<< " Enter replica count"<<endl;
  cin>> replica_count;

  cout<< "Enter weights of OSDs:"<<endl;
  int temp;
  for(int i = 0; i< num_OSD; i++){
    cin>>temp;
    if(temp > 100){
      i--;
      cout<< " Weight has to be integer within 100. See crushtool."<<endl;
    }
    else{
      weights_OSD.push_back(temp);
    }
  }

  CHROMO_LENGTH = num_OSD*7;
  POP_SIZE = max(100, 10*num_OSD);
  MAX_ALOWABLE_GENERATIONS = max(400, 100*num_OSD);
}

int main(){
  init();
  int gen_count = MAX_ALOWABLE_GENERATIONS;
  chromo_t curr_population[POP_SIZE];
  chromo_t next_population[POP_SIZE];
  population_init(curr_population);

  while(gen_count--){
    float total_fitness = 0.0f;
    assign_fitness(curr_population, total_fitness);
    int count_npop = 0;

    // The few best are directly promoted.
    //favoritism(curr_population,next_population, count_npop);

    //The major genetics part
    while(count_npop < POP_SIZE){
      string parent1 = Roullete(total_fitness, curr_population);
      string parent2 = Roullete(total_fitness, curr_population);
      string offspring1, offspring2;
      crossover(parent1, parent2, offspring1, offspring2);
      mutate(offspring1);
      mutate(offspring2);
      fill_offspring(parent1, parent2, offspring1, offspring2);
      next_population[count_npop++] = offspring1;
      next_population[count_npop++] = offspring2;
    }

    //Remove similar looking chromosomes.
    //remove_similar_chromo(next_population, count_npop);

    //add few random entries. Only after removing certain chromosomes.
    //add_random_chromo(next_population, count_npop);

    //Copy next generation back to current generation
    for (int i=0; i<POP_SIZE; i++){
      current_population[i] = next_population[i];
    }

  }

  find_best_solution(current_population);
  return 0;
}
