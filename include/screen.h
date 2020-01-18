#ifndef __SCREEN_H
#define __SCREEN_H

namespace screen {
    void setup_screen();
    void set_notif_handler(void (*handler)(const char *, const char *));
}

#endif
