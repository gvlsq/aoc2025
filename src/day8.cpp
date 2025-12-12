#include <assert.h>
#include <float.h>
#include <math.h>

#include "day8.h"

struct Vector3u {
    u64 x;
    u64 y;
    u64 z;
};
struct Junction_Box {
    Vector3u position;
};

struct Direct_Connection {
    Junction_Box *a;
    Junction_Box *b;
};

enum Circuit_State {
    CIRCUIT_STATE_INACTIVE,
    CIRCUIT_STATE_ACTIVE,
};
struct Circuit {
    Circuit_State state;

    int direct_connection_count;
    Direct_Connection direct_connections[1000];

    int junction_box_count;
    Junction_Box *junction_boxes[1000];
};

static int junction_box_count;
static Junction_Box junction_boxes[1000];

static Circuit circuits[1000];

static inline u64 squareu(u64 u) {
    return u*u;
}

static Circuit *get_circuit(Junction_Box *junction_box) {
    for (int i = 0; i < array_count(circuits); i++) {
        Circuit *circuit = &circuits[i];
        if (circuit->state == CIRCUIT_STATE_INACTIVE) continue;

        for (int j = 0; j < circuit->junction_box_count; j++) {
            if (circuit->junction_boxes[j] == junction_box) {
                return circuit;
            }
        }
    }

    return 0;
}

static bool test_direct_connection(Junction_Box *a, Junction_Box *b) {
    for (int i = 0; i < array_count(circuits); i++) {
        Circuit *circuit = &circuits[i];
        if (circuit->state == CIRCUIT_STATE_INACTIVE) continue;

        for (int j = 0; j < circuit->direct_connection_count; j++) {
            Direct_Connection *connection = &circuit->direct_connections[j];

            if (((connection->a == a) && (connection->b == b)) ||
                ((connection->a == b) && (connection->b == a))) {
                return true;
            }
        }
    }

    return false;
}

static bool test_in_same_circuit(Junction_Box *a, Junction_Box *b) {
    Circuit *circuit_a = get_circuit(a);
    Circuit *circuit_b = get_circuit(b);

    return circuit_a == circuit_b;
}

static void add_junction_box(Circuit *circuit, Junction_Box *junction_box) {
    circuit->junction_boxes[circuit->junction_box_count++] = junction_box;
}

static void add_direct_connection(Circuit *circuit, Junction_Box *a, Junction_Box *b) {
    Direct_Connection *new_connection = &circuit->direct_connections[circuit->direct_connection_count++];
    new_connection->a = a;
    new_connection->b = b;
}

static void connect_junction_boxes(Junction_Box *a, Junction_Box *b) {
    Circuit *circuit_a = get_circuit(a);
    Circuit *circuit_b = get_circuit(b);

    // "Because these two junction boxes were already in the same circuit, nothing happens!"
    if (circuit_a == circuit_b) {
        add_direct_connection(circuit_a, a, b);
        return;
    }

    // Add all of circuit B's junction boxes to circuit A. This should include junction box B itself!
    bool found_b = false;

    for (int i = 0; i < circuit_b->junction_box_count; i++) {
        Junction_Box *junction_box = circuit_b->junction_boxes[i];

        if (junction_box == b) found_b = true;

        add_junction_box(circuit_a, junction_box);
    }

    assert(found_b);

    // Add a direct connection between junction boxes A and B in circuit A.
    add_direct_connection(circuit_a, a, b);

    // Add all of circuit B's other direct connections to circuit A.
    for (int i = 0; i < circuit_b->direct_connection_count; i++) {
        Direct_Connection *existing_connection = &circuit_b->direct_connections[i];
        
        add_direct_connection(circuit_a, existing_connection->a, existing_connection->b);
    }

    // Deactivate circuit B.
    circuit_b->state = CIRCUIT_STATE_INACTIVE;
}

u64 day_8_part_1(FILE *file) {
    u64 result = 0;

    // Parse in junction boxes.
    {
        int junction_box_index = 0;
        bool parsing = true;
        while (parsing) {
            u64 x;
            u64 y;
            u64 z;
            if (fscanf(file, "%I64u,%I64u,%I64u\n", &x, &y, &z) == 3) {
                junction_boxes[junction_box_index++] = {{x, y, z}};
            } else {
                parsing = false;
            }
        }

        junction_box_count = junction_box_index;
    }

    // Set up initial circuits and the junction boxes inside of them.
    for (int i = 0; i < junction_box_count; i++) {
        Circuit *circuit = &circuits[i];
        circuit->state = CIRCUIT_STATE_ACTIVE;

        add_junction_box(circuit, &junction_boxes[i]);

        assert(circuit->direct_connection_count == 0);
    }

    // Connect junction box pairs.
    int pair_count = 0;
    while (pair_count < 1000) {
        Junction_Box *amin = 0;
        Junction_Box *bmin = 0;

        double dmin = DBL_MAX;

        for (int i = 0; i < junction_box_count - 1; i++) {
            Junction_Box *a = &junction_boxes[i];

            for (int j = i + 1; j < junction_box_count; j++) {
                Junction_Box *b = &junction_boxes[j];

                double d = sqrt((double)squareu(a->position.x - b->position.x) +
                                (double)squareu(a->position.y - b->position.y) +
                                (double)squareu(a->position.z - b->position.z));
                if (d < dmin) {
                    bool directly_connected = test_direct_connection(a, b);
                    if (directly_connected) continue;

                    amin = a;
                    bmin = b;

                    dmin = d;
                }
            }
        }
       
        assert(amin && bmin);

        connect_junction_boxes(amin, bmin);
        pair_count++;
    }

    // Find and multiply counts for the three largest circuits.
    int i0 = 0;
    int i0_count = INT32_MIN;

    int i1 = 0;
    int i1_count = INT32_MIN;

    int i2 = 0;
    int i2_count = INT32_MIN;

    for (int i = 0; i < array_count(circuits); i++) {
        Circuit *circuit = &circuits[i];
        if (circuit->state == CIRCUIT_STATE_INACTIVE) continue;

        if (circuit->junction_box_count > i0_count) {
            i2 = i1;
            i2_count = i1_count;

            i1 = i0;
            i1_count = i0_count;

            i0 = i;
            i0_count = circuit->junction_box_count;
        } else if (circuit->junction_box_count > i1_count) {
            i2 = i1;
            i2_count = i1_count;

            i1 = i;
            i1_count = circuit->junction_box_count;
        } else if (circuit->junction_box_count > i2_count) {
            i2 = i;
            i2_count = circuit->junction_box_count;
        }
    }

    result = i0_count*i1_count*i2_count;

    return result;
}

u64 day_8_part_2(FILE *file) {
    u64 result = 0;

    return result;
}
