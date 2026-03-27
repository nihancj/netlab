#include <stdio.h>
#include <limits.h>

#define MAX_NODES 20
#define INF 9999 // Representing infinity for unconnected nodes

// Function to find the vertex with minimum distance value that isn't visited yet
int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Recursive function to print the shortest path
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        return;
    }
    printPath(parent, parent[j]);
    printf(" -> %d", j);
}

// Function to print the final routing table
void printSolution(int dist[], int n, int parent[], int src) {
    printf("\n--- Routing Table for Node %d ---\n", src);
    printf("Destination\tTotal Cost\tPath\n");
    for (int i = 0; i < n; i++) {
        if (i != src) {
            printf("%d\t\t", i);
            if (dist[i] == INF) {
                printf("Unreachable\t-\n");
            } else {
                printf("%d\t\t%d", dist[i], src);
                printPath(parent, i);
                printf("\n");
            }
        }
    }
    printf("---------------------------------\n");
}

// Dijkstra's algorithm for Link State Routing
void dijkstra(int cost[MAX_NODES][MAX_NODES], int src, int n) {
    int dist[MAX_NODES];    // Holds the shortest distance from src to i
    int visited[MAX_NODES]; // 1 if node is included in shortest path tree, 0 otherwise
    int parent[MAX_NODES];  // Array to store the path tree

    // Initialize all distances as INFINITE and visited array as 0
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        
        // Mark the picked vertex as processed
        visited[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < n; v++) {
            // Update dist[v] only if it's not visited, there is an edge from u to v,
            // and the total weight of path from src to v through u is smaller than current value of dist[v]
            if (!visited[v] && cost[u][v] != 0 && cost[u][v] != INF && 
                dist[u] != INF && dist[u] + cost[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + cost[u][v];
            }
        }
    }

    // Print the constructed distance array and paths
    printSolution(dist, n, parent, src);
}

int main() {
    int n, src;
    int cost[MAX_NODES][MAX_NODES];

    printf("Enter the number of nodes in the network (Max %d): ", MAX_NODES);
    scanf("%d", &n);

    printf("\nEnter the Cost Matrix:\n");
    printf("(Note: Enter 0 for self-loops, and %d for links that do not exist)\n", INF);
    
    for (int i = 0; i < n; i++) {
        printf("Row %d: ", i);
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            // Force the diagonal to be 0 (distance to itself)
            if (i == j) {
                cost[i][j] = 0;
            }
        }
    }

    printf("\nEnter the source node (0 to %d) to generate its routing table: ", n - 1);
    scanf("%d", &src);

    if (src >= 0 && src < n) {
        dijkstra(cost, src, n);
    } else {
        printf("Invalid source node.\n");
    }

    return 0;
}
