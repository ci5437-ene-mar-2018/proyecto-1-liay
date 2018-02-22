#include <vector>
#include "priority_queue.hpp"
#include <stdbool.h>
#include <unordered_map>

#define  MAX_LINE_LENGTH 999
using namespace std;

int bfs(state_t *goal, int pruning)
{
    PriorityQueue<state_t> open; 
    state_map_t *historyMap = new_state_map();
    state_t state, child; 
    int depth, ruleid, history, c_history;
    float b_factor;
    ruleid_iterator_t iter;
    unordered_map<int, int> nodesPerDepth;
    int *new_history;
    history = init_history;


    /* add goal state */
    open.Add(0, 0, *goal);
    if (pruning)
        state_map_add( historyMap, goal, history );

    depth = open.CurrentPriority();
    
    while(!open.Empty()) {

        if (depth == open.CurrentPriority()) {
            nodesPerDepth[depth] = nodesPerDepth[depth] + 1;
        } else if (depth==0){
            printf("\n%d\t%d\t", depth, nodesPerDepth[depth]);
            depth = open.CurrentPriority();
            nodesPerDepth[depth] = 1;
        } else {
            b_factor = nodesPerDepth[depth]/nodesPerDepth[depth-1];
            printf("%f\n%d\t%d\t", b_factor, depth, nodesPerDepth[depth]);
            depth = open.CurrentPriority();
            nodesPerDepth[depth] = 1;
        }

        /* get state */
        state = open.Top();
        open.Pop();

        if (pruning) {
            new_history = state_map_get(historyMap, &state);
            history = *new_history;
        }

        init_bwd_iter( &iter, &state );
        while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {
            apply_bwd_rule( ruleid, &state, &child );

            if (pruning) {

                if( !fwd_rule_valid_for_history( history, ruleid ) )
                    continue;

                c_history = next_fwd_history( history, ruleid );
                state_map_add( historyMap, &child, c_history );


            }

            int child_g = depth + get_bwd_rule_cost( ruleid );

            open.Add( child_g, child_g, child );

        }

    }

    return -1;
}

int main(int argc, char **argv)
{
    state_t goal;
    int finished, goal_id, pruning;

    if (argc != 2) {
        printf("Please enter 1 for parcial pruning or 0 for no pruning.\n\n");
        return -1;
    }

    pruning = atoi(argv[1]);

    if (pruning != 0 && pruning != 1) {
        printf("Please enter 1 for parcial pruning or 0 for no pruning.\n\n");
        return -1;
    }

    printf("Depth\t#Nodes\tBranching Factor\n");
    printf("------------------------------------------------------------------\n");

    first_goal_state(&goal, &goal_id);

    finished = bfs(&goal, pruning);

    if (finished) {
        printf("THE END?");
    }

    return 0;
}
