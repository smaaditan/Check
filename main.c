#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "a2l_generator.h"

// XCPlite headers
#include "xcp.h"        // from third_party/XCPlite/inc
#include "xcp_cfg.h"    // default config can stay; we’ll override via API where possible
#include "xcptl_cfg.h"  // transport layer config (UDP/TCP)

#define OPTION_PROJECT_NAME "sample_ecu_demo"
#define OPTION_USE_TCP 0   // 0 = UDP, 1 = TCP
#define OPTION_SERVER_PORT 5555
#define OPTION_SERVER_ADDR {192,168,228,92}  // your requested bind IP

// Demo variables (you can later expose them through XCPlite’s instrumentation macros too)
static uint16_t rpm = 900;
static uint8_t  throttle = 5;
static int16_t  ignition_timing = 5;

static void generate_demo_a2l(void) {
    Variable vars[] = {
        {"rpm", "UWORD", 0, 8000},
        {"throttle", "UBYTE", 0, 100},
        {"ignition_timing", "SWORD", -90, 90}
    };
    generate_a2l("sample_ecu_demo.a2l", OPTION_PROJECT_NAME, vars, 3);
}

int main(void) {
    printf("🚗 Sample ECU Demo (XCPlite-based) starting…\n");

    // 1) Write a small A2L so you can import in CANape right away
    generate_demo_a2l();
    printf("✅ Wrote A2L: sample_ecu_demo.a2l\n");

    // 2) Configure transport (bind address + port)
    uint8_t ip[4] = OPTION_SERVER_ADDR;
    char bindIp[32];
    snprintf(bindIp, sizeof(bindIp), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    printf("🔊 Binding XCP on %s:%d over %s …\n",
           bindIp, OPTION_SERVER_PORT, OPTION_USE_TCP ? "TCP" : "UDP");

    // XCPlite init sequence:
    // Many examples inside XCPlite start the server and then poll a task function.
    // Here we use the same pattern as the 'hello_xcp' demo.
    if (!xcp_init_server(bindIp, OPTION_SERVER_PORT, OPTION_USE_TCP)) {
        fprintf(stderr, "❌ xcp_init_server failed\n");
        return 1;
    }
    printf("✅ XCP server up. Open CANape device to connect (A2L IP: %s, port: %d)\n",
           bindIp, OPTION_SERVER_PORT);

    // Demo “runtime”: periodically tweak variables and let XCPlite handle requests
    for (;;) {
        // trivial dummy changes so you can see values move in CANape
        rpm = (rpm + 17) % 8001;
        throttle = (throttle + 1) % 101;
        ignition_timing = (ignition_timing >= 89) ? -90 : (ignition_timing + 1);

        // XCPlite polling / packet processing
        xcp_server_task();
        // Small sleep inside XCPlite or here (depends on cfg); no busy loop needed.
    }

    // (Normally unreachable)
    xcp_shutdown_server();
    return 0;
}
