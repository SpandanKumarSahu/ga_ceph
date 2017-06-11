#include "genetics.h"
#include "extra_ceph.h"
#include <bits/stdc++.h>
using namespace std;

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
void assign_fitness(chromo_t *curr_population, int &total_fitness){
  for(int i=0; i<POP_SIZE; i++){
    assign_individual_fitness(curr_population[i]);
    total_fitness += curr_population[i].fitness;
  }
}

//A bit of trickery to go about the precision of floating points.
void set_norm_weights(){
  int total_weight = 0;
  for(int i=0; i<num_OSD; i++){
    total_weight += weights.at(i);
  }
  float total_weight = 0.0f;
  for(int i=0; i<num_OSD-1; i++){
    norm_weights.at(i) = ((float) weights.at(i))/total_weight;
    total_weight += norm_weights.at(i);
  }
  norm_weights.at(num_OSD-1) = 1.0 - total_weight;
}

vector<float> get_expected_distribution(vector<int>num){
  string input = to_string(num_OSD) + "\n" + to_string(replica_count) + "\n";
  for(int i=0; i<num.size(); i++){
    input += to_string(num.at(i)) + "\n"
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
