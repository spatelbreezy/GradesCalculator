#include <stdio.h>
#include <math.h>
#define ASSIGNMENT_SIZE 4
#define MAX_NUMBER 50
void process_drops(int database[]);
double process_numeric_score(int database[]);
void process_stats(int database[], double *mean, double *std);

int main() {
    char stats = 'X';
    int i = 0, penalty = 0, to_drop = 0, num_assignments = 0, assign_num = 0,
    score = 0, weight = 0, days_late = 0, total_weight = 0, 
    database[MAX_NUMBER * ASSIGNMENT_SIZE + 4] = {0},
    unedited_database[MAX_NUMBER * ASSIGNMENT_SIZE + 4] = {0};
    double numeric_score = 0.0, mean = 0.0, std = 0.0, 
    *mean_ptr = &mean, *std_ptr = &std; 
    
    scanf(" %d %d %c", &penalty, &to_drop, &stats);
    scanf(" %d", &num_assignments);
    /* Useful information added to database so it is all in one data structure*/
    database[0] = penalty; 
    database[1] = to_drop;
    database[2] = num_assignments;
    unedited_database[0] = penalty; 
    unedited_database[1] = to_drop;
    unedited_database[2] = num_assignments;

    for(i = 0; i < num_assignments; i++) {
        scanf(" %d, %d, %d, %d", &assign_num, &score, &weight, &days_late);
        /* Using formula (assignment number * assignment size) to store 
        assignment information all in one array database. */
        database[assign_num * ASSIGNMENT_SIZE] = assign_num;
        database[assign_num * ASSIGNMENT_SIZE + 1] = score;
        database[assign_num * ASSIGNMENT_SIZE + 2] = weight;
        database[assign_num * ASSIGNMENT_SIZE + 3] = days_late;

        /*Creating a copy of unedited assignment values*/
        unedited_database[assign_num * ASSIGNMENT_SIZE] = assign_num;
        unedited_database[assign_num * ASSIGNMENT_SIZE + 1] = score;
        unedited_database[assign_num * ASSIGNMENT_SIZE + 2] = weight;
        unedited_database[assign_num * ASSIGNMENT_SIZE + 3] = days_late;

        total_weight += weight;
    } 
    if(total_weight != 100) {
        printf("ERROR: Invalid values provided");
        return 0;
    } 

    process_drops(database);
    numeric_score = process_numeric_score(database);
   
    printf("Numeric Score: %5.4f\n", numeric_score);
    printf("Points Penalty Per Day Late: %d\n", penalty); 
    printf("Number of Assignments Dropped: %d\n", to_drop);
    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n"); 
    for(i = 4; i < MAX_NUMBER * ASSIGNMENT_SIZE + 4; i += 4) {
        if(unedited_database[i] != 0) {
            printf("%d, %d, %d, %d\n", unedited_database[i], 
            unedited_database[i + 1], unedited_database[i + 2], unedited_database[i + 3]);
        } 
    }
    if(stats == 'Y' || stats == 'y') {
        process_stats(unedited_database, mean_ptr, std_ptr);
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, std);
    }

    return 0;   
}

void process_drops(int database[]) {
    int curr = 0, i = 0, min = 99999, min_idx = 0; 

    if(database[1] != 0) { /* database[1] = # of assignments to drop */
        for(curr = 1; curr <= database[1]; curr++) {
            for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
                if(database[i + 2] != 0) { /*Checks if already dropped*/
                    if(database[i + 1] * database[i + 2] < min) {
                        min = database[i + 1] * database[i + 2];
                        min_idx = i; 
                    } else if(database[i + 1] * database[i + 2] == min) {
                        /*if two values are the same than checks 
                        which assignment # is lower*/
                        if(i < min_idx) {
                            min_idx = i; 
                        }
                    } 
                }
            }
            /*Sets the weight to zero making it "dropped" */
            database[min_idx + 2] = 0;
            min = 99999; /*Default value of min to find new lowest value*/ 
        }
    }
}

double process_numeric_score(int database[]) {
    int i = 0, penalty_per = database[0];
    double numeric_score = 0, total_weight = 0; 
    /*Adjusting scores based on how late it is */
    for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
        database[i + 1] = database[i + 1] - (database[i + 3] * penalty_per);
        if(database[i + 1] < 0) {
            database[i + 1] = 0; 
        }
        /*Finding new total weight taking dropped assignments into account*/
        total_weight += database[i + 2]; 
    }

    /*Calculating score*/
    for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
        numeric_score += database[i + 1] * (database[i + 2] / total_weight);
    }

    return numeric_score;
}

void process_stats(int database[], double *mean, double *std) {
    int i = 0, penalty = database[0], num_assignments = database[2];
    double sum_differences = 0.0; 
    /*Readjusting scores based on how late in unedited database*/
    for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
        database[i + 1] = database[i + 1] - (database[i + 3] * penalty);
        if(database[i + 1] < 0) {
            database[i + 1] = 0; 
        }
    } 

    /*Calculating mean*/
    for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
        if(database[i] != 0) { /*Ignoring empty*/
            *mean += database[i + 1];
        }
    }
    *mean = *mean / num_assignments;

    /*Calculating standard deviation*/
    for(i = 4; i < (MAX_NUMBER * ASSIGNMENT_SIZE + 4); i += 4) {
        if(database[i] != 0) {
            sum_differences += (database[i + 1] - *mean) * (database[i + 1] - *mean);
        }
    }
    *std = sqrt(sum_differences / num_assignments);
    
}
