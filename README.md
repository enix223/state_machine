# State machine

State machine implement in pure C

## Usage

1. Initialize a machine

    ```c
    // define states
    typedef enum {
        STATE_GREEN,
        STATE_YELLOW,
        STATE_RED,
    } tranffic_light_state_t;

    // define transitions
    typedef enum {
        TRANSITION_STOP,
        TRANSITION_GO,
    } traffic_light_transition_t;

    // declare a state machine
    csm_state_machine_t machine;

    // initialize the machine
    csm_machine_initialize(&machine, STATE_GREEN);

    // register state change callback
    csm_machine_register_on_state_changed(&machine, on_state_changed);
    ```

2. Define transitions

    ```c
    // define transitions
    // GREEN --[STOP]--> YELLOW
    csm_state_transition_node_t trans_node1 = {
        .from_state = STATE_GREEN,
        .transition = TRANSITION_STOP,
        .to_state = STATE_YELLOW,
    };
    csm_machine_define_state_transition(&machine, &trans_node1);

    // YELLOW --[STOP]--> RED
    csm_state_transition_node_t trans_node2 = {
        .from_state = STATE_YELLOW,
        .transition = TRANSITION_STOP,
        .to_state = STATE_RED,
    };
    csm_machine_define_state_transition(&machine, &trans_node2);

    // RED --[GO]--> YELLOW
    csm_state_transition_node_t trans_node3 = {
        .from_state = STATE_RED,
        .transition = TRANSITION_GO,
        .to_state = STATE_YELLOW,
    };
    csm_machine_define_state_transition(&machine, &trans_node3);

    // YELLOW --[GO]--> GREEN
    csm_state_transition_node_t trans_node4 = {
        .from_state = STATE_YELLOW,
        .transition = TRANSITION_GO,
        .to_state = STATE_GREEN,
    };
    csm_machine_define_state_transition(&machine, &trans_node4);
    ```

3. Start state machine

    ```c
    // start machine
    csm_machine_start(&machine);
    ```

4. Trigger a transition

    ```c
    // From GEEEN transit to YELLOW
    csm_machine_transit(&machine, TRANSITION_STOP);
    // From YELLOW transit to RED
    csm_machine_transit(&machine, TRANSITION_STOP);
    // From RED transit to YELLOW
    csm_machine_transit(&machine, TRANSITION_GO);
    // From YELLOW transit to GREEN
    csm_machine_transit(&machine, TRANSITION_GO);
    ```

5. Stop machine

    ```c
    csm_machine_stop(&machine);
    ```

For more example, please refer to `test/state_machine_test.c`
