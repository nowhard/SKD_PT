#include "core.h"

// структура записи из списка обработчиков
typedef struct{
  msg_num msg; // обрабатываемое событие
  handler hnd; // собственно сам обработчик
} iHandler;

// структура события из буфера событий
typedef struct{
  msg_num msg; // номер события
  msg_par par; // параметр
} iMessage;

// структура таймера
typedef struct{
  msg_num msg; // номер генерируемого сообщения
  msg_par par; // его параметр
  unsigned int time; // таймер в условных тиках (сейчас 1 мсек)
  unsigned int const_time;
} iTimer;


//-----------------реализовать постоянный циклический таймер для надежности
/*typedef struct{
  	msg_num msg; // номер генерируемого сообщения
  	msg_par par; // его параметрв условных тиках 
	unsigned int time; // таймер в условных тиках (сейчас 1 мсек)
   	unsigned int const_time;
} iConstTimer; */
//-----------------

 volatile iTimer  		idata  lTimer[maxTimers]={0}; // список таймеров
// volatile iConstTimer   idata  lConstTimer[maxConstTimer];//список постоянных таймеров

 volatile iHandler  	idata lHandler[maxHandlers]={0}; // список обработчиков

 volatile iMessage 		idata lMessage[maxMessages]={0}; // буфер сообщений
 volatile unsigned int  idata lMesPointer=0,hMesPointer=0; // указатели на начало и конец буфера

//------------------------------------------------------------------------------
// установка обработчика события
// вызывается: setHandler(MSG_KEY_PRESS, &checkKey);
void setHandler(msg_num msg_pr, handler hnd_pr) //using 2   ///проверить
{
  unsigned char   idata i=0,j=0;

    msg_num idata msg;
    handler idata hnd;

  msg=msg_pr;
  hnd=hnd_pr;


  while (i<maxHandlers) {
    if (lHandler[i].msg==0) { // ищем свободное место
      lHandler[i].hnd = hnd; // и регистрирем обработчик
      lHandler[i].msg = msg;
      break;
    }
    i++;
  }
  return;
}
//------------------------------------------------------------------------------
// снятие обработчика события
// вызывается: killHandler(MSG_KEY_PRESS, &checkKey);
/*void killHandler(msg_num msg_pr, handler hnd_pr) //using 2  //проверить
{
  unsigned char   xdata  i=0,j=0;
    msg_num idata msg;
    handler idata hnd;

  msg=msg_pr;
  hnd=hnd_pr;


  while (i<maxHandlers) {
    
    if ((lHandler[i].msg==msg) && (lHandler[i].hnd==hnd)) {
      lHandler[i].msg = 0; // если нашли нужный, очищаем
    }
    
    if (lHandler[i].msg != 0) {
      if (i != j) { // сдвигаем все записи к началу списка, чтобы дырок не было
        lHandler[j].msg = lHandler[i].msg;
        lHandler[j].hnd = lHandler[i].hnd;
        lHandler[i].msg = 0;
      }
      j++;
    }
    i++;
  }
  return;
}*/
//------------------------------------------------------------------------------
// занести событие в очередь
// пример вызова: sendMessage(MSG_KEY_PRESS, KEY_MENU)
void sendMessage(msg_num msg_pr, msg_par par_pr) //using 2	//проверить
{
  
    msg_num  idata msg;
    msg_par idata par;

  msg=msg_pr;
  par=par_pr;

  hMesPointer = (hMesPointer+1) & (maxMessages-1); // сдвигаем указатель головы
  
  lMessage[hMesPointer].msg = msg; // заносим событие и параметр
  lMessage[hMesPointer].par = par;
  if (hMesPointer == lMesPointer) 
  { // догнали начало очереди, убиваем необработанное сообытие
    lMesPointer = (lMesPointer+1) & (maxMessages-1);
  }
  return;
}
//------------------------------------------------------------------------------
// обработка событий
void dispatchMessage(void) //using 2
 {	//возможно тормозит, оптимизировать
     volatile char idata  i=0;
  unsigned char   idata res; //результат
  msg_num   idata msg;
  msg_par   idata par;
  
  if (hMesPointer == lMesPointer) 
  { // если пустая очередь - возврат
    return;	
  }
  
  lMesPointer = (lMesPointer+1) & (maxMessages-1); // сдвинем указатель
  
  msg = lMessage[lMesPointer].msg;
  par = lMessage[lMesPointer].par;
  
  if (msg==0)
    return;

  
  for(i=maxHandlers-1; i>=0; i--) 
  { 
  // просматриваем обработчики с конца
    if (lHandler[i].msg==msg) 
	{ 
	// последний занесенный имеет приоритет
      res = lHandler[i].hnd(par); // вызываем обработчик
		 
       if (res)
	   { // если обработчик вернул 1, перываем обработку события
        break;
       }
    }
  }
  return;
}
//------------------------------------------------------------------------------
// установить таймер
// пример вызова: setTimer(MSG_LCD_REFRESH, 0, 50);
//#pragma ot(6)
void setTimer(msg_num msg_pr, msg_par par_pr, unsigned int time_pr) //using 2   //проверить
{
  	unsigned char idata i=0,firstFree=0;

	msg_num idata msg;
	msg_par idata par;
	unsigned  int idata time;
  
  msg=msg_pr;
  par=par_pr;
  time=time_pr;	



  firstFree = maxTimers+1;
 /* if (time == 0) 
  {
    sendMessage(msg, par);
  } 
  else */
  {
  
    for (i=0; i<maxTimers; i++) 
	{ 
	   // ищем установленный таймер
      if (lTimer[i].msg == 0) 
	  {
        firstFree = i;
      } 
	  else 
	  { 
	  // если нашли - обновляем время
        if ((lTimer[i].msg == msg) && (lTimer[i].par == par))
	    {
          lTimer[i].time = time;
		  lTimer[i].const_time = time;
          return;
        }  
      }
    }
    if (firstFree <= maxTimers)
	{ // иначе - просто добавляем новый
      lTimer[firstFree].msg = msg;
      lTimer[firstFree].par = par;
      lTimer[firstFree].time = time;
	  lTimer[firstFree].const_time = time;
    }
  }
  return;
}
//------------------------------------------------------------------------------
// убить таймер
// особенность - убивает все установленные таймеры на данное событие,
// не зависимо от параметра события
void killTimer(msg_num msg_pr) //using 2	//проверить
{
    unsigned char idata i=0;
    msg_num idata msg;
  msg=msg_pr;

  for (i=0; i<maxTimers; i++) {
    if (lTimer[i].msg == msg) {
      lTimer[i].msg = 0;
    }
  }
  return;
} 
//------------------------------------------------------------------------------
// диспетчер таймеров
/*void dispatchTimer() //using 2 
{	 //похоже тормозит
  unsigned char   i=0;
    msg_num idata msg;
    msg_par idata par;
  
  for (i=0; i<maxTimers; i++) 
  {    
		if (lTimer[i].msg == 0)
		{
	      continue;
		}
	    
	    if (lTimer[i].time == 0) 
		{ 
		// если пришло время
	      msg = lTimer[i].msg;
	      par =lTimer[i].par;
	      lTimer[i].msg = 0;
	 
	      sendMessage(msg, par); // создаем событие
		  //LED1=~LED1; 
	    } 
		else 
		{
	      lTimer[i].time--; // иначе просто уменьшаем время
	    }
  }
  return;
}  */
//------------------------------------------------------------------------------
void Timer1_Interrupt(void) interrupt 3  //using 1
{
	
	//dispatchTimer(); 
   //--------------------------------------
    unsigned char   idata i=0;
    msg_num idata msg;
    msg_par idata par;
	EA=0; 
//------------обработка одинарных таймеров----------------------------------- 
  for (i=0; i<maxTimers; i++) 
  {    
		if (lTimer[i].msg == 0)
		{
	      continue;
		}
	    
	    if (lTimer[i].time == 0) 
		{ 
		// если пришло время
	      msg = lTimer[i].msg;
	      par =lTimer[i].par;
	      //lTimer[i].msg = 0;
		  lTimer[i].time=lTimer[i].const_time;
	 
	      //sendMessage(msg, par); // создаем событие
		  //-----------------------------------------------
	      hMesPointer = (hMesPointer+1) & (maxMessages-1); // сдвигаем указатель головы

		  lMessage[hMesPointer].msg = msg; // заносим событие и параметр
		  lMessage[hMesPointer].par = par;
		  if (hMesPointer == lMesPointer) 
		  { // догнали начало очереди, убиваем необработанное сообытие
		    lMesPointer = (lMesPointer+1) & (maxMessages-1);
		  }

		  //-----------------------------------------------
	    } 
	else 
	{
      lTimer[i].time--; // иначе просто уменьшаем время
    }
  }
   //---------------------------------------

	TH1	= TH1_VAL; ///200 Hz;
	TL1 = TL1_VAL;//
 EA=1;
	return;	
}
