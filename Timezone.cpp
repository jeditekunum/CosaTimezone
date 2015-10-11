/**
 * @file Timezone.cpp
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
#include "Timezone.hh"

// It is very important that TimezoneRules.hh be included with <>. Do NOT use "".
#include <TimezoneRules.hh>


/**
 * Global Timezone.
 */
Timezone tz;


bool
Timezone::zone(const char *zone_name)
{
  for (uint8_t i = 0; i < membersof(timezones); i++)
    {
      if (strcmp_P(zone_name, timezones[i].name) == 0)
        {
          zone(timezones[i]);
          return (true);
        }
    }
  return (false);
}

bool
Timezone::zone(str_P zone_name)
{
  zone_name_t t;
  strcpy_P(t, zone_name);
  return (zone(t));
}

clock_t
Timezone::local(const clock_t utc)
{
  if (is_dst(utc))
    return (utc + ((Timezone::offset_t)pgm_read_word(&m_zone->dst_rule.offset)*60));
  else
    return (utc + ((Timezone::offset_t)pgm_read_word(&m_zone->std_rule.offset)*60));
}

clock_t
Timezone::local(const clock_t utc, const Rule& rule)
{
  return (utc + ((Timezone::offset_t)pgm_read_word(&rule.offset)*60));
}

Timezone::offset_t
Timezone::offset(const clock_t utc)
{
  if (is_dst(utc))
    return ((Timezone::offset_t)pgm_read_word(&m_zone->dst_rule.offset));
  else
    return ((Timezone::offset_t)pgm_read_word(&m_zone->std_rule.offset));
}

str_P
Timezone::abbrev(const clock_t utc)
{
  if (is_dst(utc))
    return ((str_P)m_zone->dst_rule.abbrev);
  else
    return ((str_P)m_zone->std_rule.abbrev);
}

void
Timezone::all(const clock_t utc, time_t *local, bool *was_dst, Timezone::offset_t *was_offset, Timezone::abbrev_t was_abbrev)
{
  const Rule *rule;
  bool dt = is_dst(utc);

  if (was_dst)
    *was_dst = dt;

  if (dt)
    rule = &m_zone->dst_rule;
  else
    rule = &m_zone->std_rule;

  if (local)
    *local = utc + ((Timezone::offset_t)pgm_read_word(&rule->offset)*60);

  if (was_offset)
    *was_offset = (Timezone::offset_t)pgm_read_word(&rule->offset);

  if (was_abbrev)
    strcpy_P((char*)was_abbrev, (str_P)rule->abbrev);
}

clock_t
Timezone::utc(const time_t& local)
{
  clock_t dst_clock_local = rule_to_clock(m_zone->dst_rule, local.year);
  clock_t std_clock_local = rule_to_clock(m_zone->std_rule, local.year);
  clock_t clock_local = clock_t(local);
  bool dst;

  if (std_clock_local > dst_clock_local)  // northern hemisphere
    dst = (clock_local >= dst_clock_local && clock_local < std_clock_local);
  else  // southern hemisphere
    dst = !(clock_local >= std_clock_local && clock_local < dst_clock_local);

  if (dst)
    clock_local -= (Timezone::offset_t)pgm_read_word(&m_zone->dst_rule.offset)*60;
  else
    clock_local -= (Timezone::offset_t)pgm_read_word(&m_zone->std_rule.offset)*60;

  return (clock_local);
}

clock_t
Timezone::utc(const time_t& local, const Rule& rule)
{
  return (clock_t(local) - ((Timezone::offset_t)pgm_read_word(&rule.offset)*60));
}

bool
Timezone::is_dst(const clock_t utc)
{
  clock_t dst_clock_utc = dst_utc(time_t(utc).year);
  clock_t std_clock_utc = std_utc(time_t(utc).year);

  if (std_clock_utc > dst_clock_utc)  // northern hemisphere
    return (utc >= dst_clock_utc && utc < std_clock_utc);
  else  // southern hemisphere
    return !(utc >= std_clock_utc && utc < dst_clock_utc);
}

clock_t
Timezone::dst_utc(const uint8_t year)
{
  return (rule_to_clock(m_zone->dst_rule, year) - (((Timezone::offset_t)pgm_read_word(&m_zone->dst_rule.offset)-60)*60));
}

clock_t
Timezone::std_utc(const uint8_t year)
{
  return (rule_to_clock(m_zone->std_rule, year) - (((Timezone::offset_t)pgm_read_word(&m_zone->std_rule.offset)+60)*60));
}

clock_t
Timezone::rule_to_clock(const Rule& rule, uint8_t year)
{
  // *LOCAL* clock

  time_t t;
  uint8_t w;
  clock_t c;


  t.year = year;
  t.month = pgm_read_byte(&rule.month);
  t.date = 1;
  t.hours = pgm_read_byte(&rule.hour);
  t.minutes = 0;
  t.seconds = 0;
  t.set_day();

  w = pgm_read_byte(&rule.week);
  if (w == LAST) {
    if (++t.month > 12) {
      t.month = 1;
      t.year++;
    }
    w = 1;
  }

  c = clock_t(t);
  c += (7 * (w - 1) + (pgm_read_byte(&rule.dow) - t.day + 7) % 7) * 24*60*60UL;

  if (pgm_read_byte(&rule.week) == LAST)
    c -= 7 * 24*60*60UL;

  return (c);
}

IOStream& operator<<(IOStream& outs, const Timezone::Rule &rule)
{
  outs << PSTR("{\"") << (str_P)rule.abbrev << PSTR("\",")
       << pgm_read_byte(&rule.month) << PSTR(",")
       << pgm_read_byte(&rule.week) << PSTR(",")
       << pgm_read_byte(&rule.dow) << PSTR(",")
       << pgm_read_byte(&rule.hour) << PSTR(",")
       << (Timezone::offset_t)pgm_read_word(&rule.offset) << PSTR("}");
  return (outs);
}

IOStream& operator<<(IOStream& outs, const Timezone::Zone* zone)
{
  outs << PSTR("{\"") << (str_P)zone->name << PSTR("\",")
       << zone->dst_rule
       << zone->std_rule
       << PSTR("}");
  return (outs);
}

localtime_t::localtime_t(const clock_t c)
{
  tz.all(c, &time, &was_dst, &was_offset, was_abbrev);
}

IOStream& operator<<(IOStream& outs, const localtime_t& lt)
{
  outs << lt.time << PSTR(" ") << lt.was_abbrev;
  return (outs);
}
