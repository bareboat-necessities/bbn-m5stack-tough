/**********************************************************************
  AIS.h - Copyright (c) 2016-2020 Kim Bøndergaarg <kim@fam-boendergaard.dk>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  ``Software''), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **********************************************************************/

// See: https://github.com/KimBP/AIS

#ifndef _AIS_H_
#define _AIS_H_

/* Information about individual AIS message types currently supported, see:
 *
 * http://catb.org/gpsd/AIVDM.html#_types_1_2_and_3_position_report_class_a
 * http://catb.org/gpsd/AIVDM.html#_type_18_standard_class_b_cs_position_report'
 */

#include <Arduino.h>

class AIS {

public:
  enum Nmea0183AisMessages {
    AIS_MSG_1_2_3_POS_REPORT_CLASS_A,
    AIS_MSG_4_BASE_STATION_REPORT,
    AIS_MSG_5_STATIC_AND_VOYAGE,
    AIS_MSG_18_CS_POS_REPORT_CLASS_B,
    AIS_MSG_19_CS_POS_REPORT_EXT_CLASS_B,
    AIS_MSG_21_ATON_REPORT,
    AIS_MSG_24_STATIC_DATA_REPORT,

    AIS_MSG_MAX,
  };

public:
  AIS(const char* AISbitstream, unsigned int fillBits = 0);
  static const int msg_max = 60;
  bool getdata(unsigned int begin, unsigned int cnt, uint8_t* data, bool isigned = false);

  // Fixed position parameters
  uint32_t get_mmsi() {
    return get_u32(8, 30);
  };
  uint8_t get_repeat() {
    return get_u8(6, 2);
  };

  enum Nmea0183AisMessages get_type() {
    return msgType;
  };
  unsigned int get_numeric_type() {
    return msgNumeric;
  };

  /* Notice latitude is returned in minutes with 4 digits precision */
  int32_t get_latitude() {
    return get_i32(AIS_PARAM_I32_LAT);
  };

  /* Notice longitude is returned in minutes with 4 digits precision */
  int32_t get_longitude() {
    return get_i32(AIS_PARAM_I32_LONG);
  };

  int8_t get_rot() {
    return get_i8(AIS_PARAM_I8_TURN);
  };
  uint8_t get_navStatus() {
    return get_u8(AIS_PARAM_E_STATUS);
  };
  uint8_t get_timeStamp() {
    return get_u8(AIS_PARAM_U8_SECOND);
  };
  uint8_t get_manIndicator() {
    return get_u8(AIS_PARAM_E_MANEUVER);
  };
  uint8_t get_shiptype() {
    return get_u8(AIS_PARAM_E_SHIPTYPE);
  };
  uint8_t get_to_port() {
    return get_u8(AIS_PARAM_U8_TO_PORT);
  };
  uint8_t get_to_starboard() {
    return get_u8(AIS_PARAM_U8_TO_STARBOARD);
  };
  uint8_t get_epfd() {
    return get_u8(AIS_PARAM_E_EPFD);
  };
  uint8_t get_draught() {
    return get_u8(AIS_PARAM_U8_DRAUGHT);
  };
  uint8_t get_ais_version() {
    return get_u8(AIS_PARAM_U8_AIS_VERSION);
  };
  uint8_t get_month() {
    return get_u8(AIS_PARAM_U8_MONTH);
  };
  uint8_t get_day() {
    return get_u8(AIS_PARAM_U8_DAY);
  };
  uint8_t get_hour() {
    return get_u8(AIS_PARAM_U8_HOUR);
  };
  uint8_t get_minute() {
    return get_u8(AIS_PARAM_U8_MINUTE);
  };
  uint8_t get_partno() {
    return get_u8(AIS_PARAM_U8_PARTNO);
  };
  uint8_t get_atontype() {
    return get_u8(AIS_PARAM_E_ATONTYPE);
  };
  uint8_t get_atonstatus() {
    return get_u8(AIS_PARAM_U8_ATONSTATUS);
  };

  uint16_t get_SOG() {
    return get_u16(AIS_PARAM_U16_SOG);
  };
  uint16_t get_COG() {
    return get_u16(AIS_PARAM_U16_COG);
  };
  uint16_t get_HDG() {
    return get_u16(AIS_PARAM_U16_HEADING);
  };
  uint16_t get_to_bow() {
    return get_u16(AIS_PARAM_U16_TO_BOW);
  };
  uint16_t get_to_stern() {
    return get_u16(AIS_PARAM_U16_TO_STERN);
  };

  uint32_t get_radio() {
    return get_u32(AIS_PARAM_U32_RADIO);
  };
  uint32_t get_mothership_mmsi() {
    return get_u32(AIS_PARAM_U32_MOTHERSHIP_MMSI);
  };
  uint32_t get_imo() {
    return get_u32(AIS_PARAM_U32_IMO);
  };

  bool get_posAccuracy_flag() {
    return get_flag(AIS_PARAM_B_ACCURACY);
  };
  bool get_raim_flag() {
    return get_flag(AIS_PARAM_B_RAIM);
  };
  bool get_dte_flag() {
    return get_flag(AIS_PARAM_B_DTE);
  };
  bool get_cs_flag() {
    return get_flag(AIS_PARAM_B_CS);
  };
  bool get_display_flag() {
    return get_flag(AIS_PARAM_B_DISPLAY);
  };
  bool get_dsc_flag() {
    return get_flag(AIS_PARAM_B_DSC);
  };
  bool get_band_flag() {
    return get_flag(AIS_PARAM_B_BAND);
  };
  bool get_msg22_flag() {
    return get_flag(AIS_PARAM_B_MSG22);
  };
  bool get_assigned_flag() {
    return get_flag(AIS_PARAM_B_ASSIGNED);
  };
  bool get_aton_off_pos_flag() {
    return get_flag(AIS_PARAM_B_ATON_OFF_POS);
  };
  bool get_aton_virtual_flag() {
    return get_flag(AIS_PARAM_B_ATONVIRTUAL);
  };
  bool get_aton_mode_flag() {
    return get_flag(AIS_PARAM_B_ATONMODE);
  };

  const char* get_shipname() {
    return get_string(AIS_PARAM_T_SHIPNAME, shipname);
  };
  const char* get_destination() {
    return get_string(AIS_PARAM_T_DESTINATION, destination);
  };
  const char* get_callsign() {
    return get_string(AIS_PARAM_T_CALLSIGN, callsign);
  };
  const char* get_vendorid() {
    return get_string(AIS_PARAM_T_VENDORID, vendorid);
  };
  const char* get_atonname() {
    return get_string(AIS_PARAM_T_ATONNAME, atonname);
  };

private:
  enum Nmea0183AisParams {
    AIS_PARAM_U8_REPEAT,
    AIS_PARAM_U32_MMSI,
    AIS_PARAM_E_STATUS,
    AIS_PARAM_I8_TURN,
    AIS_PARAM_U16_SOG,
    AIS_PARAM_B_ACCURACY,
    AIS_PARAM_I32_LONG,
    AIS_PARAM_I32_LAT,
    AIS_PARAM_U16_COG,
    AIS_PARAM_U16_HEADING,
    AIS_PARAM_U8_SECOND,
    AIS_PARAM_E_MANEUVER,
    AIS_PARAM_B_RAIM,
    AIS_PARAM_U32_RADIO,
    AIS_PARAM_U8_AIS_VERSION,
    AIS_PARAM_U32_IMO,
    AIS_PARAM_T_CALLSIGN,
    AIS_PARAM_T_SHIPNAME,
    AIS_PARAM_E_SHIPTYPE,
    AIS_PARAM_U16_TO_BOW,
    AIS_PARAM_U16_TO_STERN,
    AIS_PARAM_U8_TO_PORT,
    AIS_PARAM_U8_TO_STARBOARD,
    AIS_PARAM_E_EPFD,
    AIS_PARAM_U8_MONTH,
    AIS_PARAM_U8_DAY,
    AIS_PARAM_U8_HOUR,
    AIS_PARAM_U8_MINUTE,
    AIS_PARAM_U8_DRAUGHT,
    AIS_PARAM_T_DESTINATION,
    AIS_PARAM_B_DTE,
    AIS_PARAM_B_CS,
    AIS_PARAM_B_DISPLAY,
    AIS_PARAM_B_DSC,
    AIS_PARAM_B_BAND,
    AIS_PARAM_B_MSG22,
    AIS_PARAM_B_ASSIGNED,
    AIS_PARAM_U8_PARTNO,
    AIS_PARAM_T_VENDORID,
    AIS_PARAM_U8_MODEL,
    AIS_PARAM_U32_SERIAL,
    AIS_PARAM_U32_MOTHERSHIP_MMSI,
    AIS_PARAM_E_ATONTYPE,
    AIS_PARAM_T_ATONNAME,
    AIS_PARAM_B_ATON_OFF_POS,
    AIS_PARAM_U8_ATONSTATUS,
    AIS_PARAM_B_ATONVIRTUAL,
    AIS_PARAM_B_ATONMODE,
    AIS_PARAM_B_ATONSPARE,
    AIS_PARAM_MAX,
  };

  struct AisParamPosPair {
    enum Nmea0183AisParams param;
    unsigned start;
  };

  struct AisTypeMsgPair {
    enum Nmea0183AisMessages msgEnum;
    uint8_t msgNumeric;
  };

private:
  static const uint16_t AisParamLength[AIS_PARAM_MAX];
  static const struct AisTypeMsgPair AisMsgTypes[];
  static const struct AisParamPosPair* AisMsgParams[AIS_MSG_MAX];

  static const struct AisParamPosPair AisMsgPosReportClassA[];
  static const struct AisParamPosPair AisMsgBaseStationReport[];
  static const struct AisParamPosPair AisMsgStaticAndVoyage[];
  static const struct AisParamPosPair AisMsgCsPosReportClassB[];
  static const struct AisParamPosPair AisMsgCsPosReportExtClassB[];
  static const struct AisParamPosPair AisMsgAidToNavigationReport[];
  static const struct AisParamPosPair AisMsgStaticDataRaport[];

private:
  void decode(unsigned int fillBits);
  int getbit(unsigned int idx);
  uint16_t get_u16(unsigned start, unsigned len);
  uint16_t get_u16(enum Nmea0183AisParams param);

  uint32_t get_u32(unsigned start, unsigned len);
  uint32_t get_u32(enum Nmea0183AisParams param);

  int32_t get_i32(unsigned start, unsigned len);
  int32_t get_i32(enum Nmea0183AisParams param);

  int8_t get_i8(unsigned start, unsigned len);
  int8_t get_i8(enum Nmea0183AisParams param);

  uint8_t get_u8(unsigned start, unsigned len);
  uint8_t get_u8(enum Nmea0183AisParams param);

  void get_string(char* str, unsigned start, unsigned cnt);
  const char* get_string(enum Nmea0183AisParams param, char* str);
  bool get_flag(enum Nmea0183AisParams param);

  enum Nmea0183AisMessages numericToMessage(uint8_t msgNumeric);
  bool getParamStart(enum Nmea0183AisParams param, unsigned& start);

private:
  static const uint8_t bits_pr_char = 6;
  static const uint8_t shipname_strlen = 20;  // a bits
  static const uint8_t atonname_strlen = 20;
  static const uint8_t destination_strlen = 20;  // a bits
  static const uint8_t callsign_strlen = 7;      // a 6 bits
  static const uint8_t vendorid_strlen = 3;      // a 6 bits

  uint8_t msg[msg_max];
  unsigned int msgLen;
  Nmea0183AisMessages msgType;
  unsigned int msgNumeric;
  //TODO: Allocates strings based on msgType
  char shipname[shipname_strlen + 1];
  char atonname[atonname_strlen + 1];
  char destination[destination_strlen + 1];
  char callsign[callsign_strlen + 1];
  char vendorid[vendorid_strlen + 1];
};

#endif  // _AIS_H_
