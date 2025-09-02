#ifndef A2L_GENERATOR_H
#define A2L_GENERATOR_H

typedef struct {
    const char* name;
    const char* datatype;
    double minVal;
    double maxVal;
} Variable;

void generate_a2l(const char* filename, const char* ecuName, const Variable* vars, int varCount);

#endif
