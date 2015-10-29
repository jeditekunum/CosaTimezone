/**
 * @file CosaTimezone.ino
 * @version 0.9
 *
 * @section License
 * Copyright (C) 2014-2015, jeditekunum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#include "Cosa/IOStream.hh"
#include "Cosa/UART.hh"
#include "Cosa/Time.hh"

#include "Timezone.hh"

IOStream ios(&uart);

Timezone utc_tz;
#ifdef TIMEZONE_US_EASTERN
Timezone us_e_tz;
#endif
#ifdef TIMEZONE_US_CENTRAL
Timezone us_c_tz;
#endif
#ifdef TIMEZONE_US_MOUNTAIN
Timezone us_m_tz;
#endif
#ifdef TIMEZONE_US_PACIFIC
Timezone us_p_tz;
#endif
#ifdef TIMEZONE_EUROPE_STOCKHOLM
Timezone eu_sweden_tz;
#endif

void print(time_t &t)
{
  ios << endl;
  ios << t << PSTR(" UTC") << PSTR(" (input)") << endl;

  ios << time_t(utc_tz.local(t)) << PSTR(" ") << utc_tz.abbrev(t) << PSTR(" ") << utc_tz << endl;
#ifdef TIMEZONE_US_EASTERN
  ios << time_t(us_e_tz.local(t)) << PSTR(" ") << us_e_tz.abbrev(t) << PSTR(" ") << us_e_tz << endl;
#endif
#ifdef TIMEZONE_US_CENTRAL
  ios << time_t(us_c_tz.local(t)) << PSTR(" ") << us_c_tz.abbrev(t) << PSTR(" ") << us_c_tz << endl;
#endif
#ifdef TIMEZONE_US_MOUNTAIN
  ios << time_t(us_m_tz.local(t)) << PSTR(" ") << us_m_tz.abbrev(t) << PSTR(" ") << us_m_tz << endl;
#endif
#ifdef TIMEZONE_US_PACIFIC
  ios << time_t(us_p_tz.local(t)) << PSTR(" ") << us_p_tz.abbrev(t) << PSTR(" ") << us_p_tz << endl;
#endif
#ifdef TIMEZONE_EUROPE_STOCKHOLM
  ios << time_t(eu_sweden_tz.local(t)) << PSTR(" ") << eu_sweden_tz.abbrev(t) << PSTR(" ") << eu_sweden_tz << endl;
#endif
}

void setup()
{
  uart.begin(9600);
  ios << PSTR("CosaTimezone: started") << endl;

#ifdef TIMEZONE_US_CENTRAL
  tz.zone(PSTR(TIMEZONE_US_CENTRAL));
  ios << PSTR("Global timezone set to ") << PSTR(TIMEZONE_US_CENTRAL) << endl;
#endif

#ifdef TIMEZONE_US_EASTERN
  us_e_tz.zone(PSTR(TIMEZONE_US_EASTERN));
#endif
#ifdef TIMEZONE_US_CENTRAL
  us_c_tz.zone(PSTR(TIMEZONE_US_CENTRAL));
#endif
#ifdef TIMEZONE_US_MOUNTAIN
  us_m_tz.zone(PSTR(TIMEZONE_US_MOUNTAIN));
#endif
#ifdef TIMEZONE_US_PACIFIC
  us_p_tz.zone(PSTR(TIMEZONE_US_PACIFIC));
#endif
#ifdef TIMEZONE_EUROPE_STOCKHOLM
  eu_sweden_tz.zone(PSTR(TIMEZONE_EUROPE_STOCKHOLM));
#endif

  time_t t;

  t.year = 14;
  t.date = 1;
  t.hours = 12;
  t.minutes = 15;
  t.seconds = 30;

  // 2014-02-01-12:15:30 UTC
  // 2014-02-01 07:15:30 EST
  // 2014-02-01 06:15:30 CST
  // 2014-02-01 05:15:30 MST
  // 2014-02-01 04:15:30 PST
  // 2014-02-01 13:15:30 CET

  t.month = 2;
  t.set_day();

  print(t);

  // 2014-06-01-12:15:30 UTC
  // 2014-06-01 08:15:30 EDT
  // 2014-06-01 07:15:30 CDT
  // 2014-06-01 06:15:30 MDT
  // 2014-06-01 05:15:30 PDT
  // 2014-06-01 14:15:30 CEST

  t.month = 6;
  t.set_day();

  print(t);

  // 2014-11-01-12:15:30 UTC
  // 2014-11-01 08:15:30 EDT
  // 2014-11-01 07:15:30 CDT
  // 2014-11-01 06:15:30 MDT
  // 2014-11-01 05:15:30 PDT
  // 2014-11-01 13:15:30 CET

  t.month = 11;
  t.set_day();

  print(t);

  // 2014-12-01-12:15:30 UTC
  // 2014-12-01 07:15:30 EST
  // 2014-12-01 06:15:30 CST
  // 2014-12-01 05:15:30 MST
  // 2014-12-01 04:15:30 PST
  // 2014-12-01 13:15:30 CET

  t.month = 12;
  t.set_day();

  print(t);

#ifdef TIMEZONE_US_CENTRAL
  ios << endl;

  // 2014-06-15 08:09:10 UTC

  t.year = 14;
  t.month = 6;
  t.date = 15;
  t.hours = 8;
  t.minutes = 9;
  t.seconds = 10;
  t.set_day();

  localtime_t localtime(t);
  ios << PSTR("2014-06-15 08:09:10 UTC is localtime ") << localtime
      << endl << PSTR("  (should be 2014-06-16 03:09:10 CDT)") << endl;

  // Convert 2014-9-21 17:18:19 CST to UTC
  t.year = 14;
  t.month = 9;
  t.date = 21;
  t.hours = 17;
  t.minutes = 18;
  t.seconds = 19;

  ios << PSTR("2014-09-21 17:18:19 US/Central is ");
  ios << time_t(us_c_tz.utc(t)) << PSTR(" UTC")
      << endl << PSTR("  (should be 2014-09-21 22:18:19 UTC)") << endl;
#endif

  ios << endl << PSTR("Done.") << endl;
}

void loop()
{
  sleep(1);
}
