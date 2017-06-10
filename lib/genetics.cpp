#include <string>
#include <stdlib.h>
#include <iostream.h>
#include <time.h>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;


//Generates random chromosome.
string generateChromosome(){
  string temp="";
  for(int i = 0; i < CHROMO_LENGTH; i++){
    if(rand()%2 == 0)
      temp += string("0");
    else
      temp += string("1");
  }
  return temp;
}

//Coding to chromosome
string convert_to_chromo(vector<int> num){
  char ar[CHROMO_LENGTH];
  memset(ar, '0', sizeof(ar));
  int count = num.size();
  for(int i = CHROMO_LENGTH-1; i >=0 ; i -= GENE_LENGTH){
    int j = i;
    int temp = num.at(count-1);
    count--;
    while(temp != 0){
      if(j%2 == 1){
	ar[j] = '1';
      }
      j--;
      temp>>1;
    }
  }
  string temp(ar);
  return temp;
}

//Decoding a chromosome
vector<int> convert_to_num(string chromo){
  vector<int> num;
  for(int i = 0; i < CHROMO_LENGTH; i += GENE_LENGTH){
    int temp = 1;
    int val = 0;
    for(int j = GENE_LENGTH-1; j >= 0; j--){
      if(chromo.at(j+i) == '1')
	val += temp;
      temp<<1;
    }
    num.push_back(temp);
  }
  return num;
}

//Initialising the first population
void population_init(chromo_t *curr_population){
  for(int i = 0; i < POP_SIZE; i++){
    curr_population[i].bits = generateChromosome();
    curr_population[i].fitness = 0.0f;
  }
  curr_population[0].bits = convert_to_chromo(weights_OSD);
}

//Crossover function
void crossover(string &parent1, string &parent2, string &offspring1, string &offspring2);{
  if (floatRand() < CROSSOVER_RATE){
      int crossover = (int) (RANDOM_NUM * CHROMO_LENGTH);
      offspring1 = parent1.substr(0, crossover) + parent2.substr(crossover, CHROMO_LENGTH);
      offspring2 = parent2.substr(0, crossover) + parent1.substr(crossover, CHROMO_LENGTH);
    }
}

//Mutate function
void mutate(string &offspring){
  for (int i=0; i<offspring.length(); i++){
    if (floatRand() < MUTATION_RATE){
      if (bits.at(i) == '1')
        bits.at(i) = '0';
      else
        bits.at(i) = '1';
    }
  }
}

//Roullete function
string Roulette(float total_fitness, chromo_t *curr_population, int num_mem = POP_SIZE){
  float Slice = (float)(RANDOM_NUM * total_fitness);
  float FitnessSoFar = 0.0f;
  for (int i=0; i<num_mem; i++){
      FitnessSoFar += Population[i].fitness;
      if (FitnessSoFar >= Slice)
	       return Population[i].bits;
  }
  return "";
}

//Add random chromosomes till the size of the population is met. The part of simulated annealing.
void add_random_chromo(chromo_t *next_population, int &count_npop){
  while(count_npop < POP_SIZE){
    next_population[count_npop] = generateChromosome();
    count_npop++;
  }
}

//Print the result
void find_best_solution(chromo_t *curr_population){
  float total_fitness = 0.0f;
  assign_fitness(curr_population, total_fitness);

  int max_index = 0;
  for(int i=1; i<POP_SIZE; i++){
    if(curr_population[i].fitness > curr_population[max_index].fitness)
      max_index = i;
  }

  vector<int> res = convert_to_num(curr_population[max_index].bits);
  std::std::cout << "The tuned weights should be: " << '\n';
  for(int i=0; i<res.size(); i++){
    cout<<res.at(i)<<" ";
  }
}

/*
  Choose whether offsprings or parents will be in the next generations

  ** Currently uses only Roulette method
  ** Should be using n_match method
*/
void fill_offspring(string &parent1, string &parent2, string &offspring1, string &offspring2){
  assign_individual_fitness(offspring1);
  assign_individual_fitness(offspring2);
  chromo_t temp_population[4];
  temp_population[0] = parent1;
  temp_population[1] = parent2;
  temp_population[2] = offspring1;
  temp_population[3] = offspring2;
  float total_fitness = parent1.fitness + parent2.fitness + offspring1.fitness + offspring2.fitness;
  offspring1 = Roulette(total_fitness, temp_population, 4);
  offspring2 = Roulette(total_fitness, temp_population, 4);
}

//[TODO] Let the best chromosomes pass on unharmed
void favoritism(chromo_t *curr_population, chromo_t *next_population, int &count_npop){
  int favorites = min(10,max(1, (int) (0.05 * POP_SIZE)));
}

//[TODO] Remove similar chromosomes.
void remove_similar_chromo(chromo_t *next_population, int &count_npop){

}
