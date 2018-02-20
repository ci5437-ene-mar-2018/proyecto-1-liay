/*
This program computes the distance from the given start state (i.e. the cost of the least-cost path from the start)
of every state that can be reached from the start state.
It does this by executing Dijktra's algorithm forwards from the start state.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <vector>
#include "priority_queue.hpp"
#include <stdbool.h>

#define  MAX_LINE_LENGTH 999

// GLOBAL VARIABLE(S) FOR STATISTICS
    int64_t nodes_expanded;
/*
int manhattanDistance(state_t *state)
{
    int distancia = 0;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (tablero[i][j] != 0) {
                distancia += Math.abs(((tablero[i][j]-1)/3)-i) + Math.abs(((tablero[i][j]-1)%3)-j);
            }
        }
    }
    return distancia;
}*/

int aStar(state_t *start)
{
    PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *distance = new_state_map(); // contains the cost-from-start for all states that have been generated
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

int main(int argc, char **argv)
{
    char str[MAX_LINE_LENGTH + 1] ;
    ssize_t nchars;
    int d,true_d,nstates;
    state_t start;

    loadPDB();

/* normal operation */

// READ THE START STATE
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

    nodes_expanded = 0;  // initialize global variable
    d = aStar(&start);
    if (d < 0) {
        printf("no solution.  nodes expanded = %lu\n", nodes_expanded);
    } else {
        printf("solution cost = %d, nodes expanded = %lu\n", d, nodes_expanded);
    }

    return 0;
}
