/* ************************************************************************
 *
 *   language specific global variables: German (ISO 8859-1)
 *
 *   (c) 2012-2019 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *
 * ************************************************************************ */


/*
 *  German
 */

#if defined (UI_GERMAN)

  /*
   *  constant strings
   *  - stored in EEPROM
   */


  /* firmware */
  const unsigned char Tester_str[] EEMEM = "Bauteiletester";


  /* common terms and texts */
  const unsigned char Probing_str[] EEMEM = "Suche...";
  const unsigned char Timeout_str[] EEMEM = "Timeout";
  const unsigned char Failed1_str[] EEMEM = "Kein Bauteil";
  const unsigned char Failed2_str[] EEMEM = "gefunden!";
  const unsigned char Done_str[] EEMEM = "fertig!";
  const unsigned char Select_str[] EEMEM = "Wähle";
  const unsigned char Selftest_str[] EEMEM = "Selbsttest";
  const unsigned char Adjustment_str[] EEMEM = "Abgleich";
  const unsigned char Save_str[] EEMEM = "Speichern";
  const unsigned char Load_str[] EEMEM = "Laden";
  const unsigned char Show_str[] EEMEM = "Werte zeigen";
  const unsigned char Remove_str[] EEMEM = "Entferne";
  const unsigned char Create_str[] EEMEM = "Baue";
  const unsigned char ShortCircuit_str[] EEMEM = "Kurzschluss!";
  const unsigned char DischargeFailed_str[] EEMEM = "Batterie?";
  const unsigned char Error_str[] EEMEM = "Fehler!";
  const unsigned char Exit_str[] EEMEM = "Abbrechen";
  const unsigned char Checksum_str[] EEMEM = "Prüfsummen-";
  const unsigned char BJT_str[] EEMEM = "Transistor";
  const unsigned char Thyristor_str[] EEMEM = "Thyristor";
  const unsigned char Triac_str[] EEMEM = "Triac";
  const unsigned char PUT_str[] EEMEM = "PUT";
  const unsigned char Bye_str[] EEMEM = "Ciao!";


  /* units */
  const unsigned char Hertz_str[] EEMEM = "Hz";


  /* options */
  #ifndef BAT_NONE
    const unsigned char Battery_str[] EEMEM = "Bat";
    const unsigned char OK_str[] EEMEM = "ok";
    const unsigned char Weak_str[] EEMEM = "schwach";
    const unsigned char Low_str[] EEMEM = "leer";
  #endif

  #ifdef BAT_EXT_UNMONITORED
    const unsigned char External_str[] EEMEM = "ext";
  #endif

  #ifdef UI_KEY_HINTS
  const unsigned char Menu_or_Test_str[] EEMEM = "<Menü Test>";
  #endif

  #if defined (SW_PWM_SIMPLE) || defined (SW_PWM_PLUS)
    const unsigned char PWM_str[] EEMEM = "PWM";
  #endif

  #ifdef SW_SQUAREWAVE
    const unsigned char SquareWave_str[] EEMEM = "Rechteck";
  #endif

  #ifdef HW_ZENER
    const unsigned char Zener_str[] EEMEM = "Zener";
    const unsigned char Min_str[] EEMEM = "Min";
  #endif

  #ifdef HW_FREQ_COUNTER
    const unsigned char FreqCounter_str[] EEMEM = "Freq. Zähler";
  #endif

  #ifdef HW_FREQ_COUNTER_EXT
    const unsigned char CounterChannel_str[] EEMEM = "Ch";
    const unsigned char FreqInput_str[] EEMEM = "BNC";
    const unsigned char LF_Crystal_str[] EEMEM = "LF Quartz";
    const unsigned char HF_Crystal_str[] EEMEM = "HF Quartz";
  #endif

  #ifdef HW_EVENT_COUNTER
    const unsigned char EventCounter_str[] EEMEM = "Ereig. Zähler";
    const unsigned char Count_str[] EEMEM = "Zählen";
    const unsigned char Time_str[] EEMEM = "Zeit";
    const unsigned char Events_str[] EEMEM = "Ereignisse";
    const unsigned char Stop_str[] EEMEM = "Stop";
  #endif

  #ifdef SW_ENCODER
    const unsigned char Encoder_str[] EEMEM = "Dreh-Encoder";
    const unsigned char TurnRight_str[] EEMEM = "Drehe rechts!";
  #endif

  #ifdef SW_CONTRAST
    const unsigned char Contrast_str[] EEMEM = "Kontrast";
  #endif

  #if defined (SW_IR_RECEIVER) || defined (HW_IR_RECEIVER)
    const unsigned char IR_Detector_str[] EEMEM = "IR-Detektor";
  #endif

  #ifdef SW_IR_TRANSMITTER
    const unsigned char IR_Transmitter_str[] EEMEM = "IR RC";
    const unsigned char IR_Send_str[] EEMEM = "sende...";
  #endif

  #ifdef SW_OPTO_COUPLER
    const unsigned char OptoCoupler_str[] EEMEM = "Optokoppler";
    const unsigned char None_str[] EEMEM = "Keiner";
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
    const unsigned char CapLeak_str[] EEMEM = "C Leckstrom";
    const unsigned char CapCharge_str[] EEMEM = "Laden";
    const unsigned char CapHigh_str[] EEMEM = "Rl";
    const unsigned char CapLow_str[] EEMEM = "Rh";
    const unsigned char CapDischarge_str[] EEMEM = "Entladen";
  #endif

  #ifdef HW_TOUCH
    const unsigned char TouchSetup_str[] EEMEM = "Touch Setup";
  #endif

  #ifdef SW_POWER_OFF
    const unsigned char PowerOff_str[] EEMEM = "Off";
  #endif

#endif


/* ************************************************************************
 *   EOF
 * ************************************************************************ */
