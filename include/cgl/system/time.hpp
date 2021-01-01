/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 time.hpp

 2006-09-22 11:29:22

 地球公转一周（回归年）约为365.2422天

 datetime 从0年0月0日到现在的微秒数，没有时区

 计算负数还有问题

 millisecond 毫秒 1/1000秒
 microsecond 微秒 1/1000000秒
 nanosecond  纳秒 1/1000000000秒


*/
#ifndef TIME_HPP_20060922112922
#define TIME_HPP_20060922112922

#include <cgl/public.h>

#if defined(CGL_PLATFORM_WINDOWS)
    #if defined(CGL_PLATFORM_MSVC) || defined(CGL_PLATFORM_CBUILDER)
        #include <windows.h>
        #include <time.h>
    #elif defined(CGL_PLATFORM_GNUC)
        #include <sys/time.h>
    #else
        #error "cgl not implemented for platform"
    #endif
#elif defined(CGL_PLATFORM_APPLE)

    #include <mach/mach_time.h>

#elif defined(CGL_PLATFORM_LINUX) || defined(CGL_PLATFORM_UNIX) || defined(CGL_PLATFORM_ANDROID)
    // Posix means Linux, Unix, and Macintosh OSX, among others (including Linux-based mobile platforms).
    #if defined(CGL_PLATFORM_MINGW)
        #include <pthread_time.h>
    #endif
    #include <time.h>
    #if (defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC))
        #include <errno.h>
    #else
        #include <sys/time.h>
        #include <unistd.h>
    #endif
#endif

namespace cgl{
namespace system{
namespace time{

const int64_t DAY_HOURS           = 24LL;
const int64_t DAY_MINUTES         = 1440LL;
const int64_t DAY_SECONDS         = 86400LL;
const int64_t DAY_MILLISECONDS    = 86400000LL;
const int64_t DAY_MICROSECONDS    = 86400000000LL;

const int64_t HOUR_MINUTES        = 60LL;
const int64_t HOUR_SECONDS        = 3600LL;
const int64_t HOUR_MILLISECONDS   = 3600000LL;
const int64_t HOUR_MICROSECONDS   = 3600000000LL;

const int64_t MINUTE_SECONDS      = 60LL;
const int64_t MINUTE_MILLISECONDS = 60000LL;
const int64_t MINUTE_MICROSECONDS = 60000000LL;

const int64_t SECOND_MILLISECONDS = 1000LL;
const int64_t SECOND_MICROSECONDS = 1000000LL;
const int64_t SECOND_NANOSECONDS  = 1000000000LL;

//---------------------------------------------------------------------------

//nanoseconds
inline uint64_t clock_nanoseconds()
{
#if defined(CGL_PLATFORM_WINDOWS)
    uint64_t t;
    uint64_t frequency;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
    return t * CGL_GIGA / frequency;
#elif defined(CGL_PLATFORM_APPALE)
    return mach_absolute_time();
#elif defined(CGL_PLATFORM_POSIX) // Posix means Linux, Unix, and Macintosh OSX, among others (including Linux-based mobile platforms).
    #if (defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC))
        timespec ts;
        int result = clock_gettime(CLOCK_MONOTONIC, &ts);

        if (result == EINVAL)
            result = clock_gettime(CLOCK_REALTIME, &ts);

        const uint64_t nNanoseconds = uint64_t(ts.tv_nsec) + uint64_t(ts.tv_sec) * CGL_GIGA;
        return nNanoseconds;
    #else
        struct timeval tv;
        gettimeofday(&tv, NULL);
        const uint64_t nMicroseconds = uint64_t(tv.tv_usec) + uint64_t(tv.tv_sec) * CGL_MILLION);
        return nMicroseconds * CGL_MILION;
    #endif
#else
    #error "cgl not implemented for platform"
#endif
}

//milliseconds
inline uint64_t clock_milliseconds()
{
    return clock_nanoseconds() / CGL_MILLION;
}

//microsecond
inline uint64_t clock_microseconds()
{
    return clock_nanoseconds() / CGL_KILO;
}

inline uint64_t clock_nano()
{
    return clock_nanoseconds();
}

inline int clock()
{
    return std::clock();
}
//---------------------------------------------------------------------------
//system::time::now()
#ifdef CGL_PLATFORM_WINDOWS

int64_t now()
{
    //windows的FILETIME以10*1000*1000为一秒单位
    SYSTEMTIME st;
    union{
        FILETIME ft;
        int64_t t;
    }tm;

    GetLocalTime(&st);
    st.wYear += 1601;
    SystemTimeToFileTime(&st, &tm.ft);
    tm.t /= 10000000;
    tm.t *= 1000000;
    tm.t += clock_microseconds() % 1000000;
    return tm.t;
}

#else

int64_t now()
{
    return 0;
}

#endif

//---------------------------------------------------------------------------

//能被4整除，并能被100和400同时整除的年份就是闰年
inline bool is_leapyear(int year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

//某月有几天
inline int month_days(int month)
{
    static int8_t days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return days[month];
}

//获得某年某月的天数
int month_days(int year, int month)
{
    return month==2 ? month_days(month)+is_leapyear(year) : month_days(month);
}

//获得某年的天数
int year_days(int year, int month/*1-12*/, int day/*1-31*/)
{
    int days = 0;
    for(int i=1; i<month; ++i){
        days += month_days(i);
    }
    if(month>2 && is_leapyear(year)) ++days;
    days += day;
    return days;
}

//获得某年某月某日过去的所有天数
#if 0
int sum_days(int year, int month/*1-12*/, int day/*1-31*/)
{
    int days = 0;
    for(int i=0; i<year; ++i)
    {
        days += year_days(i, 12, 31);
    }
    days += year_days(year, month, day);
    return days;
}
#endif

int sum_days(int year, int month/*1-12*/, int day/*1-31*/)
{
    int days = year * 365;
    if(year)days += (year - 1) / 4 + 1;
    days -= (year - 1) / 100 + 1;
    days += (year - 1) / 400 + 1;
    days += year_days(year, month, day);
    return days;
}

/*
x 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 ...
y 0 1 2 3 4 5 6 0 1  2  3  4  5  6  0  1 ...
根据当有以上两个对应的数列，求当x等于n时，y的值是多少
radix 为y的循环基数
*/
template<int radix>
inline int calculate(int64_t n, int x, int y)
{
    int64_t t = -((x - y) - n) % radix;
    if(t < 0)t += radix;
    return static_cast<int>(t);
}

/*
int day_week(int year, int month, int day)
{//通过今天的时间计算
    time_t now = time(NULL);
    tm     t = *localtime(&now);
    int    week;
    //_getsystime(&t);    //获得时间方式2
    int today = get_days(t.tm_year+1900, t.tm_mon+1, t.tm_mday);
    int aday = get_days(year, month, day);
    week = - ((today-t.tm_wday) - aday) % 7;
    if(week<0)week+=7;
    return week;
}//*/

//获得某天是星期几
//week[sun][mon][...]
//0年0月0日星期5
//通过具体的时间获得 2006-1-1 days=732678 wday=0
int day_week(int64_t days)
{
    return calculate<7>(days, 0, 5);
}

int day_week(int year, int month/*1-12*/, int day/*1-31*/)
{
    int days = sum_days(year, month, day);
    return calculate<7>(days, 0, 5);
}

//获得时间秒数
inline int get_second(int hour, int minute, int second)
{
    return hour*360+minute*60+second;
}

//计算天数有几个月几天
void month_day(int days, int &month, int&day, bool leap)
{
    int daydata[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (leap)daydata[1]++;
    month = 0;
    for (int i=0 ;i<12; ++i)
    {
        if (days <= daydata[i]){
            break;
        }
        else{
            ++month;
            days -= daydata[i];
        }
    }
    ++month;//如果days很大，这里day会很大
    day = days;
}

//计算天数有多少年
inline int get_year(int64_t days)
{
    return static_cast<int>((days-1)*10000LL/3652422LL);//100k年无误差
    /*
    c语言库函数，100k年误差少2年
    int i = (unsigned)((days*24*8) / (1461L * 24L));   Number of 4 year blocks
    i = i>>1;// i = (i << 2);
    */
}

int split_date(int64_t days, int& year, int& month, int& day)
{
    year = get_year(days);
    day = static_cast<int>(days - sum_days(year, 0, 0));
    month_day(day, month, day, is_leapyear(year));
    return 0;
}

int split_time(int64_t time, int& hour, int& minute, int& second, int& microsecond)
{
    microsecond = time % SECOND_MICROSECONDS;
    time /= SECOND_MICROSECONDS;
    second = time % 60;
    time /= 60;
    minute = time % 60;
    hour = static_cast<int>(time / 60);
    return 0;
}
//---------------------------------------------------------------------------

class date
{
public:
    int year;
    int month;
    int day;
    int week;

    date() : year(), month(), day(), week() { /* void */ }
    date(int64_t days)
    {
        this->set_date(days);
    }

    void set_date(int y, int m, int d)
    {
        year = y;
        month = m;
        day = d;
        week = day_week(year, month, day);
    }

    void set_date(int64_t days)
    {
        split_date(days, year, month, day);
        week = day_week(days);
    }

    int64_t get_date()const
    {
        return sum_days(year, month, day);
    }
};

class time
{
public:
    int hour;
    int minute;
    int second;
    int microsecond;

    time():hour(), minute(), second(), microsecond(){ /* void */ }
    time(int64_t t)
    {
        split_time(t, hour, minute, second, microsecond);
    }

    void set_time(int h, int m, int s, int ms)
    {
        hour = h;
        minute = m;
        second = s;
        microsecond = ms;
    }

    void set_time(int64_t t)
    {
        split_time(t, hour, minute, second, microsecond);
    }

    int64_t get_time()const
    {
        int64_t t = microsecond;
        t += second * SECOND_MICROSECONDS;
        t = minute * MINUTE_MICROSECONDS;
        t = hour * HOUR_MICROSECONDS;
        return t;
    }
};

class datetime : public date, public time
{
public:
    datetime() : date(), time()
    {

    }
    datetime(int64_t t) : date(t / DAY_MICROSECONDS), time(t % DAY_MICROSECONDS)
    {

    }

    void set(int64_t t)
    {
        set_time(t % DAY_MICROSECONDS);
        set_date(t / DAY_MICROSECONDS);
    }

    int64_t get()const
    {
        int64_t t = get_date();
        t *= DAY_MICROSECONDS;
        t += get_time();
        return t;
    }

    int format(char* buf, size_t size)
    {
        using namespace std;
        return snprintf(buf, size, "%i-%02i-%02i %02i:%02i:%02i.%06i", year, month, day, hour, minute, second, microsecond);
    }
};

//---------------------------------------------------------------------------

}//end namespace time
}//end namespace system
}//end namespace cgl



//---------------------------------------------------------------------------
//
// 时间转换
//

#include <time.h>

namespace cgl{

int64_t time_to_filetime(int64_t t)
{
    //1600-12-30
    const int64_t n = 50522659200000000ULL;//1601
    return (t - n) * 10;
}

int64_t from_filetime(int64_t t)
{
    const int64_t n = 50522659200000000ULL;//1601
    return (t + n) / 10;
}

//dos time
/*
struct DOS_TIME
{
    union{
        uint32_t time;
        struct{
            uint32_t seconds:5; //x2
            uint32_t minute:6;
            uint32_t hour:5;
            uint32_t day:5;
            uint32_t month:4;
            uint32_t year:7;
        };
    };
};
*/

//dostime转换本地时间
void dostime_to_localtime(tm& t, uint32_t dostime)
{
    t.tm_sec  = (dostime & 0x1F) * 2;
    t.tm_min  = (dostime >> 5) & 0x3F;
    t.tm_hour = (dostime >> 11) & 0x1F;
    t.tm_mday = (dostime >> 16) & 0x1F;
    t.tm_mon  = (dostime >> 21) & 0x0F;
    t.tm_year = ((dostime >> 25) & 0x7F) + 1980;
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_isdst = -1;

    //DOS_TIME dt;
    //dt.time = dostime
    //dt.year...
}

//dostime to c time
uint32_t dostime_to_unixtime(uint32_t dostime)
{
    tm t;
    dostime_to_localtime(t, dostime);
    t.tm_year -= 1900;
    t.tm_mon -= 1;

    #ifndef _USE_32BIT_TIME_T    //msvc
    #pragma warning (time_t)
    #endif

    time_t time = mktime(&t);
    return uint32_t(time);
}


}//end namespace cgl

#endif //TIME_HPP_20060922112922

