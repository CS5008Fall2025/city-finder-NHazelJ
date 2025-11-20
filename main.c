// main program for shortest path finder
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "graph.h"
#include "dijkstra.h"

// printing the help text, exactly like sample output
static void print_help(void) {
    printf("Commands:\n");
    printf("\tlist - list all cities\n");
    printf("\t<city1> <city2> - find the shortest path between two cities\n");
    printf("\thelp - print this help message\n");
    printf("\texit - exit the program\n");
}

static void print_welcome(void) {
    printf("*****Welcome to the shortest path finder!******\n");
    print_help();
    printf("*********\n");
}

// printing the path from source to destination using the previous_vertex[] array that Dijkstra filled
static void print_path(const Graph *graph, int source_index, int destination_index, int distance_to[], int previous_vertex[]) {

    // if distance is infinity, there is no path
    if (distance_to[destination_index] == INF_DISTANCE) {
        printf("Path Not Found...\n");
        return;
    }

    // reconstructing the path backwards into this array
    int path_indices[MAX_VERTICES];
    int path_length = 0;

    int current = destination_index;
    while (current != -1 && path_length < MAX_VERTICES) {
        path_indices[path_length] = current;
        path_length++;
        current = previous_vertex[current];
    }

    printf("Path Found...\n");

    // printing from source to destination
    for (int i = path_length - 1; i >= 0; i--) {
        int city_index = path_indices[i];
        printf("\t%s\n", graph->city_names[city_index]);
    }

    printf("Total Distance: %d\n", distance_to[destination_index]);
}

int main(int argc, char *argv[]) {

    // expecting ./map.out <vertices> <distances>

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <vertices> <distances>\n", argv[0]);
        return 1;
    }

    const char *vertex_file_name = argv[1];
    const char *distance_file_name = argv[2];

    Graph graph;

    // building the graph from the two files
    if (graph_init_from_files(&graph, vertex_file_name, distance_file_name) != 0) {
        fprintf(stderr, "Error loading graph data.\n");
        return 1;
    }

    print_welcome();

    char input_line[256];

    while (true) {
        printf("Where do you want to go today? ");

        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            // EOF or error
            break;
        }

        // remove newline if present
        size_t length = strlen(input_line);
        if (length > 0 &&
            (input_line[length - 1] == '\n' || input_line[length - 1] == '\r')) {
            input_line[length - 1] = '\0';
        }

        // handle exit
        if (strcmp(input_line, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // handle help
        if (strcmp(input_line, "help") == 0) {
            print_help();
            continue;
        }

        // handle list
        if (strcmp(input_line, "list") == 0) {
            graph_print_cities(&graph);
            continue;
        }
        // handle print
        if (strcmp(input_line, "print") == 0) {
            graph_print_matrix(&graph);
            continue;
        }

        // trying to read two city names from the line
        char city_one[MAX_CITY_NAME];
        char city_two[MAX_CITY_NAME];
        int token_count = sscanf(input_line, "%63s %63s", city_one, city_two);

        if (token_count != 2) {
            printf("Invalid Command\n");
            print_help();
            continue;
        }

        int index_one = graph_find_city_index(&graph, city_one);
        int index_two = graph_find_city_index(&graph, city_two);

        // if either city is not in the list, treat as invalid command
        if (index_one == -1 || index_two == -1) {
            printf("Invalid Command\n");
            print_help();
            continue;
        }

        // now we run Dijkstra from city_one to all other cities
        int distance_to[MAX_VERTICES];
        int previous_vertex[MAX_VERTICES];

        dijkstra(&graph, index_one, distance_to, previous_vertex);

        // and we print the path from city_one to city_two
        print_path(&graph, index_one, index_two, distance_to, previous_vertex);
    }

    return 0;
}