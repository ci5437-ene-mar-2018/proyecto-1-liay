/*
idaStar.cpp
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
#include "node.hpp"
#include <stdbool.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <time.h>

#define  MAX_LINE_LENGTH 999

using namespace std;

int64_t nodes_expanded;

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

// TEST FILE READER
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
        Node goal = idaStar(&start);
        // A STAR ENDS

        endTime = clock();

        timeSpan = endTime - startTime;

        runTime = (timeSpan / (double) CLOCKS_PER_SEC) / 60;

        printf("%s \t True \t %f \t %ld \t\t %d\n", line.c_str(), runTime, nodes_expanded, goal.g);
        
    }

    file.close();

    return 0;
}


/* STATE READER
int main(int argc, char **argv)
{
    char str[MAX_LINE_LENGTH + 1] ;
    char goalStr[256];
    ssize_t nchars;
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
    Node goal = idaStar(&start);
    printf("nodes expanded = %lu, solution state:\n", nodes_expanded);
    print_state(stdout, &(goal).state);
    printf("DISTANCE: %d", goal.g);
    printf("\n");
    return 0;
}

*/