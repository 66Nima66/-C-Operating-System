#include <stdio.h>    /* for printf statements when debugging */
#include <stdlib.h>   /* for malloc() and free() */
#include "mem.h"

/*
  Physical memory array. This is a static global array for all
  functions in this file.  An element in the array with a value of
  zero represents a free unit of memory.
*/
static dur_t* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is
 a static global variable used by functions in this file.
 */
static int mem_size;


/*
 The last_placement_position variable contains the end position of the
 last allocated unit used by the next fit placement algorithm.
 */
static int last_placement_position;

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
 */
int mem_allocate(mem_strats_t strategy, int size, dur_t duration)
{
    int start = 0;
    int blocks_probed = 0;
    int best_fit = -1;
    int best_fit_size = mem_size + 1;
    int current_fit = -1;
    int current_fit_size = 0;
    int i;

    for (i = 0; i < mem_size; i++)
    {
        if (memory[i] == 0)
        {
            if (current_fit == -1)
            {
                current_fit = i;
                current_fit_size = 1;
                blocks_probed++;
            }
            else
            {
                current_fit_size++;
            }
        }
        else
        {
            if (current_fit != -1)
            {
                if (strategy == BESTFIT && current_fit_size >= size && current_fit_size < best_fit_size)
                {
                    best_fit = current_fit;
                    best_fit_size = current_fit_size;
                    if (current_fit_size == size) break;
                }
                else if (strategy == FIRSTFIT && current_fit_size >= size)
                {
                    break;
                }
                current_fit = -1;
            }
        }

        if (strategy == NEXTFIT && last_placement_position > 0 && i >= last_placement_position)
        {
            break;
        }
    }

    if (strategy == NEXTFIT && current_fit == -1)
    {
        i = 0;
        for (; i < last_placement_position; i++)
        {
            if (memory[i] == 0)
            {
                if (current_fit == -1)
                {
                    current_fit = i;
                    current_fit_size = 1;
                    blocks_probed++;
                }
                else
                {
                    current_fit_size++;
                }
            }
            else
            {
                if (current_fit != -1)
                {
                    if (current_fit_size >= size)
                    {
                        break;
                    }
                    current_fit = -1;
                }
            }
        }
    }

    if (strategy == BESTFIT)
    {
        current_fit = best_fit;
        current_fit_size = best_fit_size;
    }

    if (current_fit != -1 && current_fit_size >= size)
    {
        for (i = current_fit; i < current_fit + size; i++)
        {
            memory[i] = duration;
        }
        if (strategy == NEXTFIT && current_fit != -1)
        {
            last_placement_position = current_fit + size;
        }
        return blocks_probed;
    }

    return -1;
}


/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.  NOTE: when a
  memory cell is decremented to zero, it becomes "unallocated".
 */
int mem_single_time_unit_transpired()
{
    int i;
    for (i = 0; i < mem_size; i++)
    {
        if (memory[i] > 0)
        {
            memory[i]--;
        }
    }
    return 0;
}


/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size)
{
    int i;
    int fragment_count = 0;
    int current_free_block_size = 0;

    for (i = 0; i < mem_size; i++)
    {
        if (memory[i] == 0)
        {
            current_free_block_size++;
        }
        else
        {
            if (current_free_block_size > 0 && current_free_block_size <= frag_size)
            {
                fragment_count++;
            }
            current_free_block_size = 0;
        }
    }

    if (current_free_block_size > 0 && current_free_block_size <= frag_size)
    {
        fragment_count++;
    }

    return fragment_count;
}


/*
  Set the value of zero to all entries of memory.
 */
void mem_clear()
{
    int i;
    for (i = 0; i < mem_size; i++)
    {
        memory[i] = 0;
    }
}


/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init(int size)
{
  memory = malloc(sizeof(dur_t)*size);
  mem_size = size;
}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free()
{
  free(memory);
}

/*
  Print memory for testing/debugging purposes.  This is an optional
  routine to write, but highly recommended!  You will need to test and
  debug your allocation algorithms.  Calling this routine in your
  main() after every allocation (successful or not) will help in this
  endeavor.  NOTE: you should print the memory contents in contiguous
  blocks, rather than single units; otherwise, the output will be very
  long.
 */
void mem_print()
{
    int i;
    int current_block_start = -1;
    int current_block_size = 0;

    for (i = 0; i < mem_size; i++)
    {
        if (memory[i] != 0)
        {
            if (current_block_start == -1)
            {
                current_block_start = i;
                current_block_size = 1;
            }
            else
            {
                current_block_size++;
            }
        }
        else
        {
            if (current_block_start != -1)
            {
                printf("Block from %d to %d, size: %d, duration: %d\n",
                       current_block_start, current_block_start + current_block_size - 1,
                       current_block_size, memory[current_block_start]);
                current_block_start = -1;
            }
        }
    }

    if (current_block_start != -1)
    {
        printf("Block from %d to %d, size: %d, duration: %d\n",
               current_block_start, current_block_start + current_block_size - 1,
               current_block_size, memory[current_block_start]);
    }
}
