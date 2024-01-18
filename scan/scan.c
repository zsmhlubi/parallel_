// #include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int prescan[14];
// int at = 0;
struct upswipe_tree{
    int value;
    int stage_number;
    struct upswipe_tree *left_parent;
    struct upswipe_tree *right_parent;   
};
struct upswipe_tree* new_upswipe_tree(int value){
    struct upswipe_tree* upswipe_tree = (struct upswipe_tree*)malloc(sizeof(struct upswipe_tree));
    upswipe_tree->stage_number = 1;
    upswipe_tree->value = value;
    upswipe_tree->left_parent = NULL;
    upswipe_tree->right_parent = NULL;
    return upswipe_tree;
}
struct upswipe_tree* add_upswipe_tree(struct upswipe_tree* left, struct upswipe_tree* right){
    struct upswipe_tree* upswipe_tree = (struct upswipe_tree*)malloc(sizeof(struct upswipe_tree));
    upswipe_tree->stage_number = left->stage_number + 1;
    upswipe_tree->value = left->value + right->value;
    upswipe_tree->left_parent = left;
    upswipe_tree->right_parent = right;
    return upswipe_tree;
}

struct downswipe_tree{
    int value;
    int stage_number;
    struct downswipe_tree *left_child;
    struct downswipe_tree *right_child;
    struct upswipe_tree *cooperatng;
};
struct downswipe_tree* new_downswipe_tree(int value, int stage){
    struct downswipe_tree* downswipe_tree = (struct downswipe_tree*)malloc(sizeof(struct downswipe_tree));
    downswipe_tree->stage_number = stage;
    downswipe_tree->value = value;
    downswipe_tree->left_child = NULL;
    downswipe_tree->right_child = NULL;
    downswipe_tree->cooperatng = NULL;
    return downswipe_tree;
}

struct downswipe_tree* pre_scan(struct upswipe_tree* root, struct downswipe_tree* down_root, int stage){
    if (root->left_parent == NULL || root->left_parent->stage_number != root->right_parent->stage_number){
        return down_root;
    }
    struct downswipe_tree* left = new_downswipe_tree(down_root->value, stage);
    left->cooperatng = root->left_parent;
    down_root->left_child = pre_scan(left->cooperatng, left,1);
    struct downswipe_tree* right = new_downswipe_tree(root->left_parent->value + down_root->value, stage);
    right->cooperatng = root->right_parent;
    down_root->right_child = pre_scan(right->cooperatng, right,1);
    
    return down_root;
}

void print_tree(struct upswipe_tree *rot){
    if (rot == NULL){
        return;
    }
    print_tree(rot->left_parent);
    printf("%d ", rot->value);
    print_tree(rot->right_parent);
    
}

struct upswipe_tree* add(struct upswipe_tree* list[], int parent_stage, int size){
    int index = 0;
    int new_index = 0;
    int array_size = (int)(size / 2);
    if (size % 2 != 0){
        array_size = array_size + 1;
    }
    struct upswipe_tree* new_list[array_size];
    while (index < (size)){
        if (new_index < (int)(size/2)){
            struct upswipe_tree* child = add_upswipe_tree(list[index], list[index + 1]);
            new_list[new_index] = child;
            index = index + 2;
            new_index = new_index + 1;
        }
        else{
            list[index]->stage_number = parent_stage + 1;
            new_list[new_index] = list[index];
            index = index + 1;
        }
    }
    if (sizeof(new_list) / sizeof(struct upswipe_tree*) == 1){
        // print_tree(new_list[0]);
        return new_list[0];

    }
    else{
        int size = (sizeof(new_list) / sizeof(struct upswipe_tree*));
        
        struct upswipe_tree* root = add(new_list,parent_stage + 1, size);
        return root;
    }
}


int print_prescan(struct downswipe_tree *rot, int* scan[], int at){
    if (rot->left_child == NULL){
        scan[at] = &rot->value;
        at = at + 1;
        // printf("%d ", rot->value);
        return at;
    }
    at = print_prescan(rot->left_child,scan,at);
    at = print_prescan(rot->right_child,scan,at);
    return at;
}

void print_scan(int** scan, int size){
    printf("This is the scan of the given array : ");
    for (int i = 0 ; i < size; i++){ 
        // prescan[i] = prescan[i + 1];
        if (scan[i] == 0 && i != 0){
            continue;
        }
        printf("%d ", *scan[i]);
    }
    printf("\n");
}

void move(int** scan, int size){
    for (int i = 0 ; i < size; i++){
        scan[i] = scan[i + 1];
    }
}


int needed_processors(int array[], int size, int array_size){
    int row = 0;
    for (int i = 0; i < array_size; i+=size){
        if((array_size - i) < size){
            break;
        }
        row ++;
    }
    return row;
}

int main(){
    int size_of;
     printf("Enter size of array : ");
    scanf("%d", &size_of);
    printf("\n");
    int array[size_of];
    printf("Enter array to use : ");
    for (int i = 0; i < size_of; i++){
        int number;
        scanf("%d", &number);
        array[i] = number;
    }
    // for (int i = 0; i < size_of; i++){
    //     printf("%d ", array[i]);
    //     // array[i] = &number;
    // }
    int* scan[size_of];
    clock_t begin = clock();
    // serial computation
    struct upswipe_tree* original_list[(sizeof(array) / sizeof(int))];
    for (int i = 0 ; i < (sizeof(array) / sizeof(int)) ; i++){
        original_list[i] = new_upswipe_tree(array[i]);
    }
    int size = (sizeof(original_list) / sizeof(struct upswipe_tree*));
    struct upswipe_tree* root = add(original_list, 1, size);
    struct downswipe_tree* left = new_downswipe_tree(0, 1);
    struct downswipe_tree* prescan_tree = pre_scan(root,left,1);
    int at = 0;
    at = print_prescan(prescan_tree, scan, at);
    for (int i = 0 ; i < ((sizeof(scan) / sizeof(int*)) - 1); i++){ 
        scan[i] = scan[i + 1];
        // printf("%d\n", scan[i]);
    }
    scan[((sizeof(scan) / sizeof(int*)) - 1)] = &root->value;
    print_scan(scan, size_of);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    // parallel computation using OpenMp
    // try parallizing it from the begining without using functions meaning parallize it before defining the original list
    

    // int size_split;
    // int number_of_processors;
    // int left;
    // int add_to = 0;
    // if ((size_of/4) < 2){
    //     size_split = 2;
    //     number_of_processors = needed_processors(array, size_split, size_of);
    //     left = (size_split * number_of_processors) - size_of;
    // }
    // else{
    //     if((size_of % 4) == 0){
    //         size_split = size_of/4;
    //         number_of_processors = 4;
    //         left = (size_split * number_of_processors) - size_of;
    //     }
    //     else{
    //         size_split = size_of/4;
    //         number_of_processors = 4;
    //         left = (size_split * number_of_processors) - size_of;
    //     }
    // }
     
    // int** matrix[4];
    // int last[4];

    // omp_set_nested(1);
    // omp_set_num_threads(number_of_processors);

    // #pragma omp parallel shared(size_split, size_of, left, add_to , matrix)
    // {   
    //     int thread_number = omp_get_thread_num();
        
    //     if(left != 0){
    //         #pragma omp critical
    //         {
    //             left = left - 1;
    //             add_to = 1;
    //         }    
    //     }
    //     int start_at;
    //     int ends_at;
    //     if (thread_number == 0){
    //         start_at = (thread_number * size_split);
    //         ends_at = ((thread_number + 1) * size_split) + add_to;
    //     }
    //     else{
    //         start_at = (thread_number * size_split) + add_to;
    //         ends_at = ((thread_number + 1) * size_split) + add_to;
    //     }

    //         int size = ends_at - start_at;
    //         int split_array[size];
    //         matrix[thread_number] = (int**)malloc(size * sizeof(int*));
    //         int* scan[size];
    //         int at = 0;
    //         #pragma omp parallel for schedule (static)
    //         for (int i = start_at; i < ends_at; i++){
    //             split_array[i - start_at] = array[i];
    //         }
    //         struct upswipe_tree* original_list[(sizeof(split_array) / sizeof(int))];
    //         for (int i = 0 ; i < (sizeof(split_array) / sizeof(int)) ; i++){
    //             original_list[i] = new_upswipe_tree(split_array[i]);
    //         }
    //         struct upswipe_tree* root = add(original_list, 1, size);
    //         struct downswipe_tree* left = new_downswipe_tree(0, 1);
    //         struct downswipe_tree* prescan_tree = pre_scan(root,left,1);
    //         at = print_prescan(prescan_tree, scan, at);
    //         // move(scan, size);
           
    //         // scan[((sizeof(scan) / sizeof(int*)) - 1)] = &root->value;
            
    //         #pragma omp barrier
    //         for (int i = 0; i < size ;i++){
    //             array[start_at + i] = *scan[i];
    //         }
    //         last[thread_number] = root->value;

    // }
    // for(int i = 0; i < 3;i++){
    //     last[i + 1] = last[i] + last[i + 1];
    // }
    // int i = 1;
    // int b = 0;
    // int z = 0;
    // while (i < size_of){
    //     if (array[i] == 0){  
    //         b = last[z];
    //         z++;
    //     }
    //     array[i - 1] = array[i] + b;
    //     i++;
    // }
    // array[size_of - 2] = last[3];
    // array[size_of - 1] = array[size_of - 2] + array[size_of - 1];

    // for(int i = 0; i < size_of;i++){
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    // for(int i = 0; i < 4;i++){
    //     printf("%d ", last[i]);
    // }
    return 0;
}



