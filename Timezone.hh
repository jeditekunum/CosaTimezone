/**
 * @file Timezone.hh
 * @version 0.7
 *
 * @section License
 * Copyright (C) 2014, jediunix
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

#ifndef COSA_TIMEZONE_HH
#define COSA_TIMEZONE_HH

#include "Cosa/IOStream.hh"
#include "Cosa/Time.hh"

// Timezone names must be plain strings however applications can wrap with PSTR().

#define TIMEZONE_UTC                    "UTC"

// Program space can be saved by commenting out unused timezones.

#define TIMEZONE_US_EASTERN             "US/Eastern"
#define TIMEZONE_US_CENTRAL             "US/Central"
#define TIMEZONE_US_MOUNTAIN            "US/Mountain"
#define TIMEZONE_US_PACIFIC             "US/Pacific"
//#define TIMEZONE_SWEDEN                "Europe/Stockholm"


// Month numbers (1..12)
enum {
  JANUARY = 1,
  FEBRUARY = 2,
  MARCH = 3,
  APRIL = 4,
  MAY = 5,
  JUNE = 6,
  JULY = 7,
  AUGUST = 8,
  SEPTEMBER = 9,
  OCTOBER = 10,
  NOVEMBER = 11,
  DECEMBER = 12
};

class Timezone {
public:
  /** Week. */
  enum Week {
    LAST,
    FIRST,
    SECOND,
    THIRD,
    FOURTH
  } __attribute__((packed));

  typedef int16_t offset_t;

  static const uint8_t ABBREV_NAME_SIZE = 6;
  typedef char abbrev_t[ABBREV_NAME_SIZE];

  /** Rule. */
  struct Rule {       // PROGMEM
    abbrev_t abbrev;
    uint8_t month;
    Week week;
    uint8_t dow;      // SUNDAY...SATURDAY from Time.hh
    uint8_t hour;     // 0-23 hour when rule takes effect
    offset_t offset;  // from UTC in minutes
  } __attribute__((packed));

  static const uint8_t ZONE_NAME_SIZE = 20;
  typedef char zone_name_t[ZONE_NAME_SIZE];

  /** Zone. */
  struct Zone {       // PROGMEM
    zone_name_t name;
    Rule dst_rule;
    Rule std_rule;
  } __attribute__((packed));

  /**
   * Construct a timezone.
   */
  Timezone()
  {
    zone(PSTR(TIMEZONE_UTC));
  }

  /**
   * Construct a timezone.
   * @param[in] zone zone (PROGMEM).
   */
  Timezone(const Zone& zone) :
    m_zone(&zone)
  {}

  /**
   * Set timezone.
   * @param[in] zone zone to set (PROGMEM).
   */
  void zone(const Zone& zone)
    __attribute__((always_inline))
  {
    m_zone = &zone;
  }

  /**
   * Set timezone.
   * @param[in] zone zone to set.
   * @return true if successful.
   */
  bool zone(const char *zone_name);

  /**
   * Set timezone.
   * @param[in] zone zone to set.
   * @return true if successful.
   */
  bool zone(str_P zone_name);

  /**
   * Get timezone. (PROGMEM)
   */
  const Zone* zone()
    __attribute__((always_inline))
  {
    return (m_zone);
  }

  /**
   * Get timezone name.
   */
  str_P name()
  {
    return ((str_P)m_zone->name);
  }

  /**
   * Get DST rule. (PROGMEM)
   */
  const Rule& dst()
    __attribute__((always_inline))
  {
    return (m_zone->dst_rule);
  }

  /**
   * Get STD rule.  (PROGMEM)
   */
  const Rule& std()
    __attribute__((always_inline))
  {
    return(m_zone->std_rule);
  }

  /**
   * Convert UTC to local time.
   * @param[in] utc utc time.
   * @return localized time.
   */
  clock_t local(const clock_t utc);

  /**
   * Convert UTC to local time as if we were in specified rule.
   * @param[in] utc utc time.
   * @param[in] rule based on rule (PROGMEM).
   * @return localized time.
   */
  clock_t local(const clock_t utc, const Rule& rule);

  /**
   * Get offset in minutes of local zone.
   * @param[in] utc utc time.
   * @return offset in minutes.
   */
  offset_t offset(const clock_t utc);

  /**
   * Get timezone abbreviation of UTC time.
   * @param[in] utc utc time.
   * @return abbrev.
   */
  str_P abbrev(const clock_t utc);

  /**
   * Get all info.
   * @param[in] utc utc time.
   * @param[out] local local time.
   * @param[out] was_dst is dst.
   * @param[out] was_offset offset.
   * @param[out] was_abbrev abbrev.
   */
  void all(const clock_t utc, time_t *local, bool *was_dst, int16_t *was_offset, abbrev_t was_abbrev = NULL);

  /**
   * Get UTC time from local.  Note that this can be ambiguous
   * during transitions between DST and STD.
   * @param[in] local local time.
   * @return utc time.
   */
  clock_t utc(const time_t& local);

  /**
   * Get UTC time as if local time were in specified rule.
   * Note that this can be ambiguous during transitions between
   * DST and STD.  Can be used without a Timezone object.
   * @param[in] local local time.
   * @param[in] rule based on rule (PROGMEM).
   * @return utc time.
   */
  static clock_t utc(const time_t& local, const Rule& rule);

  /**
   * Is UTC time in DST?
   * @param[in] utc utc time.
   * @return true of utc time is DST.
   */
  bool is_dst(const clock_t utc);

  /**
   * Get clock for DST change time.
   * @param[in] year year.
   * @return time of DST start in given year.
   */
  clock_t dst_utc(const uint8_t year);
    
  /**
   * Get clock for STD change time.
   * @param[in] year year.
   * @return time of STD start in given year.
   */
  clock_t std_utc(const uint8_t year);

  /**
   * Print zone name to the given output stream.
   * @param[in] outs output stream.
   * @param[in] timezone to print.
   * @return output stream.
   */
  friend IOStream& operator<<(IOStream& outs, const Timezone& tz)
  {
    outs << (str_P)tz.m_zone->name;
    return (outs);
  }

private:
  const Zone *m_zone;

  /**
   * *LOCAL* Clock for rule in a given year.
   * @param[in] rule rule.
   * @param[in] year year.
   */
  clock_t rule_to_clock(const Rule& rule, uint8_t year);
};

/**
 * Print rule to the given output stream.
 * @param[in] outs output stream.
 * @param[in] rule rule to print (PROGMEM).
 * @return output stream.
 */
IOStream& operator<<(IOStream& outs, const Timezone::Rule& rule);

/**
 * Print zone to the given output stream.
 * @param[in] outs output stream.
 * @param[in] zone zone to print (PROGMEM).
 * @return output stream.
 */
IOStream& operator<<(IOStream& outs, const Timezone::Zone& zone);

/**
 * Global Timezone.
 */
extern Timezone tz;

/*
 * Common date/time structure with local timezone abbreviation.
 * Global Timezone is used when constructing.
 */
struct localtime_t {
  time_t time;
  bool was_dst;
  Timezone::offset_t was_offset;
  Timezone::abbrev_t was_abbrev;

  /**
   * Default constructor.
   */
  localtime_t() {}

  /**
   * Construct localtime from clock and current global timezone.
   * @param[in] c clock.
   */
  localtime_t(const clock_t c);
};

/**
 * Print localtime to the given output stream.
 * @param[in] outs output stream.
 * @param[in] localtime localtime to print.
 * @return output stream.
 */
IOStream& operator<<(IOStream& outs, const localtime_t& localtime);

#endif
