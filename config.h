/* config.h */

//========================================================= 
// If you get a warining notification about seat
// 1. add user to input group.
//
// sudo usermod -aG input $USER
//
// 2. logout dwm/system and login
// 3. check in terminal user is added in input group
//
// groups
// 
// 4. loginctl list-seats' or loginctl command to find seat
//==========================================================

struct GestureMap {
    int fingers;
    const char *direction;
    const char *command;
};

/* Higher Threshold is less sentivie [change and find your sweet spot] */
static const double maxThreshold = 30.0;
static const double minThreshold = 15.0;

/* Preferred repeat speed (max 150) */
static const int cooldownMs = 100;

static const struct GestureMap config[] = {
    // 3-Finger Swipes
    {3, "right", "xdotool key Alt_L+Right  &"},
    {3, "left",  "xdotool key Alt_L+Left   &"},
    {3, "up",    "xdotool key Alt_L+Right  &"},
    {3, "down",  "xdotool key Alt_L+Left   &"},

    // 4-Finger Swipes
    {4, "up",    "xdotool key Alt_L+Right  &"},
    {4, "down",  "xdotool key Alt_L+Left   &"},
};
