#ifndef WM8731_H
#define WM8731_H

#define WM8731_MSTR 0
#define WM8731_SLAV 1

extern void wm8731_init(int master_mode, int sr, int bit);
extern void wm8713_active(void);
extern void wm8713_inactive(void);

#endif /* WM8731_H */
