#ifndef EXTRA_CEPH_H
#define EXTRA_CEPH_H

#include "genetics.h"
using namespace std;

//Ceph Part
int num_OSD;
vector<int>weights_OSD;
vector<float>norm_weights;
int replica_count;

void assign_individual_fitness(chromo_t chromo);
void assign_fitness(chromo_t *curr_population, int &total_fitness);
void set_norm_weights();
vector<float> get_expected_distribution(vector<int>num);
vector<float> calculate_diff(vector<float> expected_distribution);

#endif
