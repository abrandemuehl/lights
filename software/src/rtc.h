#pragma once

void initRTC();

// Wait until the next rtc tick arrives
void rtcWait();

int rtcGetAndClear();

void rtcReset();
