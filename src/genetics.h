#ifndef GENETICS_H
#define GENETICS_H

#include <bits/stdc++.h>

using namespace std;

// Genetic Algorithms Part
float CROSSOVER_RATE;
float MUTATION_RATE;
int POP_SIZE;           //must be an even number
int CHROMO_LENGTH;
int GENE_LENGTH;
int MAX_ALLOWABLE_GENERATIONS;
int CORRECT_SELECTION_PROBABILITY;

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

//Returns a float between 0 & 1
float floatRand(){
  return (((float)rand()+1)/(RAND_MAX));
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
      if(temp%2 == 1){
	       ar[j] = '1';
      }
      j--;
      temp = temp/2;
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


//Crossover function
void crossover(chromo_t &parent1, chromo_t &parent2, chromo_t &offspring1, chromo_t &offspring2){
  if (floatRand() < CROSSOVER_RATE){
      int crossover = (int) (floatRand() * CHROMO_LENGTH);
      offspring1.bits = parent1.bits.substr(0, crossover) + parent2.bits.substr(crossover, CHROMO_LENGTH);
      offspring2.bits = parent2.bits.substr(0, crossover) + parent1.bits.substr(crossover, CHROMO_LENGTH);
    }
}

//Mutate function
void mutate(chromo_t &offspring){
  for (int i=0; i<offspring.bits.length(); i++){
    if (floatRand() < MUTATION_RATE){
      if (offspring.bits.at(i) == '1')
        offspring.bits.at(i) = '0';
      else
        offspring.bits.at(i) = '1';
    }
  }
}

//Roullete function
chromo_t Roulette(float total_fitness, chromo_t *curr_population, int num_mem = POP_SIZE){
  float Slice = (float)(floatRand() * total_fitness);
  float FitnessSoFar = 0.0f;
  for (int i=0; i<num_mem; i++){
      FitnessSoFar += curr_population[i].fitness;
      if (FitnessSoFar >= Slice)
	       return curr_population[i];
  }
}

//Add random chromosomes till the size of the population is met. The part of simulated annealing.
void add_random_chromo(chromo_t *next_population, int &count_npop){
  while(count_npop < POP_SIZE){
    next_population[count_npop].bits = generateChromosome();
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
  cout << "The tuned weights should be: " << '\n';
  for(int i=0; i<res.size(); i++){
    cout<<res.at(i)<<" ";
  }
}

/*
  Choose whether offsprings or parents will be in the next generations

  ** Currently uses only Roulette method
  ** Should be using n_match method
*/
void fill_offspring(chromo_t &parent1, chromo_t &parent2, chromo_t &offspring1, chromo_t &offspring2){
  int pop = 0;
  chromo_t temp_population[4];
  float total_fitness = 0.0f;
  if(offspring1.bits.size() != 0){
    assign_individual_fitness(offspring1);
    temp_population[pop++] = offspring1;
    total_fitness += offspring1.fitness;
  }
  if(offspring2.bits.size() != 0){
    assign_individual_fitness(offspring2);
    temp_population[pop++] = offspring2;
    total_fitness += offspring2.fitness;
  }
  temp_population[pop++] = parent1;
  temp_population[pop++] = parent2;
  total_fitness += parent1.fitness + parent2.fitness;
  offspring1 = Roulette(total_fitness, temp_population, pop);
  offspring2 = Roulette(total_fitness, temp_population, pop);
}

//[TODO] Let the best chromosomes pass on unharmed
void favoritism(chromo_t *curr_population, chromo_t *next_population, int &count_npop){
  int favorites = min(10,max(1, (int) (0.05 * POP_SIZE)));
}

//[TODO] Remove similar chromosomes.
void remove_similar_chromo(chromo_t *next_population, int &count_npop){

}

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

//Initialising the first population
void population_init(chromo_t *curr_population){
  for(int i = 0; i < POP_SIZE; i++){
    curr_population[i].bits = generateChromosome();
    curr_population[i].fitness = 0.0f;
  }
  curr_population[0].bits = convert_to_chromo(weights_OSD);
}

void assign_individual_fitness(chromo_t &chromo){
  vector<int> num = convert_to_num(chromo.bits);
  vector<float>expected_distribution = get_expected_distribution(num);
  vector<float>diff = calculate_diff(expected_distribution);
  float fitness = 0.0f;
  for(int i=0; i<diff.size(); i++){
    fitness += diff.at(i);
  }
  fitness = num_OSD - fitness;
  chromo.fitness = fitness;
}

//Assign fitness to members of population
void assign_fitness(chromo_t *curr_population, float &total_fitness){
  for(int i=0; i<POP_SIZE; i++){
    assign_individual_fitness(curr_population[i]);
    total_fitness += curr_population[i].fitness;
  }
}

//A bit of trickery to go about the precision of floating points.
void set_norm_weights(){
  int total_weight = 0;
  for(int i=0; i<num_OSD; i++){
    total_weight += weights_OSD.at(i);
  }
  float total_norm_weight = 0.0f;
  for(int i=0; i<num_OSD-1; i++){
    norm_weights.push_back(((float) weights_OSD.at(i))/total_weight);
    total_norm_weight += norm_weights.at(i);
  }
  norm_weights.push_back(1.0 - total_norm_weight);
}

vector<float> get_expected_distribution(vector<int>num){
  string input = to_string(num_OSD) + "\n" + to_string(replica_count) + "\n";
  for(int i=0; i<num.size(); i++){
    input += to_string(num.at(i)) + "\n";
  }
  fstream f("sample.txt", ios::out);
  f << input;
  f.close();
  system("python crush_simulator.py < sample.txt");
  vector<float> expected_distribution;
  fstream file_input("sample.txt");
  float temp;
  while(file_input >> temp){
    expected_distribution.push_back(temp);
  }
  system("rm -f sample.txt");
  return expected_distribution;
}

vector<float> calculate_diff(vector<float> expected_distribution){
  vector<float> res;
  for(int i=0; i<expected_distribution.size(); i++){
    res.push_back(abs(expected_distribution.at(i) - norm_weights.at(i)));
  }
  return res;
}


#endif
