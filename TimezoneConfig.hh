/**
 * @file TimezoneConfig.hh
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
 * Default for per-application customization of available Timezones.
 *
 * @section Configuration
 * To customize available Timezones, copy this file into the application
 * directory and customize.
 */

#ifndef COSA_TIMEZONECONFIG_HH
#define COSA_TIMEZONECONFIG_HH

//#warning "Using TimezoneConfig.hh in application directory"

// Timezone names must be plain strings however applications can wrap with PSTR().

// Program space can be saved by commenting out unused timezones.

#define TIMEZONE_US_EASTERN             "US/Eastern"
#define TIMEZONE_US_CENTRAL             "US/Central"
#define TIMEZONE_US_MOUNTAIN            "US/Mountain"
#define TIMEZONE_US_PACIFIC             "US/Pacific"
//#define TIMEZONE_EUROPE_STOCKHOLM       "Europe/Stockholm"

#endif
