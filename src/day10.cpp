#include "day10.h"

static const u16 UNEXPLORED_QUEUE_DISTANCE = UINT16_MAX;

struct Machine {
    int indicator_light_length;
    u16 indicator_lights;

    int button_count;
    u16 button_masks[64];
};

static int machine_count;
static Machine machines[256];

static u64 find_min_button_presses_to_solve(Machine *machine) {
    int state_count = 1 << machine->indicator_light_length;

    int read_cursor = 0;
    int write_cursor = 0;

    u16 distances[1 << 16];
    u16 queue[1 << 16];

    for (int i = 0; i < state_count; i++) {
        distances[i] = UNEXPLORED_QUEUE_DISTANCE;
    }

    distances[0] = 0;
    queue[write_cursor++] = 0;

    while (read_cursor != write_cursor) {
        u16 state = queue[read_cursor++];
        u16 d = distances[state];

        if (state == machine->indicator_lights) {
            return d;
        }

        for (int i = 0; i < machine->button_count; i++) {
            u16 next_state = state ^ machine->button_masks[i];

            if (distances[next_state] == UNEXPLORED_QUEUE_DISTANCE) {
                distances[next_state] = d + 1;
                queue[write_cursor++] = next_state;
            }
        }
    }

    Invalid_Code_Path;

    return 0;
}

#pragma warning(push)
#pragma warning(disable: 6054)
u64 day_10_part_1(FILE *file) {
    u64 result = 0;

    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        assert(machine_count < array_count(machines));

        Machine *machine = &machines[machine_count++];

        const char *line_ptr = (const char *)line;

        // Parse indicator light.
        char bits[16 + 1];
        int bit_scan_count = 0;
        sscanf(line_ptr, "[%16[.#]]%n", bits, &bit_scan_count);

        line_ptr += bit_scan_count;

        int bits_length = strlen(bits);

        assert(bits_length > 0);
        assert(bits_length <= 16);

        u16 mask = 0b0;
        for (int i = 0; i < bits_length; i++) {
            if (bits[i] == '#') {
                mask |= (u16)(1 << i);
            } else if (bits[i] == '.') {

            } else assert(0);
        }

        machine->indicator_light_length = bits_length;
        machine->indicator_lights = mask;

        // Parse buttons.
        bool parsing = true;
        while (parsing) {
            char buttons[127 + 1];
            int button_scan_count = 0;
            if (sscanf(line_ptr, " (%127[^)]) %n", buttons, &button_scan_count) != 1) break;

            u16 mask = 0b0;

            const char *tmp = buttons;
            int button_index = 0;
            int button_index_scan_count = 0;
            while (sscanf(tmp, " %d%n", &button_index, &button_index_scan_count) == 1) {
                assert(button_index >= 0);
                assert(button_index < machine->indicator_light_length);

                mask |= (u16)(1 << button_index);
                tmp += button_index_scan_count;

                button_index_scan_count = 0;
                sscanf(tmp, " ,%n", &button_index_scan_count);
                tmp += button_index_scan_count;
            }

            assert(machine->button_count < array_count(machine->button_masks));

            machine->button_masks[machine->button_count++] = mask;

            line_ptr += button_scan_count;
        }
    }

    // Sum min button presses for each machine.
    for (int i = 0; i < machine_count; i++) {
        result += find_min_button_presses_to_solve(&machines[i]);
    }

    return result;
}

u64 day_10_part_2(FILE *file) {
    u64 result = 0;

    return result;
}
#pragma warning(pop)
