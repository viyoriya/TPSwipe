#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>

/* Dynamic Header Selection for systemd and runit */
#ifdef USE_ELOGIND
    #include <elogind/sd-login.h>
#else
    #include <systemd/sd-login.h>
#endif

#include "config.h"

static int open_restricted(const char *path, int flags, void *user_data) {
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}
static void close_restricted(int fd, void *user_data) { close(fd); }
static const struct libinput_interface interface = { .open_restricted = open_restricted, .close_restricted = close_restricted };

int main() {
    char *session = NULL;
    char *seatName = NULL;

    /* 1. Detect Seat */
    if (sd_pid_get_session(0, &session) < 0 || sd_session_get_seat(session, &seatName) < 0) {
        if (seatName) free(seatName);
        seatName = strdup("seat0");
    }

    /* 2. Initialize libinput context */
    struct udev *udev = udev_new();
    struct libinput *li = libinput_udev_create_context(&interface, NULL, udev);

    /* 3. PERMISSION CHECK (Single Notification) */
    /* libinput_udev_assign_seat returns 0 on success. 
       If it fails here, we notify once and terminate. */
    if (libinput_udev_assign_seat(li, seatName) != 0) {
        system("notify-send -u critical 'tpSwipe Error' 'Permission Denied: Check if user is in input group'");
        
        // Cleanup before exit
        if (session) free(session);
        if (seatName) free(seatName);
        if (li) libinput_unref(li);
        if (udev) udev_unref(udev);
        return 1; 
    }

    /* 4. MAIN LOOP */
    struct libinput_event *ev;
    double total_dx = 0, total_dy = 0;
    struct timespec last_trigger = {0, 0};

    while (libinput_dispatch(li) == 0) {
        while ((ev = libinput_get_event(li))) {
            enum libinput_event_type type = libinput_event_get_type(ev);

            if (type >= LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN && type <= LIBINPUT_EVENT_GESTURE_SWIPE_END) {
                struct libinput_event_gesture *gev = libinput_event_get_gesture_event(ev);

                if (type == LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN) {
                    total_dx = 0; total_dy = 0;
                }
                else if (type == LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE) {
                    double dx = libinput_event_gesture_get_dx(gev);
                    double dy = libinput_event_gesture_get_dy(gev);
                    total_dx += dx;
                    total_dy += dy;

                    /* ACCELERATION LOGIC: Calculate current speed */
                    double velocity = sqrt(dx*dx + dy*dy);

                    /* Adjust threshold: faster movement = smaller threshold */
                    double currentThreshold = maxThreshold - (velocity * 1.0);
                    if (currentThreshold < minThreshold) currentThreshold = minThreshold;

                    double totalDist = sqrt(total_dx*total_dx + total_dy*total_dy);

                    if (totalDist > currentThreshold) {
                        struct timespec now;
                        clock_gettime(CLOCK_MONOTONIC, &now);
                        long elapsed = (now.tv_sec - last_trigger.tv_sec) * 1000 + (now.tv_nsec - last_trigger.tv_nsec) / 1000000;

                        if (elapsed > cooldownMs) {
                            int fingers = libinput_event_gesture_get_finger_count(gev);
                            const char *dir = (fabs(total_dx) > fabs(total_dy)) ? (total_dx > 0 ? "right" : "left") : (total_dy > 0 ? "down" : "up");

                            for (unsigned int i = 0; i < sizeof(config) / sizeof(config[0]); i++) {
                                if (config[i].fingers == fingers && strcmp(config[i].direction, dir) == 0) {
                                    system(config[i].command);
                                }
                            }
                            last_trigger = now;
                            total_dx = 0; total_dy = 0;
                        }
                    }
                }
            }
            libinput_event_destroy(ev);
        }
        usleep(5000);
    }

    free(session);
    free(seatName);
    libinput_unref(li);
    udev_unref(udev);
    return 0;
}
