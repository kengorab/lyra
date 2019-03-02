#ifndef CLYRA_LINES_H
#define CLYRA_LINES_H

#include <stdbool.h>

/**
 * A compressed representation of a mapping between bytecode instructions and
 * the line of code to which they correspond in the precompiled source by
 * storing consecutive tuples of the amount of the line number and the number of
 * instructions that correspond to that line number.
 *
 * For example, `1 12 2 2 3 6` should be grouped as (1, 12), (2, 2), (3, 6),
 * meaning the first 12 instructions correspond to line 1, the next 2 to line 2,
 * the next 6 to line 3, and so on.
 */
typedef struct {
    int capacity;
    int count;
    int* lines;
} Lines;

void initLines(Lines* lines);

void freeLines(Lines* lines);

void writeLines(Lines* lines, int line);

void addLine(Lines* lines, int line);

int getLineForOffset(Lines* lines, int offset, bool* isStart);

#endif //CLYRA_LINES_H
