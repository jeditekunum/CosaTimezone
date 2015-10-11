/**
 * @file TimezoneRules.hh
 * @version 0.9
 *
 * @section License
 * Copyright (C) 2015, jeditekunum
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
 * @section Description
 * Default for per-application customization of Timezones. This file does
 * not actuall select the timezones supported by an application instance,
 * only those available to be selected. TimezoneConfig.hh actually chooses
 * which are in the application. It only makes sense to add more timezones
 * to this file.
 *
 * @section Configuration
 * To customize available Timezones, copy this file into the application
 * directory and customize.
 */

#ifndef COSA_TIMEZONERULES_HH
#define COSA_TIMEZONERULES_HH

//#warning "Using TimezoneRules.hh in application directory"

/*
 * Each entry is the timezone name followed by the rules for standard and daylight time.
 *
 * Each rule is start of period
 *   abbreviation string
 *   month (1-12)
 *   week (LAST/FIRST/SECOND/THIRD/FORTH, 0-4)
 *   day of week (SUNDAY/MONDAY/TUESDAY/WEDNESDAY/THURSDAY/FRIDAY/SATURDAY, 1-7)
 *   hour (0-23)
 *   offset (minutes from UTC)
 */

static const Timezone::Zone timezones[] __PROGMEM =
  {
#ifdef TIMEZONE_US_EASTERN
    {TIMEZONE_US_EASTERN,
     {"EST", NOVEMBER, Timezone::FIRST, SUNDAY, 3, -5*60}, {"EDT", MARCH, Timezone::SECOND, SUNDAY, 2, -4*60}},
#endif
#ifdef TIMEZONE_US_CENTRAL
    {TIMEZONE_US_CENTRAL,
     {"CST", NOVEMBER, Timezone::FIRST, SUNDAY, 3, -6*60}, {"CDT", MARCH, Timezone::SECOND, SUNDAY, 2, -5*60}},
#endif
#ifdef TIMEZONE_US_MOUNTAIN
    {TIMEZONE_US_MOUNTAIN,
     {"MST", NOVEMBER, Timezone::FIRST, SUNDAY, 3, -7*60}, {"MDT", MARCH, Timezone::SECOND, SUNDAY, 2, -6*60}},
#endif
#ifdef TIMEZONE_US_PACIFIC
    {TIMEZONE_US_PACIFIC,
     {"PST", NOVEMBER, Timezone::FIRST, SUNDAY, 3, -8*60}, {"PDT", MARCH, Timezone::SECOND, SUNDAY, 2, -7*60}},
#endif
#ifdef TIMEZONE_EUROPE_STOCKHOLM
    {TIMEZONE_EUROPE_STOCKHOLM,
     {"CET", OCTOBER, Timezone::LAST, SUNDAY, 3, 1*60}, {"CEST", MARCH, Timezone::LAST, SUNDAY, 2, 2*60}},
#endif
    {TIMEZONE_UTC,
     {"UTC", JANUARY, Timezone::FIRST, SUNDAY, 0, 0*60}, {"UTC", DECEMBER, Timezone::LAST, SUNDAY, 0, 0*60}}
  };
#endif
