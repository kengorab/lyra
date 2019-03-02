#include <stdlib.h>

#include "lines.h"

void initLines(Lines* lines) {
    lines->capacity = 0;
    lines->count = 0;
    lines->lines = NULL;
}

void freeLines(Lines* lines) {
    free(lines->lines);
    initLines(lines);
}

void writeLines(Lines* lines, int line) {
    if (lines->capacity < lines->count + 1) {
        int oldCapacity = lines->capacity;
        lines->capacity = oldCapacity < 8 ? 8 : oldCapacity * 2;
        lines->lines = realloc(lines->lines, (size_t) lines->capacity);
    }

    lines->lines[lines->count++] = line;
}

void addLine(Lines* lines, int line) {
    // If we haven't added any lines yet, or if we have and we're starting a new
    // line, insert...
    if (lines->count == 0 || lines->lines[lines->count - 2] != line) {
        writeLines(lines, line);
        writeLines(lines, 1);
    } else {
        // ...otherwise, increment the count for the currently-tracked line.
        lines->lines[lines->count - 1]++;
    }
}

int getLineForOffset(Lines* lines, int offset, bool* isStartOfLine) {
    int instrNum = offset + 1;

    int idx = 0;
    int lineNum = lines->lines[idx];
    int lineFreq = lines->lines[idx + 1];

    while (instrNum > lineFreq) {
        instrNum -= lineFreq;
        idx += 2;
        lineNum = lines->lines[idx];
        lineFreq = lines->lines[idx + 1];
    }

    *isStartOfLine = instrNum == 1;
    return lineNum;
}
