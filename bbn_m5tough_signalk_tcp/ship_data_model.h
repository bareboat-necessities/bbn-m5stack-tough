#ifndef SHIP_DATA_MODEL_H
#define SHIP_DATA_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif
  
typedef enum {
  NA = -1,
  OFF = 0,
  ON = 1
} on_off_e;

typedef enum {
  SEVERITY_NA = -1,
  ALARM = 0,
  ALERT = 1,
  WARN = 2,
  EMERGENCY = 3
} notification_severity_e;

typedef enum {
  MODE_NA = -1,
  HEADING_MAG = 0,
  HEADING_TRUE = 1,
  APP_WIND_MAG = 2,
  APP_WIND_TRUE = 3,
  TRUE_WIND_MAG = 4,
  TRUE_WIND_TRUE = 5,
  GROUND_WIND_MAG = 6,
  GROUND_WIND_TRUE = 7,
  COG_MAG = 8,
  COG_TRUE = 9
} ap_mode_e;

typedef enum {
  STATE_NA = -1,
  STANDBY = 0,
  ENGADED = 1
} ap_state_e;

typedef enum {
  CMD_TYPE_NA = -1,
  FOLLOW_DIR = 0,
  FOLLOW_ROUTE = 1
} ap_cmd_type_e;

typedef uint16_t age_t;

typedef struct on_off_t {
  on_off_e st;
  age_t age;
} on_off_t;

typedef struct _ap_mode_t {
  ap_mode_e mode;
  age_t age;
} ap_mode_t;

typedef struct _ap_cmd_type_t {
  ap_cmd_type_e cmd;
  age_t age;
} ap_cmd_type_t;

typedef struct _ap_state_t {
  ap_state_e st;
  age_t age;
} ap_state_t;

typedef struct _angle_deg_t {
  float deg;
  age_t age;
} angle_deg_t;

typedef struct _kn_t {
  float kn;
  age_t age;
} kn_t;

typedef struct _RPM_t {
  float rpm;
  age_t age;
} RPM_t;

typedef tm env_time_t;

typedef struct _nav_light {
  on_off_t state;
  age_t age;
} nav_light;

typedef struct _angular_velocity_t {
  float deg_sec;
  age_t age;
} _angular_velocity_t;

typedef struct _position_t {
  struct _angle_deg_t lat;
  struct _angle_deg_t lon;
} position_t;

typedef struct _nav_lights_t {
  struct _nav_light anchor;
  struct _nav_light motoring;
  struct _nav_light stern;
  struct _nav_light bow_red_green;
} nav_lights_t;

typedef struct _navigation_t {
  struct _position_t position;
  struct _angle_deg_t course_over_ground_true;
  struct _angle_deg_t course_over_ground_mag;
  struct _angle_deg_t mag_var;
  //struct _angle_deg_t mag_dev;
  struct _angle_deg_t heading_mag;
  struct _angle_deg_t heading_true;
  //struct _angle_deg_t heading_compass;
  struct _angular_velocity_t rate_of_turn;
  struct _kn_t speed_over_ground_kn;
  struct _kn_t speed_through_water_kn;
  struct _nav_lights_t lights;
} navigation_t;

typedef struct _wind_t {
  struct _kn_t apparent_wind_speed_kn;
  struct _angle_deg_t apparent_wind_angle_true;
  struct _angle_deg_t apparent_wind_angle_mag;
  struct _kn_t true_wind_speed_kn;
  struct _angle_deg_t true_wind_angle_true;
  struct _angle_deg_t true_wind_angle_mag;
  struct _kn_t ground_wind_speed_kn;
  struct _angle_deg_t ground_wind_angle_true;
  struct _angle_deg_t ground_wind_angle_mag;
} _wind_t;

typedef struct _depth_t {
  float below_keel;
  float below_transducer;
  float below_surface;
  age_t age;
} depth_t;

typedef struct _deg_C_t {
  float deg_C;
  age_t age;
} deg_C_t;

typedef struct _hPa_t {
  float hPa;
  age_t age;
} hPa_t;

typedef struct _percent_t {
  float pct;
  age_t age;
} percent_t;

typedef struct _lux_t {
  float lux;
  age_t age;
} lux_t;

typedef struct _litters_t {
  float L;
  age_t age;
} litters_t;

typedef struct _water_t {
  struct _deg_C_t temp_deg_C;
} water_t;

typedef struct _air_t {
  struct _deg_C_t temp_deg_C;
  struct _hPa_t pressure;
  struct _percent_t humidity_pct;
  struct _lux_t illumination;
} air_t;

typedef struct _environment_t {
  env_time_t time_gps;
  env_time_t time_net;
  struct _wind_t wind;
  struct _depth_t depth;
  struct _water_t water;
  struct _air_t air_outside;
  struct _air_t air_inside;
} environment_t;

typedef struct _current_amp_t {
  float amp;
  age_t age;
} current_amp_t;

typedef struct _voltage_V_t {
  float volt;
  age_t age;
} voltage_V_t;

typedef struct _power_W_t {
  float watt;
  age_t age;
} power_W_t;

typedef struct _freq_Hz_t {
  float Hz;
  age_t age;
} freq_Hz_t;

typedef struct _ac_load_t {
  struct _current_amp_t current;
  struct _voltage_V_t voltage;
  struct _power_W_t powerW;
  struct _freq_Hz_t freqHz;
} ac_load_t;

typedef struct _dc_load_t {
  struct _current_amp_t current;
  struct _voltage_V_t voltage;
  struct _power_W_t powerW;
} dc_load_t;

typedef struct _ac_t {
  const char *load_labels[3];
  struct _ac_load_t loads[3];
  struct _ac_load_t total_load;
} ac_t;

typedef struct _dc_t {
  const char *load_labels[3];
  struct _dc_load_t loads[3];
  struct _dc_load_t total_load;
} dc_t;

typedef enum {
  INV_STATE_NA = -1,
  INV_OFF = 0,
  INV_LOW_POWER = 1,
  INV_FAULT = 2,
  INV_BULK = 3,
  INV_ABSORPTION = 4,
  INV_INVERTING = 9
} inverter_state_e;

typedef enum {
  CHA_STATE_NA = -1,
  CHA_OFF = 0,
  CHA_LOW_POWER = 1,
  CHA_FAULT = 2,
  CHA_BULK = 3,
  CHA_ABSORPTION = 4,
  CHA_FLOAT = 5,
  CHA_STORAGE = 6,
  CHA_EQUALIZE = 7,
  CHA_PASSTHROUGH = 8,
  CHA_INVERTING = 9,
  CHA_POWER_ASSIST = 10,
  CHA_POWER_SUPPLY = 11,
  CHA_BULK_PROTECTION = 252,
} charger_state_e;

typedef struct _inverter_state_t {
  inverter_state_e st;
  age_t age;
} inverter_state_t;

typedef struct _charger_state_t {
  charger_state_e st;
  age_t age;
} charger_state_t;

typedef struct _inverter_t {
  const char *ac_line_labels[2];
  const char *dc_line_labels[2];
  struct _ac_load_t ac_loads[2];
  struct _dc_load_t dc_loads[2];
  struct _inverter_state_t state;
} inverter_t;

typedef struct _inverters_t {
  const char *inverters_labels[1];
  struct _inverter_t by_idx[1];
  struct _ac_load_t total_ac_load;
  struct _dc_load_t total_dc_load;
} inverters_t;

typedef struct _charger_t {
  const char *dc_out_label;
  struct _dc_load_t dc_out;
  struct _charger_state_t state;
} charger_t;

typedef struct _chargers_t {
  const char *chargers_labels[8];
  struct _charger_t by_idx[8];
  struct _dc_load_t total_dc_out;
} chargers_t;

typedef struct _battery_t {  // bank
  struct _voltage_V_t voltage;
  struct _percent_t remaining_capacity_pct;
  struct _percent_t state_of_charge_pct;
} battery_t;

typedef struct _batteries_t {
  const char *batteries_labels[8];
  struct _battery_t by_idx[8];
} batteries_t;

typedef struct _electrical_t {
  struct _ac_t ac;
  struct _dc_t dc;
  struct _inverters_t inverters;
  struct _batteries_t batteries;
  struct _chargers_t chargers;
} electrical_t;

typedef struct _autopilot_t {
  struct _ap_state_t ap_state;
  struct _angle_deg_t command;
  struct _ap_cmd_type_t command_type;
  struct _ap_mode_t ap_mode;
} autopilot_t;

typedef struct _steering_t {
  struct _angle_deg_t rudder_angle;
  struct _autopilot_t autopilot;
} steering_t;

typedef struct _tank_t {
  struct _litters_t volume;
  struct _percent_t percent_of_full;
} tank_t;

typedef struct _tanks_t {
  const char *freshwater_label[2];
  const char *blackwater_label[2];
  const char *fuel_label[3];
  const char *gas_label[2];
  const char *live_well_label[1];
  const char *bait_well_label[1];
  struct _tank_t freshwater[2];
  struct _tank_t blackwater[2];
  struct _tank_t fuel[3];
  struct _tank_t gas[2];
  struct _tank_t live_well[1];
  struct _tank_t bait_well[1];
} tanks_t;

typedef struct _engine_t {
  struct _deg_C_t temp_deg_C;
  struct _RPM_t revolutions_RPM;
  struct _deg_C_t oil_temp;
  struct _hPa_t oil_pressure;
  struct _deg_C_t coolant_temp;
  struct _hPa_t coolant_pressure;
  struct _voltage_V_t alternator_voltage;
} engine_t;

typedef struct _propulsion_t {
  const char *labels[8];
  struct _engine_t engines[8];
} propulsion_t;

typedef struct _notification_t {
  notification_severity_e severity;
  const char *message;
  age_t age;
} _notification_t;

typedef struct _notifications_t {
  struct _notification_t last_notification;
} _notifications_t;

typedef struct _ship_data_t {
  struct _navigation_t navigation;
  struct _environment_t environment;
  struct _electrical_t electrical;
  struct _propulsion_t propulsion;
  struct _steering_t steering;
  struct _tanks_t tanks;
  struct _notifications_t notification;
} ship_data_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
