/*
This program computes the distance from the given start state (i.e. the cost of the least-cost path from the start)
of every state that can be reached from the start state.
It does this by executing Dijktra's algorithm forwards from the start state.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <vector>
#include "priority_queue.hpp"
#include "node.hpp"
#include <stdbool.h>
#include <algorithm>

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
}

unsigned heuristic(state_t *state) {
    return 0;
}

*/
Node* dfsVisit(Node *n, unsigned bound, unsigned *nextBound, const int history) {
    unsigned f = (*n).g + heuristic((*n).state); 
    unsigned t = INT_MAX;
    int ruleid;
    int c_history;
    ruleid_iterator_t iter;
    state_t child;
    Node *next;

    if (f > bound) {
        *nextBound = f;
        return NULL;
    }

    if((*n).parent != NULL) {
        print_state(stdout, &(*(*n).parent).state);
        printf("\n");
    }

    if (is_goal(&(*n).state)) {
        *nextBound = (*n).g;
        return n;
    }

    ++nodes_expanded;

    init_fwd_iter(&iter, &(*n).state);
    while((ruleid = next_ruleid(&iter)) >= 0) {
        apply_fwd_rule(ruleid, &(*n).state, &child);

        if( !fwd_rule_valid_for_history( history, ruleid ) )
            continue;
        c_history = next_fwd_history( history, ruleid );

        Node n_child(child, n, ((*n).g + 1));

        next = dfsVisit(&n_child, bound, nextBound, c_history);

        if (next != NULL) {
            return next;
        }

        t = std::min(t, *nextBound);

    }

    *nextBound = t;
    return NULL;
}

Node idaStar(state_t *start)
{   
    Node root(*start, NULL, 0);
    unsigned bound = heuristic(*start); 
    unsigned nextBound;
    Node *final;

    while (1) {
        final = dfsVisit(&root, bound, &nextBound, init_history);

        if (final != NULL) {
            return *final;
        }
        bound = nextBound;
    }

}

int main(int argc, char **argv)
{
    char str[MAX_LINE_LENGTH + 1] ;
    char goalStr[256];
    ssize_t nchars;
    state_t start;

    loadPDB();

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
    Node goal = idaStar(&start);
    printf("nodes expanded = %lu, solution state:\n", nodes_expanded);
    print_state(stdout, &(goal).state);
    printf("DISTANCE: %d", goal.g);
    printf("\n");
    return 0;
}
