#include "stdafx.h"
#include "test_libevent.h"

struct timeval lasttime;
int times = 0;

static void timeout_cb(evutil_socket_t fd, short e, void *arg)
{
    struct timeval newtime, difference;
    struct event *timeout = (event *)arg;
    double elapsed;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &lasttime, &difference);
    elapsed = difference.tv_sec +
        (difference.tv_usec / 1.0e6);

    printf("timeout_cb called at %d: %.3f seconds elapsed.\n",
        (int)newtime.tv_sec, elapsed);
    lasttime = newtime;

    if (++times < 5)
    {
        struct timeval tv;
        evutil_timerclear(&tv);
        tv.tv_sec = 2;
        event_add(timeout, &tv);
    }
}

void test_libevent()
{
    struct event timeout;
    struct timeval tv;
    struct event_base *base;

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    (void)WSAStartup(wVersionRequested, &wsaData);

    /* Initalize the event library */
    base = event_base_new();

    /* Initalize one event */
    event_assign((event *)&timeout, base, -1, 0, timeout_cb, (void*) &timeout);

    evutil_timerclear(&tv);
    tv.tv_sec = 2;
    event_add((event *)&timeout, &tv);

    evutil_gettimeofday(&lasttime, NULL);

    event_base_dispatch(base);
}
