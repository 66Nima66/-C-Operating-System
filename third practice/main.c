#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include "mem.h"

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  duration of the each simulation run.  The third parameter is the
  number of times to repeat each experiment (ie. number of runs). The
  fourth parameter is a random number seed. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 runs with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
*/

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        printf("Usage: ./hw7 memory_size simulation_duration num_runs random_seed\n");
        return 1;
    }

    int memory_size = atoi(argv[1]);
    int simulation_duration = atoi(argv[2]);
    int num_runs = atoi(argv[3]);
    int random_seed = atoi(argv[4]);

    // Add WORSTFIT to mem_strats_t in mem.h
    int strategy_idx;
    for (strategy_idx = 1; strategy_idx < 4; strategy_idx++)
    {
        mem_strats_t strategy = (mem_strats_t) strategy_idx;
        const char* strategy_name[] = {"", "FIRSTFIT", "BESTFIT", "WORSTFIT"};

        srand(random_seed);

        int run;
        int total_fragment_count = 0;
        int total_probes = 0;
        int total_allocation_failures = 0;

        for (run = 0; run < num_runs; run++)
        {
            mem_init(memory_size);

            int time;
            for (time = 0; time < simulation_duration; time++)
            {
                int allocation_size = MIN_REQUEST_SIZE + rand() % (MAX_REQUEST_SIZE - MIN_REQUEST_SIZE + 1);
                int allocation_duration = MIN_DURATION + rand() % (MAX_DURATION - MIN_DURATION + 1);

                int probe_count = mem_allocate(strategy, allocation_size, allocation_duration);
                if (probe_count == -1)
                {
                    total_allocation_failures++;
                }
                else
                {
                    total_probes += probe_count;
                }

                mem_single_time_unit_transpired();
            }

            int frag_size = 10; // Adjust this value to analyze fragmentation for different sizes
            int fragment_count = mem_fragment_count(frag_size);
            total_fragment_count += fragment_count;

            mem_free();
        }

        double average_fragment_count = (double) total_fragment_count / num_runs;
        double average_probes = (double) total_probes / (num_runs * simulation_duration);
        double average_allocation_failures = (double) total_allocation_failures / num_runs;

        printf("Strategy: %s\n", strategy_name[strategy_idx]);
        printf("  - Average external fragmentation: %.2f\n", average_fragment_count);
        printf("  - Average number of probes: %.2f\n", average_probes);
        printf("  - Average number of allocation failures: %.2f\n", average_allocation_failures);
    }

    return 0;
}