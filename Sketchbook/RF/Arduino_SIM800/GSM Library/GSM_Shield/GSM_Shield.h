/*
 GSM.h - library for the GSM Playground - GSM Shield for Arduino
 Released under the Creative Commons Attribution-Share Alike 3.0 License
 http://www.creativecommons.org/licenses/by-sa/3.0/
 www.hwkitchen.com
*/
#ifndef __GSM_Shield
#define __GSM_Shield

#include "WProgram.h"
#include "NewSoftSerial.h"

#define GSM_LIB_VERSION 101 // library version X.YY (e.g. 1.00)

// if defined - debug print is enabled with possibility to print out 
// debug texts to the terminal program
//#define DEBUG_PRINT

// if defined - debug print is enabled with possibility to print out 
// the data recived from gsm module
//#define DEBUG_GSMRX

// if defined - debug LED is enabled, otherwise debug LED is disabled
//#define DEBUG_LED_ENABLED

// if defined - SMSs are not send(are finished by the character 0x1b
// which causes that SMS are not send)
// by this way it is possible to develop program without paying for the SMSs 
//#define DEBUG_SMS_ENABLED


// pins definition
#define GSM_ON              8 // connect GSM Module turn ON to pin 77 
#define GSM_RESET           9 // connect GSM Module RESET to pin 35
//#define DTMF_OUTPUT_ENABLE  71 // connect DTMF Output Enable not used
#define DTMF_DATA_VALID     14 // connect DTMF Data Valid to pin 14
#define DTMF_DATA0          72 // connect DTMF Data0 to pin 72
#define DTMF_DATA1          73 // connect DTMF Data1 to pin 73
#define DTMF_DATA2          74 // connect DTMF Data2 to pin 74
#define DTMF_DATA3          75 // connect DTMF Data3 to pin 75

// length for the internal communication buffer
#define COMM_BUF_LEN        200

// some constants for the IsRxFinished() method
#define RX_NOT_STARTED      0
#define RX_ALREADY_STARTED  1

// some constants for the InitParam() method
#define PARAM_SET_0   0
#define PARAM_SET_1   1

// DTMF signal is NOT valid
//#define DTMF_NOT_VALID      0x10


// status bits definition
#define STATUS_NONE                 0
#define STATUS_INITIALIZED          1
#define STATUS_REGISTERED           2
#define STATUS_USER_BUTTON_ENABLE   4




// SMS type 
// use by method IsSMSPresent()
enum sms_type_enum
{
  SMS_UNREAD,
  SMS_READ,
  SMS_ALL,

  SMS_LAST_ITEM
};

enum comm_line_status_enum 
{
  // CLS like CommunicationLineStatus
  CLS_FREE,   // line is free - not used by the communication and can be used
  CLS_ATCMD,  // line is used by AT commands, includes also time for response
  CLS_DATA,   // for the future - line is used in the CSD or GPRS communication  
  CLS_LAST_ITEM
};

enum rx_state_enum 
{
  RX_NOT_FINISHED = 0,      // not finished yet
  RX_FINISHED,              // finished, some character was received
  RX_FINISHED_STR_RECV,     // finished and expected string received
  RX_FINISHED_STR_NOT_RECV, // finished, but expected string not received
  RX_TMOUT_ERR,             // finished, no character received 
                            // initial communication tmout occurred
  RX_LAST_ITEM
};


enum at_resp_enum 
{
  AT_RESP_ERR_NO_RESP = -1,   // nothing received
  AT_RESP_ERR_DIF_RESP = 0,   // response_string is different from the response
  AT_RESP_OK = 1,             // response_string was included in the response

  AT_RESP_LAST_ITEM
};

enum registration_ret_val_enum 
{
  REG_NOT_REGISTERED = 0,
  REG_REGISTERED,
  REG_NO_RESPONSE,
  REG_COMM_LINE_BUSY,
    
  REG_LAST_ITEM
};

enum call_ret_val_enum
{
  CALL_NONE = 0,
  CALL_INCOM_VOICE,
  CALL_ACTIVE_VOICE,
  CALL_INCOM_VOICE_AUTH,
  CALL_INCOM_VOICE_NOT_AUTH,
  CALL_INCOM_DATA_AUTH,
  CALL_INCOM_DATA_NOT_AUTH,
  CALL_ACTIVE_DATA,
  CALL_OTHERS,
  CALL_NO_RESPONSE,
  CALL_COMM_LINE_BUSY,

  CALL_LAST_ITEM
};


enum getsms_ret_val_enum
{
  GETSMS_NO_SMS   = 0,
  GETSMS_UNREAD_SMS,
  GETSMS_READ_SMS,
  GETSMS_OTHER_SMS,

  GETSMS_NOT_AUTH_SMS,
  GETSMS_AUTH_SMS,

  GETSMS_LAST_ITEM
};


class GSM
{
  public:
    byte comm_buf[COMM_BUF_LEN+1];  // communication buffer +1 for 0x00 termination

    // library version
    int LibVer(void);
    // constructor
    GSM(void);
    // serial line initialization
    //void InitSerLine(long baud_rate);
    // set comm. line status
    inline void SetCommLineStatus(byte new_status) {comm_line_status = new_status;};
    // get comm. line status
    inline byte GetCommLineStatus(void) {return comm_line_status;};


    // turns on GSM module
    void TurnOn(long baud_rate);
    // sends some initialization parameters
    void InitParam (byte group);
    // enables DTMF decoder
    //void EnableDTMF(void);
    // gets DTMF value
    //byte GetDTMFSignal(void);
    // turns off/on the speaker
    void SetSpeaker(byte off_on);
    // checks if module is registered in the GSM network
    // must be called regularly
    byte CheckRegistration(void);
    // returns registration state
    byte IsRegistered(void);
    // returns whether complete initialization was made
    byte IsInitialized(void);
    // finds out the status of call
    byte CallStatus(void);
    byte CallStatusWithAuth(char *phone_number,
                            byte first_authorized_pos, byte last_authorized_pos);
    // picks up an incoming call
    void PickUp(void);
    // hangs up an incomming call
    void HangUp(void);
    // calls the specific number
    void Call(char *number_string);
    // makes a call to the number stored at the specified SIM position
    void Call(int sim_position);

    // Speaker volume methods - set, increase, decrease
    char SetSpeakerVolume(byte speaker_volume);
    char IncSpeakerVolume(void);
    char DecSpeakerVolume(void);

    // sends DTMF signal
    char SendDTMFSignal(byte dtmf_tone);

    // User button methods
    inline byte IsUserButtonEnable(void) {return (module_status & STATUS_USER_BUTTON_ENABLE);};
    inline void DisableUserButton(void) {module_status &= ~STATUS_USER_BUTTON_ENABLE;};
    inline void EnableUserButton(void) {module_status |= STATUS_USER_BUTTON_ENABLE;};
    byte IsUserButtonPushed(void);  


    // SMS's methods 
    char SendSMS(char *number_str, char *message_str);
    char SendSMS(byte sim_phonebook_position, char *message_str);
    char IsSMSPresent(byte required_status);
    char GetSMS(byte position, char *phone_number, char *SMS_text, byte max_SMS_len);
    char GetAuthorizedSMS(byte position, char *phone_number, char *SMS_text, byte max_SMS_len,
                          byte first_authorized_pos, byte last_authorized_pos);
    char DeleteSMS(byte position);

    // Phonebook's methods
    char GetPhoneNumber(byte position, char *phone_number);
    char WritePhoneNumber(byte position, char *phone_number);
	char DelPhoneNumber(byte position);
    char ComparePhoneNumber(byte position, char *phone_number);


    // routines regarding communication with the GSM module
    void RxInit(uint16_t start_comm_tmout, uint16_t max_interchar_tmout);
    byte IsRxFinished(void);
    byte IsStringReceived(char const *compare_string);
    byte WaitResp(uint16_t start_comm_tmout, uint16_t max_interchar_tmout);
    byte WaitResp(uint16_t start_comm_tmout, uint16_t max_interchar_tmout, 
                  char const *expected_resp_string);
    char SendATCmdWaitResp(char const *AT_cmd_string,
               uint16_t start_comm_tmout, uint16_t max_interchar_tmout,
               char const *response_string,
               byte no_of_attempts);
			   
	// new routine  TDGINO by Boris
	
	//echo
	void Echo(byte state);


    // debug methods
#ifdef DEBUG_LED_ENABLED
    void BlinkDebugLED (byte num_of_blink);
#endif

#ifdef DEBUG_PRINT
    void DebugPrint(const char *string_to_print, byte last_debug_print);
    void DebugPrint(int number_to_print, byte last_debug_print);
#endif

  private:
    byte comm_line_status;

    // global status - bits are used for representation of states
    byte module_status;

    // variables connected with communication buffer
    
    byte *p_comm_buf;               // pointer to the communication buffer
    byte comm_buf_len;              // num. of characters in the buffer
    byte rx_state;                  // internal state of rx state machine    
    uint16_t start_reception_tmout; // max tmout for starting reception
    uint16_t interchar_tmout;       // previous time in msec.
    unsigned long prev_time;        // previous time in msec.

    // last value of speaker volume
    byte last_speaker_volume; 

    char InitSMSMemory(void);
};
#endif
