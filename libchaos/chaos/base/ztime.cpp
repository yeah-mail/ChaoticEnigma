#include "ztime.h"

#if COMPILER == MSVC
    #include <windows.h>
#endif

namespace LibChaos {

#if COMPILER == MSVC

ZTime::ZTime(){
    SYSTEMTIME systime;
    GetLocalTime(&systime);
    FILETIME time;
    SystemTimeToFileTime(&systime, &time);

    // Smash FILETIME into _time
    // It works, I promise*
    memcpy(&_time, &time, 8);
}

ZTime::ZTime(const ZTime &other) : _time(other._time){

}

ZTime::~ZTime(){

}

int ZTime::getSecs() const {
    //tm *time;
    return 0;
}

ZString ZTime::getTime() const {
    SYSTEMTIME systime;
    FILETIME time;
    memcpy(&time, &_time, 8);
    FileTimeToSystemTime(&time, &systime);
    return ZString::ItoS(systime.wHour, 10, 2) + ":" + ZString::ItoS(systime.wMinute, 10, 2) + ":" + ZString::ItoS(systime.wSecond, 10, 2);
}

ZString ZTime::getDate() const {
    SYSTEMTIME systime;
    FILETIME time;
    memcpy(&time, &_time, 8);
    FileTimeToSystemTime(&time, &systime);
    return ZString::ItoS(systime.wMonth, 10, 2) + "/" + ZString::ItoS(systime.wDay, 10, 2) + "/" + ZString::ItoS(systime.wYear, 10, 2);
}

#else

ZTime::ZTime(){
    _time = time(NULL);
}

ZTime::ZTime(const ZTime &other) : _time(other._time){

}

ZTime::~ZTime(){

}

int ZTime::getSecs() const {
    tm *time;
    return 0;
}

ZString ZTime::getTime() const {
    struct tm *time;
    time = localtime(&_time);
    char buffer[20];
    sprintf(buffer, "%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
    ZString out(buffer);
    return out;
}

ZString ZTime::getDate() const {
    struct tm *time;
    time = localtime(&_time);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%02d", time->tm_mon + 1, time->tm_mday, time->tm_year - 100);
    ZString out(buffer);
    return out;
}

#endif

}
