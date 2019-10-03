/* ************************************************************************
 *
 *   language specific global variables: Danish (ISO 8859-1)
 *
 *   (c) 2018-2019 by Markus Reschke
 *   translation by glenndk@mikrocontroller.net
 *   based on code from Markus Frejek and Karl-Heinz K¸bbeler
 *
 * ************************************************************************ */


/*
 *  Danish
 */

#if defined (UI_DANISH)

  /*
   *  constant strings
   *  - stored in EEPROM
   */


  /* firmware */
  const unsigned char Tester_str[] EEMEM = "Component Tester";


  /* common terms and texts */
  const unsigned char Probing_str[] EEMEM = "Søger...";
  const unsigned char Timeout_str[] EEMEM = "Timeout";
  const unsigned char Failed1_str[] EEMEM = "Ingen komponent";
  const unsigned char Failed2_str[] EEMEM = "fundet!";
  const unsigned char Done_str[] EEMEM = "færdig!";
  const unsigned char Select_str[] EEMEM = "Vælg";
  const unsigned char Selftest_str[] EEMEM = "Test";
  const unsigned char Adjustment_str[] EEMEM = "Justering";
  const unsigned char Save_str[] EEMEM = "Gem";
  const unsigned char Load_str[] EEMEM = "Hent";
  const unsigned char Show_str[] EEMEM = "Vis værdier";
  const unsigned char Remove_str[] EEMEM = "Fjern";
  const unsigned char Create_str[] EEMEM = "Opret";
  const unsigned char ShortCircuit_str[] EEMEM = "Kortsluttet kredsløb!";
  const unsigned char DischargeFailed_str[] EEMEM = "Batteri?";
  const unsigned char Error_str[] EEMEM = "Fejl!";
  const unsigned char Exit_str[] EEMEM = "Afbryd";
  const unsigned char Checksum_str[] EEMEM = "Tjeksum";
  const unsigned char BJT_str[] EEMEM = "BJT";
  const unsigned char Thyristor_str[] EEMEM = "SCR";
  const unsigned char Triac_str[] EEMEM = "Triac";
  const unsigned char PUT_str[] EEMEM = "PUT";
  const unsigned char Bye_str[] EEMEM = "Farvel!";


  /* units */
  const unsigned char Hertz_str[] EEMEM = "Hz";


  /* options */
  #ifndef BAT_NONE
    const unsigned char Battery_str[] EEMEM = "Bat";
    const unsigned char OK_str[] EEMEM = "ok";
    const unsigned char Weak_str[] EEMEM = "svag";
    const unsigned char Low_str[] EEMEM = "lav";
  #endif

  #ifdef BAT_EXT_UNMONITORED
    const unsigned char External_str[] EEMEM = "ext";
  #endif

  #ifdef UI_KEY_HINTS
  const unsigned char Menu_or_Test_str[] EEMEM = "<Menu Test>";
  #endif

  #if defined (SW_PWM_SIMPLE) || defined (SW_PWM_PLUS)
    const unsigned char PWM_str[] EEMEM = "PWM";
  #endif

  #ifdef SW_SQUAREWAVE
    const unsigned char SquareWave_str[] EEMEM = "firkantbølge";
  #endif

  #ifdef HW_ZENER
    const unsigned char Zener_str[] EEMEM = "Zener";
    const unsigned char Min_str[] EEMEM = "Min";
  #endif

  #ifdef HW_FREQ_COUNTER
    const unsigned char FreqCounter_str[] EEMEM = "Frekvenstæller";
  #endif

  #ifdef HW_FREQ_COUNTER_EXT
    const unsigned char CounterChannel_str[] EEMEM = "Ch";
    const unsigned char FreqInput_str[] EEMEM = "BNC";
    const unsigned char LF_Crystal_str[] EEMEM = "LF-krystal";
    const unsigned char HF_Crystal_str[] EEMEM = "HF-krystal";
  #endif

  #ifdef HW_EVENT_COUNTER
    const unsigned char EventCounter_str[] EEMEM = "Event Counter";
    const unsigned char Count_str[] EEMEM = "Count";
    const unsigned char Time_str[] EEMEM = "Time";
    const unsigned char Events_str[] EEMEM = "Events";
    const unsigned char Stop_str[] EEMEM = "Stop";
  #endif

  #ifdef SW_ENCODER
    const unsigned char Encoder_str[] EEMEM = "Drejegiver";
    const unsigned char TurnRight_str[] EEMEM = "Drej højre om!";
  #endif

  #ifdef SW_CONTRAST
    const unsigned char Contrast_str[] EEMEM = "Kontrast";
  #endif

  #if defined (SW_IR_RECEIVER) || defined (HW_IR_RECEIVER)
    const unsigned char IR_Detector_str[] EEMEM = "IR-detektor";
  #endif

  #ifdef SW_IR_TRANSMITTER
    const unsigned char IR_Transmitter_str[] EEMEM = "IR-RC";
    const unsigned char IR_Send_str[] EEMEM = "sender...";
  #endif

  #ifdef SW_OPTO_COUPLER
    const unsigned char OptoCoupler_str[] EEMEM = "Optokopler";
    const unsigned char None_str[] EEMEM = "Ingen";
    const unsigned char CTR_str[] EEMEM = "CTR";
  #endif

  #if defined (SW_OPTO_COUPLER) || defined (SW_DS18B20) || defined (HW_EVENT_COUNTER)
    const unsigned char Start_str[] EEMEM = "Start";
  #endif

  #ifdef SW_UJT
    const unsigned char UJT_str[] EEMEM = "UJT";
  #endif

  #ifdef SW_SERVO
    const unsigned char Servo_str[] EEMEM = "Servo";
    const unsigned char Sweep_str[] EEMEM = "<->";
  #endif

  #ifdef SW_CAP_LEAKAGE
    const unsigned char CapLeak_str[] EEMEM = "Cap Leakage";
    const unsigned char CapCharge_str[] EEMEM = "Charging";
    const unsigned char CapHigh_str[] EEMEM = "Rl";
    const unsigned char CapLow_str[] EEMEM = "Rh";
    const unsigned char CapDischarge_str[] EEMEM = "Discharging";
  #endif

  #ifdef HW_TOUCH
    const unsigned char TouchSetup_str[] EEMEM = "Touch-opsætning";
  #endif

  #ifdef SW_POWER_OFF
    const unsigned char PowerOff_str[] EEMEM = "Off";
  #endif

#endif


/* ************************************************************************
 *   EOF
 * ************************************************************************ */
