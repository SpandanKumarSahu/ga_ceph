#include <bits/stdc++.h>
#include "genetics.h"

using namespace std;

//Sets the global constants.
void init(){
  CROSSOVER_RATE = 0.7;
  MUTATION_RATE = 0.001;
  CORRECT_SELECTION_PROBABILITY = 0.9;
  GENE_LENGTH = 7;
  cout<< "Enter number of OSDs"<<endl;
  cin>> num_OSD;
  cout<< "Enter replica count"<<endl;
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
  set_norm_weights();

  CHROMO_LENGTH = num_OSD*GENE_LENGTH;
  POP_SIZE = min(100, 10*num_OSD);
  MAX_ALLOWABLE_GENERATIONS = min(20*num_OSD,500);
}

int main(){
  init();
  int gen_count = MAX_ALLOWABLE_GENERATIONS;
  chromo_t curr_population[POP_SIZE];
  chromo_t next_population[POP_SIZE];
  population_init(curr_population);
  float total_fitness = 0.0f;
  assign_fitness(curr_population, total_fitness);

  while(gen_count--){
    int count_npop = 0;

    // The few best are directly promoted.
    favoritism(curr_population,next_population, count_npop);

    //The major genetics part
    while(count_npop < POP_SIZE){
      chromo_t parent1 = Roulette(total_fitness, curr_population);
      chromo_t parent2 = Roulette(total_fitness, curr_population);
      chromo_t offspring1, offspring2;
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

    //To improve upon the running time, replace similar chromosomes with randomly generated chromosomes
    //replace_similar_with_random_chromo(next_population);

    //Copy next generation back to current generation and update total_fitness
    total_fitness = 0.0f;
    for (int i=0; i<POP_SIZE; i++){
      curr_population[i] = next_population[i];
      total_fitness += next_population[i].fitness;
    }
    cout<<(1.0 - ((float)gen_count/MAX_ALLOWABLE_GENERATIONS))*100<<"% completed."<<endl;
  }

  find_best_solution(curr_population);
  return 0;
}
