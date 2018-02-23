/*
aStar.cpp
Universidad Simon Bolivar
CI5437: Inteligencia Artificial I

Autores: 
  - Yarima Luciani 13-10770
  - Lautaro Villalon 12-10427

Equipo: LIAY
Prof. Blai Bonet
Ene-Mar 2018
*/

#include <vector>
#include "priority_queue.hpp"
#include <stdbool.h>
#include <fstream>
#include <string>
#include <time.h>


#define  MAX_LINE_LENGTH 999

using namespace std;

int64_t nodes_expanded;

int aStar(state_t *start)
{
    PriorityQueue<state_t> open;
    state_map_t *distance = new_state_map();
    state_t state, child; 
    int g, ruleid;
    ruleid_iterator_t iter;


    /* add start state */
    state_map_add( distance, start, 0 );
    open.Add(0, 0, *start );
    
    while(!open.Empty()) {

        /* get current distance from start */
        g = open.CurrentPriority();

        /* get state */
        state = open.Top();
        open.Pop();

        int *old_distance = state_map_get(distance, &state);
        ++nodes_expanded;

        if ((old_distance == NULL) || (g < *old_distance) || (!compare_states(&state, start))) {
            state_map_add( distance, &state, g );

            if (is_goal(&state)) {
                return g;
            }
            init_fwd_iter( &iter, &state );
            while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {
                apply_fwd_rule( ruleid, &state, &child );

                int child_g = g + get_fwd_rule_cost( ruleid );

                int child_h = heuristic(child);

                int child_f = child_g + child_h;

                if (child_h < INT_MAX) {
                    open.Add( child_f, child_g, child );

                }
            }


        }
    }

    return -1;
}

//TEST FILE READER
int main(int argc, char **argv)
{
    char str[MAX_LINE_LENGTH + 1];
    string line;
    int d;
    ssize_t nchars;
    float runTime;
    state_t start;
    ifstream file;
    clock_t startTime, endTime, timeSpan;


    loadPDB();


    printf("Please enter a test file followed by ENTER: ");
    if (fgets(str, sizeof str, stdin) == NULL) {
        printf("Error: empty input line.\n");
        return 0;
    }


    str[strlen(str)-1] = '\0';

    file.open(str);

    if (!file.is_open()) {
        printf("Error: Invalid filename:%s\n", str);
        return -1;
    }

    printf("Instance \t\t\t Solved   Time \t   Nodes Expanded   Distance\t\n");
    printf("-----------------------------------------------------------------------------\n");
    
    while (!file.eof()) {
        getline(file, line);
        nchars = read_state(line.c_str(), &start);
        if (nchars <= 0) {
            printf("Error: invalid state entered.\n");
            continue;
        }

        startTime = clock();
        nodes_expanded = 0; 

        // A STAR BEGINS 
        d = aStar(&start);
        // A STAR ENDS

        endTime = clock();

        timeSpan = endTime - startTime;

        runTime = (timeSpan / (double) CLOCKS_PER_SEC) / 60;

        if (d < 0) {
            printf("%s \t False \t %f \t %ld \t\t %d\n", line.c_str(), runTime, nodes_expanded, d);
        } else {
            printf("%s \t True \t %f \t %ld \t\t %d\n", line.c_str(), runTime, nodes_expanded, d);
        }
        
    }

    file.close();

    return 0;
}


/* STATE READER

int main(int argc, char **argv)
{
    char str[MAX_LINE_LENGTH + 1] ;
    ssize_t nchars;
    int d,true_d,nstates;
    state_t start;

    loadPDB();


    printf("Please enter a state followed by ENTER: ");
    if (fgets(str, sizeof str, stdin) == NULL) {
        printf("Error: empty input line.\n");
        return 0;
    }
    nchars = read_state(str, &start);
    if (nchars <= 0) {
        printf("Error: invalid state entered.\n");
        return 0;
    }

    printf("The state you entered is: ");
    print_state(stdout, &start);
    printf("\n");

    nodes_expanded = 0;  
    d = aStar(&start);
    if (d < 0) {
        printf("no solution.  nodes expanded = %lu\n", nodes_expanded);
    } else {
        printf("solution cost = %d, nodes expanded = %lu\n", d, nodes_expanded);
    }

    return 0;
}

*/
