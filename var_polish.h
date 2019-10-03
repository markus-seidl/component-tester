/* ************************************************************************
 *
 *   language specific global variables: Polish (ISO 8859-1)
 *
 *   (c) 2012-2019 by Markus Reschke
 *   translation by Szpila (sszpila@interia.pl)
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *
 * ************************************************************************ */


/*
 *  Polish
 */

#if defined (UI_POLISH)

  /*
   *  constant strings
   *  - stored in EEPROM
   */


  /* firmware */
  const unsigned char Tester_str[] EEMEM = "Component Tester";


  /* common terms and texts */
  const unsigned char Probing_str[] EEMEM = "Testuje...";
  const unsigned char Timeout_str[] EEMEM = "Brak odpowiedzi";
  const unsigned char Failed1_str[] EEMEM = "Nie wykryto";
  const unsigned char Failed2_str[] EEMEM = "elementu!";
  const unsigned char Done_str[] EEMEM = "zrobione!";
  const unsigned char Select_str[] EEMEM = "Wybierz";
  const unsigned char Selftest_str[] EEMEM = "Selftest";
  const unsigned char Adjustment_str[] EEMEM = "Ustawienia";
  const unsigned char Save_str[] EEMEM = "Zapisz";
  const unsigned char Load_str[] EEMEM = "Wczytaj";
  const unsigned char Show_str[] EEMEM = "Pokaz parametry";
  const unsigned char Remove_str[] EEMEM = "Usun";
  const unsigned char Create_str[] EEMEM = "Utworz";
  const unsigned char ShortCircuit_str[] EEMEM = "zwarcie sond!";
  const unsigned char DischargeFailed_str[] EEMEM = "Bateria?";
  const unsigned char Error_str[] EEMEM = "Blad!";
  const unsigned char Exit_str[] EEMEM = "Wyjscie";
  const unsigned char Checksum_str[] EEMEM = "Suma kontrolna";
  const unsigned char BJT_str[] EEMEM = "Tranzystor";
  const unsigned char Thyristor_str[] EEMEM = "Tyrystor";
  const unsigned char Triac_str[] EEMEM = "Triak";
  const unsigned char PUT_str[] EEMEM = "Programowalny UJT";
  const unsigned char Bye_str[] EEMEM = "Czesc!";


  /* units */
  const unsigned char Hertz_str[] EEMEM = "Hz";


  /* options */
  #ifndef BAT_NONE
    const unsigned char Battery_str[] EEMEM = "Bat";
    const unsigned char OK_str[] EEMEM = "ok";
    const unsigned char Weak_str[] EEMEM = "slaba";
    const unsigned char Low_str[] EEMEM = "niska";
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
    const unsigned char SquareWave_str[] EEMEM = "Gen. prostokata";
  #endif

  #ifdef HW_ZENER
    const unsigned char Zener_str[] EEMEM = "Dioda Zenera";
    const unsigned char Min_str[] EEMEM = "Min";
  #endif

  #ifdef HW_FREQ_COUNTER
    const unsigned char FreqCounter_str[] EEMEM = "Licznik czest.";
  #endif

  #ifdef HW_FREQ_COUNTER_EXT
    const unsigned char CounterChannel_str[] EEMEM = "Ch";
    const unsigned char FreqInput_str[] EEMEM = "BNC";
    const unsigned char LF_Crystal_str[] EEMEM = "LF kwarc";
    const unsigned char HF_Crystal_str[] EEMEM = "HF kwarc";
  #endif

  #ifdef HW_EVENT_COUNTER
    const unsigned char EventCounter_str[] EEMEM = "Event Counter";
    const unsigned char Count_str[] EEMEM = "Count";
    const unsigned char Time_str[] EEMEM = "Time";
    const unsigned char Events_str[] EEMEM = "Events";
    const unsigned char Stop_str[] EEMEM = "Stop";
  #endif

  #ifdef SW_ENCODER
    const unsigned char Encoder_str[] EEMEM = "Enkoder";
    const unsigned char TurnRight_str[] EEMEM = "Pokrec w prawo!";
  #endif

  #ifdef SW_CONTRAST
    const unsigned char Contrast_str[] EEMEM = "Kontrast";
  #endif

  #if defined (SW_IR_RECEIVER) || defined (HW_IR_RECEIVER)
    const unsigned char IR_Detector_str[] EEMEM = "Dekoder IR";
  #endif

  #ifdef SW_IR_TRANSMITTER
    const unsigned char IR_Transmitter_str[] EEMEM = "Enkoder IR";
    const unsigned char IR_Send_str[] EEMEM = "wysylam...";
  #endif

  #ifdef SW_OPTO_COUPLER
    const unsigned char OptoCoupler_str[] EEMEM = "Opto-izolator";
    const unsigned char None_str[] EEMEM = "Brak";
    const unsigned char CTR_str[] EEMEM = "CTR";
  #endif

  #if defined (SW_OPTO_COUPLER) || defined (SW_DS18B20) || defined (HW_EVENT_COUNTER)
    const unsigned char Start_str[] EEMEM = "Start";
  #endif

  #ifdef SW_UJT
    const unsigned char UJT_str[] EEMEM = "UJT";
  #endif

  #ifdef SW_SERVO
    const unsigned char Servo_str[] EEMEM = "Serwo";
    const unsigned char Sweep_str[] EEMEM = "<->";
  #endif

  #ifdef SW_CAP_LEAKAGE
    const unsigned char CapLeak_str[] EEMEM = "Uplywnosc";
    const unsigned char CapCharge_str[] EEMEM = "Ladowanie";
    const unsigned char CapHigh_str[] EEMEM = "Rl";
    const unsigned char CapLow_str[] EEMEM = "Rh";
    const unsigned char CapDischarge_str[] EEMEM = "Rozladowyw.";
  #endif

  #ifdef HW_TOUCH
    const unsigned char TouchSetup_str[] EEMEM = "Ust. dotyku";
  #endif

  #ifdef SW_POWER_OFF
    const unsigned char PowerOff_str[] EEMEM = "Wylacz";
  #endif

#endif


/* ************************************************************************
 *   EOF
 * ************************************************************************ */
