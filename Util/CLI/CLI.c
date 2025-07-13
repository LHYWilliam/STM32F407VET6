#include "CLI.h"

void CLI_Init(CLI_t *CLI) { Serial_Printf(CLI->Serial, "\n{CLI}# "); }

void CLI_Handler(CLI_t *CLI) {
    if (CLI->Serial->PackRecieved) {
        Serial_Printf(CLI->Serial, "%s\n", CLI->Serial->StringPack);
        Serial_Printf(CLI->Serial, "{CLI}# ");

        Serial_Clear(CLI->Serial);
    }
}