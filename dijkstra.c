// implementation of Dijkstra algorithm
#include <stdbool.h>
#include "dijkstra.h"

void dijkstra(const Graph *graph, int source_index, int distance_to[],int previous_vertex[]) {

    int number_of_vertices = graph->number_of_vertices;
    bool visited[MAX_VERTICES];

    // initialize all distances and visited flags
    for (int i = 0; i < number_of_vertices; i++) {
        // we do not know distance yet
        distance_to[i] = INF_DISTANCE;
        // no previous city yet
        previous_vertex[i] = -1;
        // no city is visited yet
        visited[i] = false;
    }

    // distance from source to itself is 0
    distance_to[source_index] = 0;

    // main loop, each time, pick the unvisited city with the smallest distance.
    for (int step = 0; step < number_of_vertices; step++) {

        int closest_vertex = -1;
        int closest_distance = INF_DISTANCE;

        // finding the unvisited vertex with smallest known distance
        for (int v = 0; v < number_of_vertices; v++) {
            if (!visited[v] && distance_to[v] < closest_distance) {
                closest_distance = distance_to[v];
                closest_vertex = v;
            }
        }

        // ff we did not find any unvisited vertex, we are done.
        if (closest_vertex == -1) {
            break;
        }

        visited[closest_vertex] = true;

        // for each neighbor, see if going through closest_vertex is shorter
        for (int neighbor = 0; neighbor < number_of_vertices; neighbor++) {
            int edge_distance =
                graph->distance_matrix[closest_vertex][neighbor];

            if (edge_distance == INF_DISTANCE) {
                // no road to that neighbor
                continue;
            }

            int alternative_distance = distance_to[closest_vertex] + edge_distance;

            if (alternative_distance < distance_to[neighbor]) {
                distance_to[neighbor] = alternative_distance;
                previous_vertex[neighbor] = closest_vertex;
            }
        }
    }
}