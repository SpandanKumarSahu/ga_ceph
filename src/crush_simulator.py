#!/usr/bin/python

# Assumptions :
# 1. All objects are of same size
# 2. Ceph CRUSH behaves like random()

import bisect as bisect
import random


def normalise(array):
	return [float(i)/sum(array) for i in array]

def remove( checklist, i):
	checklist.remove(i)
	return checklist

def getWeights( checklist ):
	weight_list = [current_weights[i] for i in checklist]
	return weight_list

def input_weights():
	for i in range(num_OSD):
		temp = input()
		weights.append(temp)
	norm_weights = normalise(weights)
	current_weights = norm_weights[:]

def pick( checklist, random_number):
	temp_weights = normalise( getWeights(checklist) )
	for i in range(len(temp_weights)-1):
		temp_weights[i+1] += temp_weights[i]
	index = bisect.bisect(temp_weights, random_number)
	current_load_distribution[checklist[index]] += 1
	del checklist[index]
	return checklist

weights = list()

num_OSD = input()
replica_count = input()
input_weights()
count_objects = 0

current_load_distribution = [0]*num_OSD
current_weights = weights[:]
norm_weights = normalise(weights)

while (count_objects < 10000):
	checklist = range(num_OSD)
	picks = 0
	while (picks <= replica_count):
		checklist = pick(checklist, random.random())
		picks += 1
	count_objects += 1

norm_dist = normalise(current_load_distribution)
temp_str = "\n".join(map(str,norm_dist))
f = open("sample.txt", 'w')
f.write(temp_str)
f.close()
