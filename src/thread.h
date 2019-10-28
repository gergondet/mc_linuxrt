#pragma once

static bool MC_LINUXRT_RUN = true;

void * init(int argc, char * argv[]);

void run(void * data);
