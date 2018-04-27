#pragma once

void initRTC(float period);

// Wait until the next rtc tick arrives
void rtcWait();

int rtcGetAndClear();

void rtcReset();
