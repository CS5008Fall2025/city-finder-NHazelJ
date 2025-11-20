#include <stdio.h>
#include <string.h>
#include "graph.h"

// helper function to remove a newline at the end of a string.
// fgets() keeps the '\n'. this removes it so we get boston not boston\n

static void trim_newline(char *line) {
    size_t length = strlen(line);
    if (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
        line[length - 1] = '\0';
    }
}

// look for a city name in graph->city_names and return its index
int graph_find_city_index(const Graph *graph, const char *city_name) {
    for (int index = 0; index < graph->number_of_vertices; index++) {
        if (strcmp(graph->city_names[index], city_name) == 0) {
            // found
            return index;
        }
    }
    // not found
    return -1;
}

// print all city names – used for the list command
void graph_print_cities(const Graph *graph) {
    for (int index = 0; index < graph->number_of_vertices; index++) {
        printf("%s\n", graph->city_names[index]);
    }
}

// print the distance matrix.
void graph_print_matrix(const Graph *graph) {
    printf("Distance matrix (INF=%d means no direct road):\n", INF_DISTANCE);

    // print header row
    printf("      ");
    for (int j = 0; j < graph->number_of_vertices; j++) {
        printf("%6s", graph->city_names[j]);
    }
    printf("\n");

    // print each row
    for (int i = 0; i < graph->number_of_vertices; i++) {
        printf("%6s", graph->city_names[i]);
        for (int j = 0; j < graph->number_of_vertices; j++) {
            int distance = graph->distance_matrix[i][j];
            if (distance == INF_DISTANCE) {
                printf("%6s", "INF");
            } else {
                printf("%6d", distance);
            }
        }
        printf("\n");
    }
}

// read vertex file, each line is a city name
static int load_vertices(Graph *graph, const char *vertex_file_name) {
    FILE *file_pointer = fopen(vertex_file_name, "r");
    if (file_pointer == NULL) {
        perror("Could not open vertex file");
        return -1;
    }

    graph->number_of_vertices = 0;

    char line_buffer[256];

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        trim_newline(line_buffer);

        // skipping empty lines
        if (line_buffer[0] == '\0') {
            continue;
        }

        if (graph->number_of_vertices >= MAX_VERTICES) {
            fprintf(stderr, "Too many vertices. Increase MAX_VERTICES.\n");
            fclose(file_pointer);
            return -1;
        }

        // copy the city name into the array
        strncpy(graph->city_names[graph->number_of_vertices],
                line_buffer,
                MAX_CITY_NAME - 1);
        graph->city_names[graph->number_of_vertices][MAX_CITY_NAME - 1] = '\0';

        graph->number_of_vertices++;
    }

    fclose(file_pointer);

    // Initialize the distance matrix. distance to itself is 0
    for (int i = 0; i < graph->number_of_vertices; i++) {
        for (int j = 0; j < graph->number_of_vertices; j++) {
            if (i == j) {
                graph->distance_matrix[i][j] = 0;
            } else {
                graph->distance_matrix[i][j] = INF_DISTANCE;
            }
        }
    }

    return 0;
}

// read distance file: city1 city2 distance per line
static int load_distances(Graph *graph, const char *distance_file_name) {
    FILE *file_pointer = fopen(distance_file_name, "r");
    if (file_pointer == NULL) {
        perror("Could not open distances file");
        return -1;
    }

    char line_buffer[256];
    char city_one[MAX_CITY_NAME];
    char city_two[MAX_CITY_NAME];
    int distance_value;

    while (fgets(line_buffer, sizeof(line_buffer), file_pointer) != NULL) {
        // expecting city1 city2 distance
        // if we do not get 3, we just skip that line
        
        if (sscanf(line_buffer, "%63s %63s %d",
                   city_one, city_two, &distance_value) != 3) {
            continue;  // bad or empty line, skip
        }

        int index_one = graph_find_city_index(graph, city_one);
        int index_two = graph_find_city_index(graph, city_two);

        if (index_one == -1 || index_two == -1) {
            // if the city is not in the vertex list we skip the line
            continue;
        }

        // undirected graph: road goes both ways. so we fill BOTH [one][two] and [two][one].
        graph->distance_matrix[index_one][index_two] = distance_value;
        graph->distance_matrix[index_two][index_one] = distance_value;
    }

    fclose(file_pointer);
    return 0;
}

// public function creates a graph from both files
int graph_init_from_files(Graph *graph,
                          const char *vertex_file_name,
                          const char *distance_file_name) {
    if (load_vertices(graph, vertex_file_name) != 0) {
        return -1;
    }
    if (load_distances(graph, distance_file_name) != 0) {
        return -1;
    }
    return 0;
}