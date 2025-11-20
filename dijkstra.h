#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

// running Dijkstra starting from source_index
//  will fills, distance_to[i] = shortest distance from source to i
//  it will also previous_vertex[i] = previous vertex on shortest path to i

void dijkstra(const Graph *graph, int source_index, int distance_to[], int previous_vertex[]);

// DIJKSTRA_H
#endif