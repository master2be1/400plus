#ifndef DISPLAY_H_
#define DISPLAY_H_

#define MAIN_DIALOG (*(int*)(0x47F0))

extern void restore_display();
extern void display_refresh();

extern void display_countdown(int seconds);

#endif /* DISPLAY_H_ */
