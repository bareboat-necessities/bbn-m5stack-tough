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
    APP_WIND = 2,
    APP_WIND_MAG = 3,
    APP_WIND_TRUE = 4,
    TRUE_WIND = 5,
    TRUE_WIND_MAG = 6,
    TRUE_WIND_TRUE = 7,
    GROUND_WIND_MAG = 8,
    GROUND_WIND_TRUE = 9,
    COG_MAG = 10,
    COG_TRUE = 11,
  } ap_mode_e;

  typedef enum {
    STATE_NA = -1,
    STANDBY = 0,
    ENGAGED = 1
  } ap_state_e;

  typedef enum {
    CMD_TYPE_NA = -1,
    FOLLOW_DIR = 0,
    FOLLOW_ROUTE = 1
  } ap_cmd_type_e;

  typedef unsigned long age_t;

  typedef struct on_off_t {
    on_off_e st;
    age_t age = 0U;
  } on_off_t;

  typedef struct _ap_mode_t {
    ap_mode_e mode;
    age_t age = 0U;
  } ap_mode_t;

  typedef struct _ap_cmd_type_t {
    ap_cmd_type_e cmd;
    age_t age = 0U;
  } ap_cmd_type_t;

  typedef struct _ap_state_t {
    ap_state_e st;
    age_t age = 0U;
  } ap_state_t;

  typedef struct _angle_deg_t {
    float deg = 0;
    age_t age = 0U;
  } angle_deg_t;

  typedef struct _hour_float_t {
    float hr;
    age_t age = 0U;
  } hour_float_t;

  typedef struct _kn_t {
    float kn = 0;
    age_t age = 0U;
  } kn_t;

  typedef struct _length_m_t {
    float m = 0;
    age_t age = 0U;
  } length_m_t;

  typedef struct _RPM_t {
    float rpm = 0;
    age_t age = 0U;
  } RPM_t;

  typedef tm time_tm_t;

  typedef struct _env_time_t {
    time_tm_t t;
    age_t age = 0U;
  } env_time_t;

  typedef struct _nav_light {
    on_off_t state;
    age_t age = 0U;
  } nav_light;

  typedef struct _angular_velocity_t {
    float deg_min = 0;
    age_t age = 0U;
  } _angular_velocity_t;

  typedef struct _position_t {
    struct _angle_deg_t lat;
    struct _angle_deg_t lon;
  } position_t;

  typedef struct _attitude_t {
    struct _angle_deg_t heel;
    struct _angle_deg_t pitch;
  } attitude_t;

  typedef struct _nav_lights_t {
    struct _nav_light anchor;
    struct _nav_light motoring;
    struct _nav_light stern;
    struct _nav_light bow_red_green;
  } nav_lights_t;

  typedef struct _next_point_t {
    struct _angle_deg_t bearing_true;
    struct _angle_deg_t bearing_mag;
    struct _length_m_t distance;
    struct _kn_t velocity_made_good;
  } next_point_t;

  typedef struct _course_rhumbline_t {
    struct _length_m_t cross_track_error;
    struct _angle_deg_t bearing_track_true;
    struct _angle_deg_t bearing_track_mag;
    struct _next_point_t next_point;
  } course_rhumbline_t;

  typedef enum {
    NS_NA = -1,  // not available
    NS_MOORED = 0,
    NS_MOTORING = 1,
    NS_SAILING = 2,
    NS_ANCHORED = 3,
  } nav_state_e;

  typedef struct _nav_state_t {
    nav_state_e st = nav_state_e::NS_NA;
    age_t age = 0U;
  } nav_state_t;

  typedef struct _navigation_t {
    struct _nav_state_t state;
    struct _position_t position;
    struct _position_t position_before;
    struct _attitude_t attitude;
    struct _angle_deg_t course_over_ground_true;
    struct _angle_deg_t course_over_ground_mag;
    struct _angle_deg_t mag_var;
    //struct _angle_deg_t mag_dev;
    struct _angle_deg_t heading_mag;
    struct _angle_deg_t heading_true;
    struct _angle_deg_t leeway;
    //struct _angle_deg_t heading_compass;
    struct _angular_velocity_t rate_of_turn;
    struct _kn_t speed_over_ground;
    struct _kn_t speed_over_ground_avg;
    struct _kn_t speed_through_water;
    struct _kn_t drift;
    struct _angle_deg_t set_true;
    struct _nav_lights_t lights;
    struct _course_rhumbline_t course_rhumbline;
  } navigation_t;

  typedef struct _wind_t {
    struct _kn_t apparent_wind_speed;
    struct _angle_deg_t apparent_wind_angle;
    struct _angle_deg_t apparent_wind_dir_true;
    struct _angle_deg_t apparent_wind_dir_mag;
    struct _kn_t true_wind_speed;
    struct _angle_deg_t true_wind_angle;
    struct _angle_deg_t true_wind_dir_true;
    struct _angle_deg_t true_wind_dir_mag;
    struct _kn_t ground_wind_speed;
    struct _angle_deg_t ground_wind_angle;
    struct _angle_deg_t ground_wind_dir_true;
    struct _angle_deg_t ground_wind_dir_mag;
  } _wind_t;

  typedef struct _depth_t {
    struct _length_m_t below_keel;
    struct _length_m_t below_transducer;
    struct _length_m_t below_surface;
  } depth_t;

  typedef struct _deg_C_t {
    float deg_C;
    age_t age = 0U;
  } deg_C_t;

  typedef struct _hPa_t {
    float hPa;
    age_t age = 0U;
  } hPa_t;

  typedef struct _percent_t {
    float pct;
    age_t age = 0U;
  } percent_t;

  typedef struct _lux_t {
    float lux;
    age_t age = 0U;
  } lux_t;

  typedef struct _litters_t {
    float L;
    age_t age = 0U;
  } litters_t;

  typedef struct _water_t {
    struct _deg_C_t temp_deg_C;
    struct _percent_t salinity_pct;
  } water_t;

  typedef struct _air_t {
    struct _deg_C_t temp_deg_C;
    struct _hPa_t pressure;
    struct _percent_t humidity_pct;
    struct _lux_t illuminance;
  } air_t;

  typedef struct _env_inside_t {
    struct _air_t fridge;
    struct _air_t freezer;
    struct _air_t engine_room;
    struct _air_t salon;
    struct _deg_C_t hot_water;
  } env_inside_t;

  typedef struct _environment_t {
    struct _env_time_t time_gps;
    struct _env_time_t time_net;
    struct _wind_t wind;
    struct _depth_t depth;
    struct _depth_t depth_before;
    struct _angle_deg_t depth_gradient;
    struct _water_t water;
    struct _air_t air_outside;
    struct _env_inside_t env_inside;
    struct _hour_float_t sunrise;
    struct _hour_float_t sunset;
    int no_sunset_flag = 0;
    int no_dark_flag = 0;
    struct _hour_float_t daylight_duration;
    struct _hour_float_t nautical_twilight_duration;
    struct _hour_float_t nautical_twilight_start;
    struct _hour_float_t nautical_twilight_end;
  } environment_t;

  typedef struct _current_amp_t {
    float amp;
    age_t age = 0U;
  } current_amp_t;

  typedef struct _amp_hr_t {
    float amp_hr;
    age_t age = 0U;
  } amp_hr_t;

  typedef struct _voltage_V_t {
    float volt;
    age_t age = 0U;
  } voltage_V_t;

  typedef struct _power_W_t {
    float watt;
    age_t age = 0U;
  } power_W_t;

  typedef struct _freq_Hz_t {
    float Hz;
    age_t age = 0U;
  } freq_Hz_t;

  /*

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
    age_t age = 0U;
  } inverter_state_t;

  typedef struct _charger_state_t {
    charger_state_e st;
    age_t age = 0U;
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
*/

  typedef struct _power_n_current_t {
    struct _current_amp_t current;
    struct _power_W_t powerW;
  } power_n_current_t;

  typedef struct _sys_ac_t {
    struct _power_n_current_t active_in[3];
    struct _power_n_current_t consumption[3];
  } sys_ac_t;

  typedef struct _battery_stat_t {
    struct _voltage_V_t voltage;
    struct _current_amp_t current;
    struct _power_W_t powerW;
    struct _percent_t state_of_charge_pct;
  } battery_stat_t;

  typedef struct _pv_stat_t {
    struct _current_amp_t current;
    struct _power_W_t powerW;
  } pv_stat_t;

  typedef struct _sys_dc_t {
    struct _pv_stat_t pv;
    struct _battery_stat_t battery;
  } sys_dc_t;

  typedef struct _electrical_t {
    struct _sys_ac_t sys_ac;
    struct _sys_dc_t sys_dc;
    //    struct _ac_t ac;
    //    struct _dc_t dc;
    //    struct _inverters_t inverters;
    //    struct _batteries_t batteries;
    //    struct _chargers_t chargers;
  } electrical_t;

  typedef struct _ap_servo_t {
    struct _voltage_V_t voltage;
    struct _deg_C_t temp;
    struct _amp_hr_t amp_hr;
  } ap_servo_t;

  typedef struct _autopilot_t {
    struct _ap_state_t ap_state;
    struct _angle_deg_t heading;
    struct _angle_deg_t command;
    struct _ap_cmd_type_t command_type;
    struct _ap_mode_t ap_mode;
    struct _ap_servo_t ap_servo;
  } autopilot_t;

  typedef struct _steering_t {
    struct _angle_deg_t rudder_angle;
    struct _autopilot_t autopilot;
  } steering_t;

  typedef enum {
    FLUID_TYPE_NA = -1,  // not available
    FUEL = 0,
    FRESH_WATER = 1,
    WASTE_WATER = 2,
    LIVE_WELL = 3,
    LUBRICATION = 4,
    BLACK_WATER = 5,
    GAS = 64,
  } fluid_type_e;

  typedef struct _tank_t {
    //const char *tank_label;
    fluid_type_e fluid_type = FLUID_TYPE_NA;
    struct _litters_t volume;
    struct _percent_t percent_of_full;
  } tank_t;

#define MAX_TANKS 8

  typedef struct _tanks_t {
    struct _tank_t tank[MAX_TANKS];
  } tanks_t;

#define MAX_ENGINES 8

#define MAX_ENGINE_LBL_LENGTH 32

  typedef struct _engine_t {
    struct _deg_C_t temp_deg_C;
    struct _RPM_t revolutions_RPM;
    struct _deg_C_t oil_temp;
    struct _hPa_t oil_pressure;
    struct _deg_C_t coolant_temp;
    struct _hPa_t coolant_pressure;
    struct _voltage_V_t alternator_voltage;
    char engine_label[MAX_ENGINE_LBL_LENGTH] = {0};
  } engine_t;

  typedef struct _propulsion_t {
    struct _engine_t engines[MAX_ENGINES];
  } propulsion_t;

  typedef struct _notification_t {
    notification_severity_e severity;
    const char *message;
    age_t age = 0U;
  } _notification_t;

  typedef struct _notifications_t {
    struct _notification_t last_notification;
  } _notifications_t;

  typedef struct _design_t {
    struct _length_m_t length;
    struct _length_m_t beam;
    struct _length_m_t draft;
    struct _length_m_t air_height;
  } design_t;

  typedef struct _vessel_t {
    char name[51] = {0};
    char call_sign[12] = {0};
    char mmsi[10] = {0};
  } vessel_t;

  typedef struct _ship_data_t {
    struct _navigation_t navigation;
    struct _environment_t environment;
    struct _electrical_t electrical;
    struct _propulsion_t propulsion;
    struct _steering_t steering;
    struct _tanks_t tanks;
    struct _design_t design;
    struct _vessel_t vessel;
    struct _notifications_t notification;
  } ship_data_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
