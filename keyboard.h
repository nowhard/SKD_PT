#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "pt/pt.h"
#include <ADuC845.h>

//---------------------------
PT_THREAD(Keyboard_Process(struct pt *pt));//процесс обработки клавиатуры
void Beep_Signal(unsigned char time);
PT_THREAD(Beep_Process(struct pt *pt));
//---------------------------
#endif