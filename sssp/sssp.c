// #include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
    int weight;
    int node_number;
    struct node* from;  
};

struct node* new_node(int node_number){
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->from = NULL;
    node->weight = 0;
    node->node_number = node_number;
    return node;
}


void update_distance(int** matrix, struct node* next_node, struct node* distance[], int left_nodes){
    // struct node* new_distance[left_nodes];
    // new_distance[next_node->node_number] = next_node;
    for (int node_number = 1; node_number < left_nodes; node_number++){
        if (next_node->node_number == node_number){
            continue;
        }
        else{
            if(matrix[next_node->node_number][node_number] == 0){
                // struct node* next = new_node(node_number);
                // new_distance[node_number] = next;
                continue;
            }
            else{
                int weight = matrix[next_node->node_number][node_number] + next_node->weight;
                struct node* next = new_node(node_number);
                next->from = next_node;
                next->weight = weight;
                // new_distance[node_number] = next; 
                if (next->weight < distance[node_number]->weight){
                    distance[node_number] = next;
                }
                else if(distance[node_number]->weight == 0){
                    distance[node_number] = next;
                }
            }                
        }
    }
    // compare_distances(distance, new_distance, left_nodes);
}

void get_distance(int number_nodes, int** matrix, struct node* from, struct node* distance[]){
    distance[0] = from;
    for (int node_number = 1; node_number < number_nodes; node_number++){
        struct node* next = new_node(node_number);
        next->from = from;
        next->weight = matrix[0][node_number];
        distance[node_number] = next;
    }
}


bool update(int** matrix, int number_nodes, struct node* distance[]){
    bool missed = false;
    for (int from = 1; from < number_nodes; from++){
        for (int i = from; i < number_nodes; i++){
            if (distance[i]->weight != 0){
                update_distance(matrix, distance[i], distance, number_nodes);
            }
            else{
                missed = true;
            }
        } 
    }
    return missed;
}
void print_distance(struct node* distance[], int number_nodes){
    printf("distance to other nodes\n\n nodes          distance \n   v             0->v \n");
    for (int i = 1; i < number_nodes; i++){
        printf("   %d              %d\n", i, distance[i]->weight);
    }
}
void back_tracking(struct node* node){
    if (node->from == NULL){
        return;
        // printf("%d -> ", node->node_number);
    }
    back_tracking(node->from);
    printf("%d -> ",node->from->node_number);
}

void print_path(struct node* distance[], int number_nodes){
    printf("\npath to other nodes from 0\n\n nodes             path \n   v               0 -> n \n");
    for (int i = 1; i < number_nodes; i++){
        printf("   %d               ", i);
        back_tracking(distance[i]);
        printf("%d\n", distance[i]->node_number);
    }
}

// // void compare_distances_omp(struct node* distance[],struct node* new_distance[], int left_nodes){
// //     // #pragma omp parallel for firstprivate(left_nodes) shared(distance, new_distance)
// //     for (int node_number = 1; node_number < left_nodes; node_number++){
// //         if (new_distance[node_number]->weight == 0){
// //             continue;
// //         }
// //         else if (new_distance[node_number]->weight < distance[node_number]->weight){
// //             distance[node_number] = new_distance[node_number];
// //         }
// //         else if(distance[node_number]->weight == 0){
// //             distance[node_number] = new_distance[node_number];
// //         }
// //     }
// // }

// void update_distance_omp(int** matrix, struct node* next_node, struct node* distance[], int left_nodes){
//     // struct node* new_distance[left_nodes];
//     // new_distance[next_node->node_number] = next_node;
//     // bool less_then = false;
//     // #pragma omp parallel for firstprivate(left_nodes, matrix, next_node) shared(new_distance)

//     omp_set_num_threads(5);

//     #pragma omp parallel for shared(matrix, next_node, distance) schedule(static)
//     for (int node_number = 1; node_number < left_nodes; node_number++){
//         if (next_node->node_number == node_number){
//             continue;
//         }
//         else if(matrix[next_node->node_number][node_number] == 0){
//             // struct node* next = new_node(node_number);
//             // new_distance[node_number] = next;
//             continue;
//         }
//         else{
//             int weight = matrix[next_node->node_number][node_number] + next_node->weight;
//             struct node* next = new_node(node_number);
//             next->from = next_node;
//             next->weight = weight;
//             // new_distance[node_number] = next;
//             #pragma omp critical 
//             {
//                 if (next->weight < distance[node_number]->weight){
//                     distance[node_number] = next;
//                 }
//                 else if(distance[node_number]->weight == 0){
//                     distance[node_number] = next;
//                 }
//             }
//         }                
//     }
//     // compare_distances_omp(distance, new_distance, left_nodes);
// }

// bool update_omp(int** matrix, int number_nodes, struct node* distance[]) {
//     bool missed = false;

//     // omp_set_nested(2);
//     omp_set_num_threads(5);

//     #pragma omp parallel for shared(matrix, distance, missed) schedule(dynamic)
//     for (int from = 1; from < number_nodes; from++) {
//         for (int i = from; i < number_nodes; i++) {
//             if (distance[i]->weight != 0) {
//                 update_distance_omp(matrix, distance[i], distance, number_nodes);
//             }
//             else {
//                 #pragma omp critical
//                 missed = true;
//             }
//         }
//     }
//     return missed;
// }

void function(int** edge_matrix, int number_nodes){
    // print_matrix(number_nodes, edge_matrix);
    struct node* start = new_node(0);
    struct node* distances[number_nodes];
    get_distance(number_nodes, edge_matrix, start, distances);
    
    clock_t begin = clock();
    bool missed = update(edge_matrix, number_nodes, distances);
    while (missed){
        missed = update(edge_matrix, number_nodes, distances);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    print_distance(distances, number_nodes);
    print_path(distances, number_nodes);
    printf("%f\n", time_spent);
}



int main(){
    int number_nodes;
    int number_vertices;
    printf("Enter the number of nodes : ");
    scanf("%d", &number_nodes);
    printf("Enter the number of edges : ");
    scanf("%d", &number_vertices);

    int * edge_matrix[number_nodes];
    for (int i = 0; i < number_nodes; i++){
        edge_matrix[i] = calloc(number_nodes, sizeof(int));
        for (int j = 0; j < number_nodes; j++){
            edge_matrix[i][j] = 0;
        }
    }
    printf("Enter all edges : ");

    for (int i = 0; i < number_vertices; i++){
        int from;
        int to;
        int weight;
        scanf("%d", &from);
        scanf("%d", &to);
        scanf("%d", &weight);
        edge_matrix[from][to] = weight;
        edge_matrix[to][from] = weight;
    }

    function(edge_matrix, number_nodes);
    
    return 0;
}