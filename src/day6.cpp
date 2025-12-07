#include <assert.h>
#include <stdlib.h>

#include "day6.h"

static const int COLUMN_COUNT = 1000;

struct Value_Row {
    u64 *values;
};

struct Operator_Row {
    char *operators;
};

u64 day_6_part_1(FILE *file) {
    u64 result = 0;

    Value_Row value_rows[4];
    value_rows[0].values = (u64 *)malloc(sizeof(value_rows[0].values)*COLUMN_COUNT);
    value_rows[1].values = (u64 *)malloc(sizeof(value_rows[1].values)*COLUMN_COUNT);
    value_rows[2].values = (u64 *)malloc(sizeof(value_rows[2].values)*COLUMN_COUNT);
    value_rows[3].values = (u64 *)malloc(sizeof(value_rows[3].values)*COLUMN_COUNT);

    Operator_Row operator_row;
    operator_row.operators = (char *)malloc(sizeof(operator_row.operators[0])*COLUMN_COUNT);

    // Parse the value columns.
    int column_index = 0;

    int value_array_index = 0;
    u64 *value_arrays[4] = {
        value_rows[0].values,
        value_rows[1].values,
        value_rows[2].values,
        value_rows[3].values,
    };

    bool parsing = true;
    while (parsing) {
        u64 value;
        fscanf(file, "%I64u", &value);

        value_arrays[value_array_index][column_index++] = value;
        if (column_index == COLUMN_COUNT) {
            column_index = 0;
            value_array_index++;

            if (value_array_index == 4) {
                parsing = false;
            }
        }
    }

    // Parse the operator columns.
    column_index = 0;

    parsing = true;
    while (parsing) {
        char op;
        fscanf(file, "%c", &op);

        if ((op == '*') ||
            (op == '+')) {
            operator_row.operators[column_index++] = op;

            if (column_index == 1000) {
                parsing = false;
            }
        }
    }

    // Calculate the result for each column.
    u64 *column_results = (u64 *)malloc(sizeof(*column_results)*COLUMN_COUNT);

    for (int i = 0; i < COLUMN_COUNT; i++) {
        char op = operator_row.operators[i];

        if (op == '+') {
            column_results[i] = (value_rows[0].values[i] +
                                 value_rows[1].values[i] +
                                 value_rows[2].values[i] +
                                 value_rows[3].values[i]);
        } else if (op == '*') {
            column_results[i] = (value_rows[0].values[i]*
                                 value_rows[1].values[i]*
                                 value_rows[2].values[i]*
                                 value_rows[3].values[i]);
        } else assert(0);
    }

    // Sum results across all columns.
    for (int i = 0; i < COLUMN_COUNT; i++) {
        result += column_results[i];
    }

    return result;
}

u64 day_6_part_2(FILE *file) {
    u64 result = 0;

    return result;
}
