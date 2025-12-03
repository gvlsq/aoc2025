#include <math.h>

#include "day2.h"

bool get_char_at(u64 number, int index, char *result) {
    if (number < (u64)powf((float)10, index)) {
        return false;
    }

    u64 dividend = number % (u64)powf((float)10, index + 1);
    u64 divisor = dividend / (u64)powf((float)10, index);

    *result = '0' + divisor;

    return true;
}

int get_number_length(u64 number) {
    int index = 0;

    for (;;) {
        char c;
        if (get_char_at(number, index, &c)) {
            index++;
        } else {
            break;
        }
    }

    return index;
}

u64 day_2_part_1(FILE *file) {
    u64 sum = 0;

    bool parsing = true;
    while (parsing) {
        u64 minimum;
        u64 maximum;
        if (fscanf(file, "%I64u-%I64u,\n", &minimum, &maximum) == 2) {
            for (u64 number = minimum; number <= maximum; number++) {
                int number_length = get_number_length(number);
                if ((number_length % 2) != 0) continue;

                bool failed = false;

                for (int i = 0; i < (number_length / 2); i++) {
                    char first_half_digit;
                    get_char_at(number, i, &first_half_digit);

                    char second_half_digit;
                    get_char_at(number, (number_length / 2) + i, &second_half_digit);

                    if (first_half_digit != second_half_digit) {
                        failed = true;
                        break;
                    }
                }

                if (!failed) {
                    sum += number;
                }
            }
        } else {
            parsing = false;
        }
    }

    return sum;
}

u64 day_2_part_2(FILE *file) {
    u64 sum = 0;

    bool parsing = true;
    while (parsing) {
        u64 minimum;
        u64 maximum;
        if (fscanf(file, "%I64u-%I64u,\n", &minimum, &maximum) == 2) {
            for (u64 number = minimum; number <= maximum; number++) {
                int number_length = get_number_length(number);

                bool all_same = true;
                for (int i = 0; i < number_length - 1; i++) {
                    char a;
                    get_char_at(number, i, &a);

                    char b;
                    get_char_at(number, i + 1, &b);

                    if (a != b) {
                        all_same = false;
                        break;
                    }
                }

                if ((number_length > 1) && all_same) {
                    sum += number;
                } else {
                    for (int i = 2; i < number_length; i++) { // all_same covers the == case
                        if ((number_length % i) == 0) {
                            bool failed = false;

                            for (int j = 0; !failed && (j < i); j++) { // Loop over groups
                                for (int k = 0; k < ((number_length / i) - 1); k++) { // Loop over digits in the group
                                    char digit;
                                    get_char_at(number, k*i + j, &digit);

                                    char digit_in_next_group;
                                    get_char_at(number, (k + 1)*i + j, &digit_in_next_group);

                                    if (digit != digit_in_next_group) {
                                        failed = true;
                                        break;
                                    }
                                }
                            }

                            if (!failed) {
                                sum += number;
                                break;
                            }
                        }
                    }
                }
            }
        } else {
            parsing = false;
        }
    }

    return sum;
}
