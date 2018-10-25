#include <Ticker.h>
Ticker blinker;
//unsigned int secendCount;

void InitTimer()
{
  blinker.attach(1, TimerTicker); //Use <strong>attach_ms</strong> if you need time in ms
}

void TimerTicker()
{
  if (secendCount + 1 != 0)
    secendCount++;
}
