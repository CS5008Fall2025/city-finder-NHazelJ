#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

// Maximum number of cities supported.
#define MAX_VERTICES   256

// Maximum length for a city name (including null terminator).
#define MAX_CITY_NAME  64
#define INF_DISTANCE   1000000000

typedef struct {
    // how many cities
    int number_of_vertices;
    // city names
    char city_names[MAX_VERTICES][MAX_CITY_NAME];
    // adjacency matrix of distances
    int distance_matrix[MAX_VERTICES][MAX_VERTICES];
} Graph;

// Build the graph from two files:
// vertex_file_name which each line has one city name
// distance_file_name which "city1 city2 distance" on each line
// returns 0 on success, -1 on failure.

int graph_init_from_files(Graph *graph, const char *vertex_file_name, const char *distance_file_name);

// printing all city names, one per line
void graph_print_cities(const Graph *graph);

// finding the index of a city in the city_names array.
// returns indexes or -1 if not found.

int graph_find_city_index(const Graph *graph, const char *city_name);

// debug helper to print the distance matrix
void graph_print_matrix(const Graph *graph);

// GRAPH_H
#endif