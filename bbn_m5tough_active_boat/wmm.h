#ifndef WMM_H
#define WMM_H

#include <stdint.h>

#define WMM_EPOCH 2020.0f

typedef struct
{
  float gnm;
  float hnm;
  float dgnm;
  float dhnm;
} wmm_cof_record_t;

/**
 * Initialize the WMM. Needs calling only once.
 */
void wmm_init(void);

/**
 * Get the date in WMM format
 *
 * @param year Year in 2 digit format of 21st centuary, i.e. 20 represents 2020
 * @param month Month, 1 to 12
 * @param date Date of month, 1 to 31
 * @return Date in WMM format
 * @note No checking of illegal dates is done
 */
float wmm_get_date(uint8_t year, uint8_t month, uint8_t date);

/**
 * Get the magnetic variation at a point on the earth's surface
 *
 * @param glat Latitude in degrees and fractional degrees, negative south
 * @param glon Longitude in degrees and fractional degrees, negative west
 * @param time_years The date as returned from wmm_get_date
 * @param dec Pointer to float holding calculated magnetic variation (also known as declination). Negative is west.
 * @note The altitude used is the ellipsoid at the supplied latitude/longitude, not the earth's surface. This will
 *       give very small errors in some parts of the world comapred to sea level.
 */
void E0000(float glat, float glon, float time_years, float *dec);

#endif
