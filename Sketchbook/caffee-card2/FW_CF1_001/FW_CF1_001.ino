/*                                    
                        *** SIMP_fwCF1 (Sistema Integrato di Monitoraggio delle Prestazioni, Firmware macchine "Caffè") ***
   
      Controllo remoto con SMS dell'inizializzazione, carico e scarico cialde, lock/unlocl macchina
   Sviluppato da New Technology Srls, Guido Cavalera Firmware Engineer, Marco Masciullo Hardware Arranger

   NOTE : 
   - Allo start occorre attendere circa un minuto affinchè la macchina diventi operativa.
   - Le attese o i tempi di controllo possono essere stabiliti con i Flags TEMPORIZZATORI, normalmente sono impostati in maniera ottimale. 
   - Prima della compilazione e del loading sulla macchina occorre impostare il TEL_SETUP, che sarà sempre fisso, impostare a 'false'
     tutte le informazioni di DEBUG altrimenti possibili rallentamenti o errori a 'run time' ed impostare la versione del firmware.
   - Antimanomissione : se la SIM viene rimossa ad ogni lettura stato macchina decrementa il retry di 10 fino a zero mettendola poi come manomessa
   - Al raggiungimento di una scorta minima del 10% il sistema avvisa con SMS il raggiungimento del sottoscorta.
   - Finita la scorta, ovvero se q.tà di carico <= q.tà di scarico, la macchina si blocca MA non avvisa per non consumare ulteriori SMS dato il numero 
     limitato a disposizione.
*/

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <pt.h>
#include <avr/wdt.h>

// IMPORTANTE : in SoftwareSerial.h (C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SoftwareSerial\src)
// Occorre aumentare l'RX buffer size con #define _SS_MAX_RX_BUFF 128 altrimenti problemi nella ricezione di msg lunghi
// che arrivano corrotti !!

#define TEL_SETUP "+8618576608994"  // Telefono di setup, sempre fisso ed usabile solo per l'inizializzazione o emergenza
#define TIPO_MAC "CF1"
#define VERS_FW "001"  // ULTIMA MOD : 27.5.21
#define BT_HOST_NAME "PELLEGRINO"  // IMPORTANTE : deve essere lo stesso nome indicato nell'app. bluetooth di gestione della macchina, 20 chr max

#define DEBUG false   // IMPORTANTE : Prima della release finale portarlo a 'false' altrimenti problemi di avvio del depuratore
#define vOFF 0        // Valori generici di Power ON / OFF
#define vON 1

// Flags TEMPORIZZATORI
#define SIM_START_DELAY 5000               // Tempo di ritardo allo start-up della scheda per permettere alla SIM di avviarsi
#define SECS_CHECK_GET_SMS_FROM_MEM 600000  // Tempo di check ricezione messaggi dalla mem. della sim. Default 10 min.
#define TIMEOUT_RESET_WDT 1440              // Sta ne loop di GET_SMS_FROM_MEM eseguito ogni 10 min, quindi il reset è fatto ogni 10 gg (6 volte ora * 24 ore giorno * 10 gg)

#define SECS_CHECK_EROG 1                // Ogni 'n' secs ispeziona lo stato di erogazione prodotto della macchina. Conviene tenerlo sempre a 1 secondo.  
#define SECS_TIMEOUT_EROG 5              // Dopo 'n' secs di erogazione incrementa il cnt di una unita prodotta/venduta 
#define PERC_SOTTOSC 10                  // Soglia perc. di prodotto (cialde) rimanenti al di sotto del quale scatta il sotto scorta !

// Configurazione PIN's :

const byte pSIM_Tx = 5;  
const byte pSIM_Rx = 4;  
const byte pSIM_Boot = 8;

const byte pRele_1 = 6; // Relè di attivazione erogatore
const byte pEROG_PROD = 3;

// indirizzi di memoria EEPROM su cui scrivere le info dei contatori o altro ...
// Comuni a tutte le macchine ...

const int mCodAtt = 0;            // x 20 + \0 -> Codice di attivazione usato per estrarre il cod_mac (Tel. Sim) di questa macchina
const int mTelServ = 21;          // x 20 + \0 -> Telefono di servizio, usato normalmente nelle trasm. e ricez.
const int mFlagStatoMacch = 42;   // x 2 + \0  -> Vedi var. StatoMacch
const int mATRetry = 45;          // x 1 + \0  -> Valori da 1->9 : tentativi fatti nel rilevare la SIM, dopo 9 va in manomissione !

// Specifici per la macchina del caffè
const int mQtaCarico = 47;        // x 5 + \0
const int mQtaScarico = 53;       // x 5 + \0
const int mFlagSottoscorta = 59;  // x 2 + \0  -> Vedi var. StatoSottoscorta

const int mTimeoutResetWdt_Cnt = 62;  // x 5 + \0  Contatore timeout oltre il quale il Wdt resetta la scheda, Intero 99999
                                      //           aggiornato nel loop, a fine reset lo azzero   
const int mResetWdt_Cnt = 68;         // x 3 + \0  Contatore del nr. di reset scheda causati dal Wdt, Intero 999
const int mStatoSMS = 72;             // x 2 + \0  OK invia SMS di segnalazione, KO non inviare ed attendere intervento tecnico

// Inizializzazione variabili globali di ambiente :
unsigned long CurrTime, SecsCheckGetSMS; // Gestione Multitasking

int TimeoutErogProdotto = -1;    // Timeout di erogazione prodotto. Vedi costante SECS_TIMEOUT_EROG per ulteriori dettagli.
bool Erogato = false;  // semaforo per non far avanzare il cnt di erogazione ad oltranza se il pin è ancora chiuso

char StatoMacch[3];  // +'\0' -> OK, ME = Manomesso erogatore, MS = Manomesso mod. SIM o guasto, BI = Blocco interno a seguito di operaz. fallite o negate, 
                     //          BE = Blocco esterno fatto da un operatore per sicurezza
                     //          ST = Superata soglia TDS, SI = Superata soglia flussim. in ingresso, SU = Superata soglia flussim. in uscita
char StatoSMS[3];    // OK invia SMS di segnalazione, KO non inviare ed attendere intervento tecnico
                     // Questo sistema è stato fatto per bloccare l'invio 'ossessivo' di SMS in presenza di errori per evitare costi inutili di invio SMS 
char StatoSottoscorta[3]; // +'\0' -> OK, SS = Sottoscorta
                               
char COD_ATT[20], TEL_SIM[20], TEL_SERV[20], TEL_CORR[20];  // Tel_Corr : Normalmente viene usato il tel. di servizio assegnato in fase di inizializzazione, 
                                                // altrimenti usa quello di setup o emergenza

static struct pt ptErogatore;
char *SerialCmd = NULL; 

// Gestione SMS
const int Info_Size = 10;       // dimensione massima dell'array ricevuto
char *Info_Ric[Info_Size];      // contiene le varie parti ricevute nel sms come telefono, cmd, valore flow, val TDS, ecc...
bool DisableOverSMS = false;    // Disabilita eventuale invio di SMS verso la centrale. Da usare per bloccare invii inapropriati 

SoftwareSerial SIM800(pSIM_Rx, pSIM_Tx);

void setup()
{
  MCUSR = 0;
  wdt_disable(); 
  
  Serial.begin(19200);
  SIM800.begin(19200); 

  // Pin's non usati e li metto di default
  pinMode(7, OUTPUT); digitalWrite(7, LOW);
  pinMode(9, OUTPUT); digitalWrite(9, LOW);
  pinMode(10, OUTPUT); digitalWrite(10, LOW);
  pinMode(11, OUTPUT); digitalWrite(11, LOW);
  pinMode(12, OUTPUT); digitalWrite(12, LOW);
  pinMode(13, OUTPUT); digitalWrite(13, LOW);

  pinMode(pSIM_Boot, OUTPUT);
  digitalWrite(pSIM_Boot, HIGH);
  
  //pinMode(pEROG_PROD, INPUT);
  //digitalWrite(pEROG_PROD, LOW);

  pinMode(pEROG_PROD, INPUT_PULLUP);
  //digitalWrite(pEROG_PROD, HIGH);

  // initialize Relè pins e valore
  pinMode(pRele_1,OUTPUT);
  digitalWrite(pRele_1,LOW);
  
  // multitasking initialization  
  PT_INIT(&ptErogatore);
  
  SecsCheckGetSMS = 0; // Zero per farlo avviare al primo loop

  for(int i=0; i<Info_Size; i++)
    Info_Ric[i] = nullptr;
 
  //EEPROM_WriteStr(0, "<ERASE_MEM>");  // da usare solo 1 volta come Reset completo con perdita di tutti i dati !
 
  delay(SIM_START_DELAY);  // Faccio partire la SIM per evitare il NORMAL POWER DOWN che mi blocca poi tutti i processi

  // Avvio SIM
  char *SimEC = SIM_ExecuteCmd("AT", 5000, "OK", ""); free(SimEC);
  SimEC = SIM_ExecuteCmd("AT+CSQ", 5000, "OK", ""); free(SimEC);
  SimEC = SIM_ExecuteCmd("AT+CCID", 5000, "OK", ""); free(SimEC);
  // Forzo come 'registered' ma in realtà lo fa in automatico dandomi poi AT+CREG? -> 1,1
  SimEC = SIM_ExecuteCmd("AT+CREG=1", 5000, "OK", ""); free(SimEC);

  // Impostazioni e letture stati 
  SimEC = SIM_ExecuteCmd("AT+CMGF=1",5000, "OK", ""); free(SimEC);         //Set the SMS in text mode. 
  SimEC = SIM_ExecuteCmd("AT+CSCS=\"GSM\"",5000, "OK", ""); free(SimEC);         //Set the GSM mode. 
  //SimEC = SIM_ExecuteCmd("AT+CSCS?",5000, "OK", ""); free(SimEC);         //confirm the settings. MODIFICA
  
  // NOTA : AT+CMEE=1 attiva la visualizzazione del codice di errore. Es. “+CMS ERROR: 122” o “+CME ERROR: 133”
  SimEC = SIM_ExecuteCmd("AT+CNMI=2,1,0,0,0",5000, "OK", ""); free(SimEC);  // Set how the modem will response when a SMS is received

  // Verifica supporto '_BT'  
  //SimEC = SIM_ExecuteCmd("AT+CGMR", 5000, "OK", ""); 
  //Serial.println("AT+CGMR "+String(SimEC));
  //free(SimEC);

  SimEC = SIM_ExecuteCmd("AT+BTPOWER=0", 5000, "OK", ""); free(SimEC);
  delay(500);
  // ... Compongo il nome Host bluetooth della macchina
  char cHostName[30];
  strcpy(cHostName, "AT+BTHOST=");
  strcat(cHostName,BT_HOST_NAME);
  SimEC = SIM_ExecuteCmd(cHostName, 5000, "OK", ""); free(SimEC);   
  SimEC = SIM_ExecuteCmd("AT+BTPOWER=1", 5000, "OK", ""); free(SimEC);
  
  SimEC = SIM_ExecuteCmd("AT&W",5000, "OK", ""); free(SimEC);               // Salvo le conf. sulla SIM. 

  // legge lo stato (memorizzati/tot. disponibili) degli sms nelle varie zone di mem. Con "AT+CPMS=?" si ha l'elenco delle memorie 'storage' in cui vengono salvate
  // (A : IMPORTANTE) : Per selezionare una zona di mem specifica dove operare, ovvero salvare o eliminare gli sms occorre lanciare separatamente il comando con la 
  // singola zona tipo "SM", quindi "AT+CPMS=\"SM\"". In questo caso la mem valida da usare è proprio SM o MT (ME da errore nella ricezione sms) 
  //char *SimEC =  SIM_ExecuteCmd("AT+CPMS=\"SM\",\"ME\",\"MT\"", 5000, "OK", ""); free(SimEC); // Risp : +CPMS: SM 2,20 ME 20,50 MT 2,70 

  // Elimina tutti i sms della memoria storage selezionata. NB : Leggi bene sopra : (A) IMPORTANTE ! Da usare solo per emergenza
  // SimEC = SIM_ExecuteCmd("AT+CMGDA=\"DEL ALL\"",5000,"OK", ""); free(SimEC);

  char *LeggiSM = LeggiStatoMacchina(); free(LeggiSM);  // NB : metterlo sempre alla fine di tutte le procedure di avvio

  wdt_Setup();
  
  if(DEBUG){
    while(!Serial);  // while the serial stream is not open, do nothing !

    Serial.println("Macchina da caffè pronta ! \n");
  }
}
// -----------------------------------------------------------------------------------------------
void loop() 
{
  CurrTime = millis();  // thread multitasking

  // ------------------------ SEZIONE CHECK RICEZIONE LIVE MSG : SMS, COMANDI BLUETOOTH -------------------------

  if(SIM800.available() > 0){
    char SIMLiveMsg[70];  // NB : readBytesUntil ha bisogno di un array NON di un puntatore come 'SerialCmd'
       // IMPORTANTE : mantenere una tolleranza di almeno 20 char in più sulla stringa massima ricevibile !
    // legge byte-byte fino al CR quello che cè sulla seriale senza usare il readString() che ci mette dentro anche 
    // LF/CR e che quindi mi fa saltare i contronti successivi fatti con strcmp(...)
    size_t charsRead = SIM800.readBytesUntil('\n', SIMLiveMsg, sizeof(SIMLiveMsg)-1);  //read entire line
    SIMLiveMsg[charsRead] = '\0';

    if(strcmp(SIMLiveMsg, "") != 0){
      if(StrPos(SIMLiveMsg,"+CMTI",0) > 0){  // "+CMTI" -> tag che identifica un sms appena ricevuto
        int SMS_Id = atoi(SubStr(SIMLiveMsg, StrPos(SIMLiveMsg,",",0)+1,2)); // estraggo l'idSMS da +CMTI: "SM",1
        if(SMS_Id > 0){
          SIM800.flush();
          Get_SMS(SMS_Id, false);
        }
      }else if(StrPos(SIMLiveMsg,"+BTPAIRING",0) > 0){  // richiesta di accoppiamento dal phone
       
        char *SimEC = SIM_ExecuteCmd("AT+BTPAIR=1,1", 5000, "OK", ""); free(SimEC);
        
      }else if(StrPos(SIMLiveMsg,"+BTCONNECTING",0) > 0){  // La connessione dal Phone la ricevo all'invio dei comandi

        char *SimEC = SIM_ExecuteCmd("AT+BTACPT=1", 5000, "OK", ""); free(SimEC);
      
      }else if(StrPos(SIMLiveMsg,"+BTSPPDATA",0) > 0){  // ricezione dati dal phone 
        int FromIdx = StrPos(SIMLiveMsg, "|", 0);
        int ToIdx = StrPos(SIMLiveMsg, "|", FromIdx+1);

        if(FromIdx > 0 && ToIdx > FromIdx){
          char *RespTmp1 = SubStr(SIMLiveMsg, FromIdx+1, (ToIdx-FromIdx)-1); // Intero messaggio senza | ... |
          // inizializza l'array delle parti info del messaggio ricevuto
          for(int i=0; i<Info_Size; i++){
            if(Info_Ric[i]){
              free(Info_Ric[i]);
              Info_Ric[i] = nullptr;
            }
          }
          //Serial.println(" data received "+String(RespTmp1));
          // 0 è il Nr. di tel di setup/servizio, dal 1 in poi le varie parti del messaggio come il cmd, cnt carico, cnt scarico, ecc... 
          // nonchè delle locazioni dell'array Info_Ric[...]
          ExtractToInfoRic(RespTmp1,1);
          free(RespTmp1);
        }
      }
  
      //Serial.println("Phone request : "+String(SIMLiveMsg));
      strcpy(SIMLiveMsg,"");
    }
  }

  // ------------------------ SEZIONE ACQUISIZIONE COMANDI SU SERIAL -------------------------

  if(Serial.available() > 0){
    char SerialRead[100];  // NB : readBytesUntil ha bisogno di un array NON di un puntatore come 'SerialCmd'
    // legge byte-byte fino al CR quello che cè sulla seriale senza usare il readString() che ci mette dentro anche 
    // LF/CR e che quindi mi fa saltare i contronti successivi fatti con strcmp(...)
    size_t charsRead = Serial.readBytesUntil('\n', SerialRead, sizeof(SerialRead)-1);  //read entire line
    SerialRead[charsRead] = '\0';
    // travaso nel SerialCmd la stringa ricevuta depurandola di caratteri 'fastidiosi'
    SerialCmd = malloc_strcpy(SerialRead);
    SerialCmd = FindAndReplace(SerialCmd,"\n","");
    SerialCmd = FindAndReplace(SerialCmd,"\r","");
  }

  Proc_Erogatore(&ptErogatore);

  // ------------------------ SEZIONE CHECK ED ACQUISIZIONE SMS DALLA MEM OGNI 10 min -------------------------
  
  // SecsCheckGetSMS == 0 -> per farlo entrare subito al primo loop
  if(SecsCheckGetSMS == 0 || ((CurrTime - SecsCheckGetSMS) >= SECS_CHECK_GET_SMS_FROM_MEM)){
    
    SecsCheckGetSMS = CurrTime;
   
    Get_SMS(0, true);

    // --- Incrementa il timeout del Wdt, alla fine di TIMEOUT_RESET_WDT ci sarà il reset forzato della scheda ---
    
    // ... leggo il TimeoutResetCnt presente sulla mem...
    char *TimeoutResetCntStr = EEPROM_ReadStr(mTimeoutResetWdt_Cnt);  
    // ... aggiungo e poi memorizzo il cnt aggiornato ! 
    int TimeoutResetCnt = atoi(TimeoutResetCntStr) + 1;
    free(TimeoutResetCntStr);
    
    //Serial.println("SECS_CHECK_GET_SMS_FROM_MEM cnt "+String(TimeoutResetCnt));
    
    // il reset è fatto ogni 10 gg dopo 1440 scatti = (6 volte ora * 24 ore giorno * 10 gg)
    if(TimeoutResetCnt >= TIMEOUT_RESET_WDT){  // raggiunto il timeout, avvio il reset mettendolo in comando seriale
      TimeoutResetCnt = 0;
      Info_Ric[1] = malloc_strcpy("RST");
    }  
   
    char CntBuf[6]; // Nr + '\0' 
    itoa(TimeoutResetCnt,CntBuf,10);
    EEPROM_WriteStr(mTimeoutResetWdt_Cnt, CntBuf);  
  }  
 
  // ----------------------------- SEZIONE COMANDI REMOTI/UTENTE ---------------------------

  if(SerialCmd != NULL){
    Info_Ric[1] = malloc_strcpy(SerialCmd);
    //Serial.println("Info_Ric[1] "+String(Info_Ric[1]));
    free(SerialCmd);
    SerialCmd = NULL;
  }

  // LSM : Lettura Stato Macchina, fatto da SIM (LSM) o con seriale (LSM_S)
  if((strcmp(Info_Ric[1], "LSM") == 0)||(strcmp(Info_Ric[1],"LSM_S") == 0)||(strcmp(Info_Ric[1],"LSM_B") == 0)){
    // IMPORTANTE : Non mettere qua impostazioni sul Tel_Corr : è già impostato alla richiesta. Leggi (B : IMPORTANTE)
    // la risposta deve essere data a chi ne fa richiesta !

    char *Msg = LeggiStatoMacchina();

    // RISPOSTA nelle varie modalità : seriale, bluetooth ed SMS 
    
    if(strcmp(Info_Ric[1],"LSM_S") == 0){
      Serial.println(Msg);
    }else if(strcmp(Info_Ric[1],"LSM_B") == 0){
      // Serial.println("LSM_B  "+String(Msg));

      // richiedo l'invio del messaggio blu
      char *SIMPar = malloc(30);
      sprintf(SIMPar,"AT+BTSPPSEND=%d",strlen(Msg));
      SIM800.println(SIMPar);
      free(SIMPar);

      delay(1000); // attendo il prompt '>' ...
    
      // ... scrivo il Msg ed invio
      char cSMSMsg[strlen(Msg)+1];
      strcpy(cSMSMsg, Msg);
      strcat(cSMSMsg,"\x1A");
      SIM800.println(cSMSMsg);
      
    }else{  // "LSM" richiesto da SMS o dalla normale routine 
      Send_SMS(Msg);
    }
    free(Msg);
    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;

  }else if((strcmp(Info_Ric[1],"INI") == 0)||(StrPos(Info_Ric[1],"INI_S",0) > 0)||(strcmp(Info_Ric[1],"INI_B") == 0)){

    // INIZIALIZZAZIONE : Consente di fare un reset della macchina azzerando o impostando i contatori a valori voluti
    // e di eliminare un eventuale stato di manomissione e di assegnare il numero di telefono di servizio
    // (   Cmd;Cod. attivaz./tel scheda; Tel serv.; qta carico;qta scarico) 
    // Es. INI;10VINIKF25BIN2B;+393468627670;00025;00009
    // Es. INI_S;+393452258474;+393495595524;00025;00009
    // Es. INI_S;+393452258474;+393357094359;00010;00000

    if(DEBUG){
      Serial.println("Inizializzazione macchina in corso ...");
    }

    if(StrPos(Info_Ric[1],"INI_S",0) > 0){
      //char *INIStr = malloc_strcpy(Info_Ric[1]);
      // 7 -> parto da dopo INI_S; poi ad ogni separatore di parte (;) passa a rilevare la successiva
      char *INIStr = SubStr(Info_Ric[1],7,strlen(Info_Ric[1])-6); 
       
      // inizializza l'array delle parti 
      for(int i=0; i<Info_Size; i++){
        if(Info_Ric[i]){
          free(Info_Ric[i]);
          Info_Ric[i] = nullptr;
        }
      }
      
      ExtractToInfoRic(INIStr, 2);   // 1 è il Cmd, dal 2 in poi le varie parti dell'inizializzazione, ecc... 
      free(INIStr);
    }

    char *CodAtt = malloc_strcpy(Info_Ric[2]);
    char *TelServ = malloc_strcpy(Info_Ric[3]);
    char *iQtaCarico = malloc_strcpy(Info_Ric[4]);
    char *iQtaScarico = malloc_strcpy(Info_Ric[5]);

    EEPROM_WriteStr(0, "<ERASE_MEM>");  

    // NB : Se il CodAtt ha un '+' iniziale si tratta del telefono SIM che devo memorizzare sulla PhoneBook (vedi dopo), 
    // altrim. se -1 si tratta di un codice di attivazione da mettere sulla EEPROM
    if(StrPos(CodAtt,"+",0) < 0)
      EEPROM_WriteStr(mCodAtt, CodAtt);  
    
    EEPROM_WriteStr(mTelServ, TelServ);  // NB : dopo è importante la lettura stato macchina per aggiornare il telefono
    strcpy(StatoMacch, "OK");
    EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);
    strcpy(StatoSMS, "OK");
    EEPROM_WriteStr(mStatoSMS, StatoSMS);
    EEPROM_WriteStr(mATRetry, "0");
    
    EEPROM_WriteStr(mQtaCarico, iQtaCarico); 
    EEPROM_WriteStr(mQtaScarico, iQtaScarico);
    
    strcpy(StatoSottoscorta, "OK");
    // ... MA se non siamo carichi la mette in sottoscorta ! 
    if(atoi(iQtaScarico) >= atoi(iQtaCarico)){
      strcpy(StatoSottoscorta, "SS");
    }
    EEPROM_WriteStr(mFlagSottoscorta, StatoSottoscorta); 

    free(TelServ); free(iQtaCarico); free(iQtaScarico); 
    
    // ------------- Eliminazione SMS dalla SIM per liberare mem
    char *SimEC = SIM_ExecuteCmd("AT+CPMS=\"ME\" ", 1000, "OK", ""); free(SimEC); // Risp : +CPMS: SM 2,20 ME 20,50 MT 2,70 
    // Elimina tutti i sms della memoria storage selezionata. NB : Leggi bene sopra : (A) IMPORTANTE !
    SimEC = SIM_ExecuteCmd("AT+CMGDA=\"DEL ALL\"",1000, "OK", ""); free(SimEC);

    SimEC = SIM_ExecuteCmd("AT+CPMS=\"MT\" ", 1000, "OK", "");  free(SimEC); // Risp : +CPMS: SM 2,20 ME 20,50 MT 2,70 
    // Elimina tutti i sms della memoria storage selezionata. NB : Leggi bene sopra : (A) IMPORTANTE !
    SimEC = SIM_ExecuteCmd("AT+CMGDA=\"DEL ALL\"",1000, "OK", ""); free(SimEC);
    
    // IMPORTANTE : la AT+CPMS="SM"  deve stare per ultima in quanto è la zona di mem supportata dalla SIM e quindi deve essere selezionata come mem di def. 
    SimEC = SIM_ExecuteCmd("AT+CPMS=\"SM\" ", 1000, "OK", ""); free(SimEC); // Risp : +CPMS: SM 2,20 ME 20,50 MT 2,70 
    // Elimina tutti i sms della memoria storage selezionata. NB : Leggi bene sopra : (A : IMPORTANTE) !
    SimEC = SIM_ExecuteCmd("AT+CMGDA=\"DEL ALL\"",1000, "OK", ""); free(SimEC);

    // Impostazione del telefono SIM nella phone book passato in fase di inizializzazione
    if(StrPos(CodAtt,"+",0) > 0){
      // Accede alla phone book ...
      SimEC = SIM_ExecuteCmd("AT+CPBS=\"ON\"",5000, "OK", ""); free(SimEC);  
      // ci scrive il telefono SIM
      char *SIMPar = malloc(50);
      sprintf(SIMPar,"AT+CPBW=1,\"%s\",129,\"TEL\"",CodAtt);
      SimEC = SIM_ExecuteCmd(SIMPar,5000, "OK", ""); free(SimEC);  
      free(SIMPar);
    }
    free(CodAtt);

    // Svuoto l'array delle parti 
    for(int i=0; i<Info_Size; i++){
      if(Info_Ric[i]){
        free(Info_Ric[i]);
        Info_Ric[i] = nullptr;
      }
    }

    if(DEBUG){
      Serial.println("Inizializzazione macchina fatta.");
    }
    
    char *LeggiSM = LeggiStatoMacchina(); free(LeggiSM);   // rilegge lo stato macchina


  }else if((strcmp(Info_Ric[1],"CAR") == 0)||(strcmp(Info_Ric[1],"CAR_B") == 0)){  // fatta solo tramite SMS o Bluetooth 

    // CARICO : Aggiorna il cnt di carico aggiungendo le qtà volute
    // (Cmd;Qtà di carico) Es. CAR;00010

    char *iQtaCarico = malloc_strcpy(Info_Ric[2]);
    int QtaCarico = atoi(iQtaCarico);
    // ... leggo la qtà presente sulla mem...
    char *QtaCaricoStr = EEPROM_ReadStr(mQtaCarico);  
    // ... aggiungo e poi memorizzo il cnt aggiornato ! 
    QtaCarico += atoi(QtaCaricoStr);

    EEPROM_WriteStr(mQtaCarico, FormatNumber(QtaCarico,5,0));  
    free(iQtaCarico); free(QtaCaricoStr); 

    char *QtaScaricoStr = EEPROM_ReadStr(mQtaScarico);  
    // ... aggiungo e poi memorizzo il cnt aggiornato ! 
    int QtaScarico = atoi(QtaScaricoStr) + 1;
    free(QtaScaricoStr); 
    
    // Se siamo carichi (ovvero abbiamo a disposizione prodotto da scaricare) rimuove il sottoscorta ed eventuale blocco! 
    if(QtaScarico < QtaCarico){
      strcpy(StatoSottoscorta, "OK");
      EEPROM_WriteStr(mFlagSottoscorta, StatoSottoscorta); 

      // Se la macchina era stata bloccata in autom. perchè in sottoscorta, rimuove il blocco !
      if(strcmp(StatoMacch,"BI") == 0){
        strcpy(StatoMacch, "OK");
        EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);
        digitalWrite(pRele_1, LOW);  
      }
    }
    
    if(DEBUG){
      Serial.println("Aggiornamento q.tà di carico fatta.");
    }

    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;
    free(Info_Ric[2]);
    Info_Ric[2] = nullptr;

  }else if((strcmp(Info_Ric[1],"RST") == 0)||(strcmp(Info_Ric[1],"RST_B") == 0)){    // Reset macchina

    //Serial.println("Reset scheda");

    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;
    // RESET scheda : Il wdt è impostato a 8s di timeout, bloccando il loop per 10s induco al reset 'forzato' ! 
    delay(20000); // sembra che 10s siano pochi !

  }else if((strcmp(Info_Ric[1],"KOM") == 0)||(strcmp(Info_Ric[1],"KOM_B") == 0)){

    // KOM : KO-Macchina ovvero blocco della macchina o posta come Erogatore manomesso !
    // (Cmd) Es. KOM

    //StatoMacch = "BE";
    strcpy(StatoMacch, "BE");
    EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);  

    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;

  }else if((strcmp(Info_Ric[1],"OKM") == 0)||(strcmp(Info_Ric[1],"OKM_B") == 0)){

    // OKM : rimuove lo stato di manomissione (o blocco) della macchina 
  
    strcpy(StatoMacch, "OK");
    EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);  
    EEPROM_WriteStr(mATRetry, "0");
    digitalWrite(pRele_1, LOW);    
    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;

  }else if((strcmp(Info_Ric[1],"OKS") == 0)||(strcmp(Info_Ric[1],"OKS_B") == 0)){  // OK all'invio delle segnalazioni via SMS

    DisableOverSMS = false;
    strcpy(StatoSMS, "OK");
    EEPROM_WriteStr(mStatoSMS, StatoSMS);
    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;

  }else if((strcmp(Info_Ric[1],"KOS") == 0)||(strcmp(Info_Ric[1],"KOS_B") == 0)){  // KO NON inviare segnalazioni SMS ed attendere intervento tecnico

    DisableOverSMS = true; 
    strcpy(StatoSMS, "KO");
    EEPROM_WriteStr(mStatoSMS, StatoSMS);
    free(Info_Ric[1]);
    Info_Ric[1] = nullptr;
  }

  wdt_reset();

  // ATTENZIONE : Non usare delay altrimenti la chiamata ai protothreads come Proc_FlowCtrl risulta influenzata 
  //              in termini di tempi di esecuzione
  // delay(300); // delay canonico per non far girare "all'impazzata" il loop 
}
// ---------------------------------------------------------------------------------------- 
static int Proc_Erogatore(struct pt *pt)
{
  //const int REFR_EROG = 200;
  const int REFR_EROG = 1000;
  static unsigned long LastTimeErog = 0;
  static unsigned long SecsCheckErog = 0;
  
  PT_BEGIN(pt);
  while(1) {
    // per Erogato leggere le note su nella dichiarativa
    if(!Erogato && digitalRead(pEROG_PROD) == LOW){

      if(strcmp(StatoMacch,"OK") == 0){
        if(TimeoutErogProdotto == -1){
          SecsCheckErog = 0;
          TimeoutErogProdotto = SECS_TIMEOUT_EROG;
  
          if(DEBUG){
            Serial.println("Sto erogando il prodotto, tra " + String(SECS_TIMEOUT_EROG) + " sec. scarico la cialda !"); 
          }
        }else{
          if(SecsCheckErog >= (SECS_CHECK_EROG * 1000)){
            //Serial.println(String(SecsCheckErog));
            TimeoutErogProdotto--; 
            SecsCheckErog = 0;
          }
          
          if(DEBUG){
            Serial.println(String(TimeoutErogProdotto)); 
          }
        }
      }else{ // se <> OK la macchina è manomessa

        digitalWrite(pRele_1, HIGH);
        
        // Avvisare con tre beep di alert come 'macchina manomessa' !

        Erogato = true;  // devo impostarla altrim. rifà in continuazione i beep di alert ...
        
        if(DEBUG){
          Serial.println("ATTENZIONE... Impossibile erogare, macchina manomessa !"); 
        }
      }

    }else if(digitalRead(pEROG_PROD) == HIGH){  // stato normale di pullup (non in erogazione)

      //digitalWrite(pRele_1, LOW);
      Erogato = false; // al momento in cui il tasto viene spento l'erogazione è finita e si predispone per l'altra successiva !
      TimeoutErogProdotto = -1; 
      
      if((DEBUG) && (strcmp(StatoMacch,"OK") == 0) && (TimeoutErogProdotto > 0)){  // NB : deve stare prima di assegnare -1
        Serial.println("Erogazione prodotto interrotta o terminata !");
      }
    };

    // se la macchina è OK (non manomessa) e il cnt arriva a Zero (timeout scaduto) il prodotto si considera erogato, 
    // quindi incremento il contatore  
    if((strcmp(StatoMacch,"OK") == 0) && (TimeoutErogProdotto == 0)){
      Erogato = true;
      TimeoutErogProdotto = -1;
      //digitalWrite(pEROG_PROD, LOW);

      // ... leggo la qtà di carico e scarico presente sulla mem...
      int QtaCarico = atoi(EEPROM_ReadStr(mQtaCarico));  

      char *QtaScaricoStr = EEPROM_ReadStr(mQtaScarico);  
      // ... aggiungo e poi memorizzo il cnt aggiornato ! 
      int QtaScarico = atoi(QtaScaricoStr) + 1;
      free(QtaScaricoStr); 
      
      EEPROM_WriteStr(mQtaScarico, FormatNumber(QtaScarico,5,0));  

      if(DEBUG){
        Serial.println("Tot. scar.: " + String(QtaScarico));
      }

      // siamo arrivati sotto scorta e quindi mando un avviso in centrale per il ricarico
      // Calcola la perc. di prodotto rimanente in base al carico / scarico
      int PercRim = 100 - ((QtaScarico * 100)/QtaCarico);
      //Serial.println(" car "+String(QtaCarico)+" scar "+ String(QtaScarico)+" % "+ String(PercRim) );
      if((strcmp(StatoSottoscorta,"OK") == 0) && (PercRim <= PERC_SOTTOSC )){
        
        strcpy(StatoSottoscorta, "SS");
        EEPROM_WriteStr(mFlagSottoscorta, StatoSottoscorta);  

        if(!DisableOverSMS){
          // ----------- INVIO SMS di avviso ------------- 
          // In mancanza del telefono di servizio usa quello di setup
          strcpy(TEL_CORR, TEL_SERV);
          if(strcmp(TEL_CORR, "") == 0){
            strcpy(TEL_CORR, TEL_SETUP);    
          }

          char *Msg = LeggiStatoMacchina();
          Send_SMS(Msg);
          free(Msg);
        }

        if(DEBUG){
          Serial.println("ATTENZIONE... Macchina sottoscorta !\n");
        }
      }

      // Ultime disposizioni : Blocco la macchina al sottoscorta !
      if(QtaScarico >= QtaCarico){
        strcpy(StatoMacch, "BI");
        EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);
      }
    }
    
    LastTimeErog = millis();
    PT_WAIT_UNTIL(pt, (millis()-LastTimeErog) > REFR_EROG);

    SecsCheckErog += REFR_EROG;
  }
  PT_END(pt);
}
// ---------------------------------------------------------------------------------------- 
// Nel ritorno stringa sintetica usa come separatore il ';'
char *LeggiStatoMacchina()
{
  char CntSMS[30];    // stringa della forma : 'SM,ME,MT:2,20,20,50,2,70'
  char SIMSignal[8];  
  char *rsROM;

  // -------------------------- Aggiornamento/inizial. impostazioni di stato dalla memoria ---------------------
  
  rsROM = EEPROM_ReadStr(mFlagStatoMacch);
  strcpy(StatoMacch, rsROM);
  free(rsROM);

  rsROM = EEPROM_ReadStr(mStatoSMS);
  strcpy(StatoSMS, rsROM);
  free(rsROM);

  rsROM = EEPROM_ReadStr(mCodAtt);
  strcpy(COD_ATT, rsROM);
  free(rsROM);
  
  rsROM = EEPROM_ReadStr(mTelServ);
  strcpy(TEL_SERV, rsROM);
  free(rsROM);

  rsROM = EEPROM_ReadStr(mResetWdt_Cnt); 
  int ResetWdt_Cnt = atoi(rsROM);
  free(rsROM);  

  rsROM = EEPROM_ReadStr(mQtaCarico); 
  int QtaCarico = atoi(rsROM);
  free(rsROM);

  rsROM = EEPROM_ReadStr(mQtaScarico); 
  int QtaScarico = atoi(rsROM);
  free(rsROM);

  rsROM = EEPROM_ReadStr(mFlagSottoscorta);
  strcpy(StatoSottoscorta, rsROM);
  free(rsROM);
  
  // ---------------------------- TEST di PRESENZA SIM --------------------------------
  // Se la SIM è presente va avanti con gli altri controlli altrimenti incrementa il Retry fino ad eventuale stato di manomissione

  char *SIMResp = SIM_ExecuteCmd("AT+CPIN?", 5000, "READY", "");
  bool SIMReady = (StrPos(SIMResp, "READY", 0) > 0);
  free(SIMResp);
 
  if(SIMReady){

    /*  1) lettura contatori SMS : SM,ME,MT : 2,20,20,50,2,70 ovvero SM 2,20 ME 20,50 MT 2,70 
           NB : Viene usata solo su seriale, in remoto non trasmette niente. Da attivare se serve !
    delay(200);

    //"AT+CPMS=\"SM\",\"ME\",\"MT\"+CPMS: 0,20,0,50,0,70OK";
    char *atCntSMS = SIM_ExecuteCmd("AT+CPMS=\"SM\",\"ME\",\"MT\"", 5000, "OK", ""); 
    int StartCnt = StrPos(atCntSMS, "+CPMS:", 0);
    if(StartCnt > 0){
      char *CntVal = SubStr(atCntSMS, StartCnt+6, strlen(atCntSMS)-(StartCnt+6)-1); // con -1 tolgo l'OK finale
      strcpy(CntSMS,"SM,ME,MT:");
      strcat(CntSMS,CntVal);
      free(CntVal);
    }else{
      strcpy(CntSMS, "Contatori SMS non rilevati !");
    }
    free(atCntSMS);
    */
    
    // 2) lettura livello segnale
    delay(200);

    char *atSIMSign = SIM_ExecuteCmd("AT+CSQ", 5000, "OK", "");
    int StartLiv = StrPos(atSIMSign, "+CSQ:",0);
    if(StartLiv > 0){
      char *tSign = SubStr(atSIMSign, StartLiv+5, strlen(atSIMSign)-(StartLiv+5)-1);  // con -1 tolgo l'OK finale
      tSign = FindAndReplace(tSign, ",",".");  // la virgola di decimale va sostituita con il punto
      tSign = FindAndReplace(tSign, " ","");  // elimina event spazi
      strcpy(SIMSignal,tSign);
      free(tSign);
    }else{
      strcpy(SIMSignal,"0");
    }
    free(atSIMSign);

    // ------- Legge il numero di telefono interno della SIM scritto in fase di inizializzazione scheda -----
    char *SimTel = SIM_ExecuteCmd("AT+CNUM",5000, "OK", "");
    // Parte dalla parola 'TEL' + l'offset al numero (StartPos) ...
    int StartPos = StrPos(SimTel,"TEL",0);
    // ... ed arriva alla prima virgoletta subito dopo il numero
    int EndPos = StrPos(SimTel,"\"",StartPos+6);
    if(StartPos > 0 && EndPos > StartPos){  // DP1
      char *NumTel = SubStr(SimTel,StartPos+6,EndPos-(StartPos+6));
      strcpy(TEL_SIM, NumTel);
      free(NumTel);
    }else{
      strcpy(TEL_SIM, "");
    }
    free(SimTel);   

  }else{ // il comando AT-CPIN -> Ready fallisce, inizio/continuo con il Retry !
    // ... leggo il retry presente sulla mem...
    char *RetryStr = EEPROM_ReadStr(mATRetry);  
    // ... aggiungo e poi memorizzo il cnt aggiornato ! 
    int RetryCnt = atoi(RetryStr) + 1;
    free(RetryStr);
    
    if(RetryCnt <= 9){ 
      char CntBuf[2]; // Nr + '\0' 
      itoa(RetryCnt,CntBuf,10);
      EEPROM_WriteStr(mATRetry, CntBuf);  
    }else{  // superata la soglia di retry la mette come manomessa
      strcpy(StatoMacch, "MS");  // manomissione da SIM rimossa
      EEPROM_WriteStr(mFlagStatoMacch, StatoMacch);  
    }
    
    strcpy(TEL_SIM, "");
  }

  // Il cod. di attivaz. scritto sulla eeprom è comprensivo di keyshift, quindi devo separarlo !
  char *KeyShift = SubStr(COD_ATT,1,2);
  char *CodAtt = SubStr(COD_ATT,3,17);

  if(strcmp(TEL_SIM, DecryptStr(CodAtt, atoi(KeyShift))) == 0){
    // Abilita qualcosa ...
  }else{
    strcpy(COD_ATT, "NA");
    // Disabilita qualcosa ... 
  }
  free(CodAtt); free(KeyShift);

  // se l'invio degli SMS è disabilitato da operatore ...
  if(strcmp(StatoSMS, "KO") == 0)
    DisableOverSMS = true; 

  if(DEBUG){
    Serial.print(F("***** LETTURA STATO MACCHINA *****, ver. firmware ")); Serial.println(String(VERS_FW));
    Serial.print(F("Telefono SIM : ")); Serial.print(String(TEL_SIM)); Serial.print(F(" Codice di attivazione : ")); Serial.print(String(COD_ATT)); 
    Serial.print(F(" - Stato manomissione : ")); Serial.println(String(StatoMacch));
    Serial.print(F("Telefono setup : ")); Serial.print(String(TEL_SETUP)); Serial.print(F(" - Telefono servizio : ")); Serial.println(TEL_SERV); 
    Serial.print(F("Contatori SMS SIM : ")); Serial.print(String(CntSMS)); Serial.print(F(" - Livello segnale : ")); Serial.println(String(SIMSignal)); 
    
    Serial.println(F("----------> Dati progressivi <-----------"));
    Serial.print(F("Q.tà Carico : ")); Serial.print(String(QtaCarico));
    Serial.print(F(" Q.tà Scarico : ")); Serial.print(String(QtaScarico));
    Serial.print(F(" Stato sottoscorta : ")); Serial.println(StatoSottoscorta);
    Serial.println(F("**********************************"));
  }

  // ritorna una stringa sintetica di stato inviabile 'di routine' per SMS ed elaborabile lato server o back office. Quando lo stato macchina
  // è chiesto da Seriale a seguito di procedure di prima installazione o assistenza tecnica.
  // Contatori ";CM:" + CntSMS; altro parametro aggiuntivo che è possibile mettere
  
  // IMPORTANTE : la "LSM" e "LSM_S" sono usate dal SIMP Manager o SIMP Server per acquisire inform. in fase di setup o di gestione della scheda : 
  // NON CAMBIARE la disposizione delle variabili, al massimo aggiungere le nuove in coda ! Vedi costanti in unit Common.pas in SIMP_Srv

  char *RetVal = (char *) malloc (150);
  sprintf(RetVal,"%s;%s;%s;%s;%s;%s;%s;%s;%d;%d;%s;%d;%s%s", TIPO_MAC,TEL_SIM,COD_ATT,VERS_FW,TEL_SETUP,TEL_SERV,SIMSignal,StatoMacch,QtaCarico,QtaScarico,StatoSottoscorta,ResetWdt_Cnt,StatoSMS,"|");
  
  //Serial.println(" LSM : " + String(RetVal));
  
  return RetVal;
}
// ------------------------------------------------------------------------------------
/* La lunghezza 'completa' della stringa è la stringa stessa + il '\0' di fine stringa, quindi se strLen = 9 allocherà 10 char sulla mem. 
   
  Esempio per leggere e scrivere numeri
  
  float FloatValue;
  String ValueStr;
  
  // Legge il numero come stringa e la converte in float ...
  ValueStr = EEPROM_ReadStr(memAddress);
  FloatValue = ValueStr.toFloat();
  // ... ne fa un operazione aritmetica e la riscrive modificata
  FloatValue = FloatValue + 1.10;
  EEPROM_WriteStr(memAddress, FormatNumber(FloatValue));

  NB : leggere le note di FormatNumber(...) per ulteriori dettagli
*/
void EEPROM_WriteStr(char *mAddress, char *Data)
{
  int DataSize = strlen(Data);

  if(strcmp(Data,"<ERASE_MEM>") == 0){  // Comando speciale di 'clear' memory 4(l'address viene ignorato)
    for (int i=0; i<EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }else{
    for(int i=0; i<DataSize; i++){
      EEPROM.write(mAddress+i, Data[i]);
    }
    EEPROM.write(mAddress+DataSize,'\0');   //Add termination null character for String Data
  }
    
  EEPROM.end();
}
// ---------------------------------------------------------------------------------------- 
char *EEPROM_ReadStr(char *mAddress)
{
  String RespTmp = "";
  static char *RetVal;
  int DataLen = 0;
  char k;

  k = EEPROM.read(mAddress);
  while(k!='\0' && k!=0xff){   //Read until null character
    k = EEPROM.read(mAddress + DataLen);
    RespTmp += k;
    DataLen++;
  }
  RespTmp += '\0';

  RetVal = malloc(RespTmp.length()+1);
  RespTmp.toCharArray(RetVal, RespTmp.length()+1);
  
  return RetVal;
}
// ---------------------------------------------------------------------------------------
// Formatta i numeri float/interi con gli zeri iniziali. Serve a scrivere il numero in formato stringa sulla EEPRom in modo  
// che occupi sempre lo stesso spazio di memoria. La classica configurazione dovrebbe essere Len = 9, Prec = 2 ovvero 6 interi, 2 dec. + virgola = 9 !
// Esempio : float 300.11 -> formattato 000300.11
char *FormatNumber(float Number, int Len, int Prec){
  static char ResString[10];
    
  dtostrf(Number, Len, Prec, ResString);
  for (int i = 0; i < Len+1; i++) {
    if (ResString[i]==' ') ResString[i]='0';
  }
  return ResString;
}
// -----------------------------------------------------------------------------------------------------------------
char *SIM_ExecuteCmd(char *Command, int Timeout, char *WaitFor1, char *WaitFor2)  //Send command function
{
  char RespTmp[200] = "";
  bool RespOK = false, AssignResp = false;
  static char *Response;

  // Prima del lancio del nuovo cmd svuoto tutto 
  while(SIM800.available()){SIM800.read();}   // CF1

  SIM800.println(Command);
  
  unsigned long lastRead = millis();   // last time a char was available
  while((SIM800.available() == 0) && (millis() - lastRead < Timeout));  // aspetto il primo char

  // Se non ricevo caratteri entro un determinato timeout, sicuramente ci sono problemi  
  if(SIM800.available() == 0){   // DP1
    Response = malloc_strcpy("TIMEOUT");

    if(DEBUG){  
      Serial.println("SIM_ExecuteCmd -> Cmd: "+String(Command)+" Response: "+String(Response));
    }
    
    return Response;
  }
  int i=0; 
  lastRead = millis();   // last time a char was available
  while ((millis() - lastRead < Timeout) && !RespOK){   
    
    while(SIM800.available() > 0 && !RespOK){
      char inChar = (char)SIM800.read();
      if(i < 200 && inChar != '\n' && inChar != '\r'){    
        RespTmp[i++] = inChar; // prima assegna e poi incrementa
        RespTmp[i] = '\0'; 
      }
      lastRead = millis();   // update the lastRead timestamp
    }

    // DA COMMENTARE
    //if(strcmp(RespTmp, "") != 0){
      //Serial.println("sim : "+String(RespTmp));
    //}  

    // Eccezione ! Nell'elenco SMS nella box esce subito al primo sms trovato. Lo elaboro uno alla volta
    if(StrPos(Command,"CMGL",0) > 0 && (StrPos(RespTmp,"REC READ",0) > 0 || StrPos(RespTmp,"REC UNREAD",0) > 0 || StrPos(RespTmp,"OK",0) > 0)){
      RespOK = true;
    }

    // MODIFICA
    // Eccezione : 'intercetto' a monte tutti i comandi che posso ricevere in quanto ce ne sono alcuni inizianti con la parola chiave 'OK' che possono 
    // sfuggire alla rilevazione. La SIM ad un certo punto risponde, a fine stringa, come '...|OKM|OK' ci sono 2 OK e col WaitFor1/2 è un problema intercettarli
    // in quanto uscirebbe prima al '|OK...'
    
    if(StrPos(Command,"CMGR",0) > 0){
      int StartTag = StrPos(RespTmp,"|",0);
      int EndTag = StrPos(RespTmp,"|",StartTag+1); 

      if(StartTag > 0 && EndTag > StartTag){
        AssignResp = true;
        RespOK = true;
        break;
      }
    }

    // Negli errori mi basta solo il msg apposito
    if(StrPos(RespTmp,"ERROR",0) > 0){
      strcpy(RespTmp, "ERROR");
      break;
    }

    if(StrPos(RespTmp,"NORMAL POWER DOWN",0) > 0){
      strcpy(RespTmp, "NORMAL POWER DOWN");
      break;
    }
    
    if(((strcmp(WaitFor1, "") != 0) && StrPos(RespTmp,WaitFor1,0)>0) || ((strcmp(WaitFor2, "") != 0) && StrPos(RespTmp,WaitFor2,0)>0)){
      AssignResp = true;
      RespOK = true;
    }
  }
  // Se in waitfor 1/2 non c'è alcun parametro e response non ha msg di ritorno (come gli errori) si intende che occorre ritornare
  // con l'intera stringa di riposta come ad esempio nei cmd CMGR=1 per acquisire il contenuto degli SMS
  if((!AssignResp)&&(strcmp(WaitFor1, "") == 0)&&(strcmp(WaitFor2, "") == 0)){
    AssignResp = true;
  }

  // NB : ci sono alcuni comandi che non ritornano nulla MA nel response devo far tornare comunque una stringa vuota e prima 
  // del return controllo se il ptr è stato assegnato altrimenti lo assegno vuoto
  if(AssignResp){
    Response = malloc_strcpy(RespTmp);
  }else{
    Response = malloc_strcpy("");
  }

  if(DEBUG){  
    Serial.println("SIM_ExecuteCmd -> Cmd: "+String(Command)+" Response: "+String(Response));
  }
  
  return Response;  // NB sarà il chiamante nella var di ritorno a fare poi il free()
}
// ----------------------------------------------------------------------------------------------
// MODIFICA TUTTO AGGIORNATO
// Sta in 'ricezione' di sms live MA se GetFromMem = true va nella memoria e testa se ci sono sms arrivati e non acquisiti
// in live in quanto la SIM potrebbe essere stata disconnessa.

bool Get_SMS(int SMS_Id, const bool GetFromMem)
{
  char *Response;
  bool RetVal = false;

  // La priorità di ricezione degli SMS è data a quelli live in arrivo, in assenza di nuovi sms va a cercare nella mem
  // degli SMS già arrivati e non acquisiti live, quando il GetFromMem = true 
  
  // ---------------------------------------------------------
  if(SMS_Id == 0 && GetFromMem){
    Response = SIM_ExecuteCmd("AT+CMGL=\"ALL\"", 5000, "REC READ", "REC UNREAD"); 
    
    // Se sto cercando degli sms Ricevuti (Letti o non letti) nella mem della SIM, dall'istruzione di elenco lanciata prima (AT+CMGL=\"ALL\")
    // rilevo dal box di mem, uno alla volta con il tempo di LeggiStatoMacchina, gli SMS con il tag 'Rec Read' o 'Rec Unread' per poi elaborarli
    // col Loop di gestione SMS 
    
    if(strcmp(Response, "") != 0){
      int StartPos = StrPos(Response,"+CMGL:",0);
      // ... ed arriva alla prima virgola subito dopo il cmd
      int EndPos = StrPos(Response,",",StartPos);
      if(StartPos > 0 && EndPos > StartPos) 
        SMS_Id = atoi(SubStr(Response,StartPos+6,(EndPos-(StartPos+6))));
    }
    free(Response);
  }

  //Serial.println("SMS_Id "+String(SMS_Id));
  
  // -------------------------------- Elaborazione SMS ricevuto ----------------------------------

  if(SMS_Id > 0){
    //Serial.println("AT+CMGF=1"); // Imposta la ricezione in formato testo
    char *SimEC = SIM_ExecuteCmd("AT+CMGF=1", 1000, "OK", ""); 
    //Serial.println("SimEC  "+String(SimEC));
    free(SimEC);
    
    // Recupera l'SMS dalla mem tramite ID
    char SIMPar[15];
    sprintf(SIMPar,"AT+CMGR=%d",SMS_Id);
    char *RespCMGR = SIM_ExecuteCmd(SIMPar, 2000, "", "");

    // inizializza l'array delle parti info del messaggio ricevuto
    for(int i=0; i<Info_Size; i++){
      if(Info_Ric[i]){
        free(Info_Ric[i]);
        Info_Ric[i] = nullptr;
      }
    }

    //Serial.println("RespCMGR "+String(RespCMGR));
    
    // Ricevuto l'SMS devo estrarre le varie parti (numero tel. e corpo del msg) 
    if(strcmp(RespCMGR, "") != 0){
      // 1) Estraggo il numero di telefono del mittente che sta tra un "+ ... "
      int FromIdx = StrPos(RespCMGR, "\"+", 0);   
      int ToIdx = StrPos(RespCMGR, "\"", FromIdx+1);  
      
      if(FromIdx > 0 && ToIdx > FromIdx){
        char *RespTmp = SubStr(RespCMGR, FromIdx+1, (ToIdx-FromIdx)-1); 
        Info_Ric[0] = malloc_strcpy(RespTmp);  // Nr. di tel. del mittente
        free(RespTmp);
      }

      // 2) Estraggo il messaggio e lo suddivido nelle sue sottoparti con ExtractToInfoRic

      FromIdx = StrPos(RespCMGR, "|", 0);
      ToIdx = StrPos(RespCMGR, "|", FromIdx+1);

      if(FromIdx > 0 && ToIdx > FromIdx){
        char *RespTmp1 = SubStr(RespCMGR, FromIdx+1, (ToIdx-FromIdx)-1); // Intero messaggio senza | ... |
      
        //Serial.println("Msg ric. : "+String(RespTmp1));

        // 0 è il Nr. di tel di setup/servizio, dal 1 in poi le varie parti del messaggio come il cmd, cnt carico, cnt scarico, ecc... 
        // nonchè delle locazioni dell'array Info_Ric[...]
        ExtractToInfoRic(RespTmp1,1);

        free(RespTmp1);
      }        

      // una volta estratte le info necessarie lo elimina in base all'indice assegnato per non fa occupare mem sulla SIM   
      char SIMPar1[15];
      sprintf(SIMPar1,"AT+CMGD=%d",SMS_Id);
      char *SimEC1 = SIM_ExecuteCmd(SIMPar1, 2000, "OK", ""); free(SimEC1);

      RetVal = true;
    }
    free(RespCMGR);
  }

  if(RetVal){
    // in Info_Ric ci sono tutte le parti del comando inviato via SMS e quindi qua li devo ricomporre sulla base del tipo comando (es. INI) 
    // e dei parametri associati in modo che vengano poi eseguiti 
    // (B : IMPORTANTE)
    // Se ho inoltrato una richiesta col tel di setup o tel. servizio, la risposta mi deve essere data sempre sul telefono con cui ho inoltrato 
    // la richiesta. Diversamente (con altri numeri non riconosciuti) viene segnalata una manomissione per Intrusione ! 
    if((strcmp(Info_Ric[0], TEL_SETUP) == 0) || (strcmp(Info_Ric[0], TEL_SERV) == 0)){

      strcpy(TEL_CORR, Info_Ric[0]); 

    }else{         // Nell'intrusione svuota il comando per non far poi eseguire nulla
      free(Info_Ric[1]);   // Nell'intrusione svuota il comando per non far poi eseguire nulla
      Info_Ric[1] = nullptr;  

      if(DEBUG){
        Serial.println("ATTENZIONE... Tentata intrusione da "+String(Info_Ric[0]));
      }
    }
  }

  return RetVal;
}
// ----------------------------------------------------------------------------------------------
void Send_SMS(char *SMSMsg)
{
  // IMPORTANTE : Non mettere qua impostazioni sul Tel_Corr : è già impostato alla richiesta. Leggi (B : IMPORTANTE)
  // Il tel corr può cambiare in base alla richiesta ovvero la risposta deve essere data a chi ne fa richiesta !
  
  if(DEBUG){
    Serial.println("Invio risposta al "+String(TEL_CORR));
  }

  // Prima del lancio del nuovo cmd svuoto tutto 
  while(SIM800.available()){SIM800.read();}   

  // richiedo l'invio del SMS
  char *SIMPar = malloc(30);
  sprintf(SIMPar,"AT+CMGS=\"%s\"\r",TEL_CORR);
  SIM800.println(SIMPar);
  free(SIMPar);

  delay(1000); // attendo il prompt '>' ...

  // ... scrivo il Msg ed invio
  char cSMSMsg[150];
  strcpy(cSMSMsg, SMSMsg);
  strcat(cSMSMsg,"\x1A");
  
  SIM800.println(cSMSMsg);
}
// ------------------------------------------------------------------------------------------------------
// Elabora un comando speciale ricevuto come INI o CAR ed estrae le parti mettendole nell'array globale Info_Ric
// StartPart -> indica da quale cella nell'array Info_Ric occorre partire a mettere i valori estratti (deve essere < di Info_Size)
// ad ogni separatore di parte (;) passa a rilevare la successiva
// (   Cmd;Cod. attivaz.|tel scheda;  Tel serv.;      Sgl_LTO_Usc_Min, Sgl_LTO_Usc_Max;  Sgl_TDS_Min;  Sgl_TDS_Max;  Temper_Reg;  LTO_Usc_Reg;  TDS_Reg  LT_Tot_Prod
// Es. INI;10VINIK2PFIPBNS;           +393487546567;  00030;           00150;            00020;        00400;        00016.5;     00040;        00100;   00022.3;/OLD
// Es. INI_S;+393498792983;           +393487546567;  00030;           00150;            00020;        00400;        00016.5;     00040;        00100;   00022.3;/OLD
void ExtractToInfoRic(char *CmdPart, int StartPart){
  char *sPart = strtok(CmdPart, ";");
  while (sPart){
    if(StartPart < Info_Size)
      Info_Ric[StartPart++] = malloc_strcpy(sPart);

    sPart = strtok(NULL, ";");
  }
}
// ------------------------------------------------------------------------------------------------------
// ATTENZIONE : E' la funzione corrispondente fatta in delphi (SIMP_Man -> DM1.EncryptStr(...):String;) che crea il cod. di attivazione della macchina 
// che viene poi utilizzato dal Fw tramite questa funzione che decripta il codice passato e ne ricava un IMEI che deve essere uguale a quello della macchina
// Quindi qualsiasi variazione fatta qua deve essere fatta anche nella procedura in Delphi. Valori ottimali KeyShift : da 5 a 30 (adesso uso 10 come KeyShift)
char *DecryptStr(char *StrToDecrypt, int KeyShift)
{  
  char AlphaEncode[37] = "QB1C4DFG6IXZJ0KL8M5OA+WPR9ST2UHVE7YN3";
  static char RetVal[30]; char ch;
 
  for(int i = 0; StrToDecrypt[i] != '\0'; ++i){
    ch = StrToDecrypt[i];

    for(int j=0; j<37; j++){
      if(AlphaEncode[j] == ch){
        if(j >= KeyShift)
          RetVal[i] = AlphaEncode[j-KeyShift];
        else
          RetVal[i] = AlphaEncode[37-(KeyShift-j)];
        
        RetVal[i+1] = '\0'; 
      }
    }
  }
  return RetVal;
}
// ---------------------------------------------------------------
// SubStr Stringa, pos del char di partenza x nr. char da prelevare, 
// NB : Ritorna il puntatore all Sub string ricavata e già dimensionata con malloc, quindi da 'free' alla fine !
char *SubStr( const char *s, size_t pos, size_t n ){
  size_t length = strlen( s );
  pos = pos-1; //+1 : il primo char parte da 1 e non da zero !
  if ( !( pos < length ) ) return NULL;
  if ( length - pos < n ) n = length - pos;
  char *t = malloc( ( n + 1 ) * sizeof( char ) );
  if ( t ){
    strncpy( t, s + pos, n );
    t[n] = '\0';
  }
  return t;
}
// ---------------------------------------------------------------
// hay -> intera stringa in cui cercare, needle -> carattere/stringa da cercare, offset -> punto da dove iniziare
int StrPos(char *haystack, char *needle, int offset){
  int i = 0, d = 0;
  if(strlen(haystack) >= strlen(needle) && strlen(haystack) > offset){
    for(i = 0; i < strlen(haystack); i++) {
      int found = 1; //assume we found (wanted to use boolean)
      for(d = 0; d < strlen(needle); d++) {
        if(haystack[i + d + offset] != needle[d]) {
          found = 0; 
          break;
        }
      }
      if(found == 1)
       return offset+i+1;  // inizio da 1 
    }
    return -1;
  }else{
    return -1; //fprintf(stdout, "haystack smaller\n"); 
  }
} 
// ---------------------------------------------------------------
char *FindAndReplace(char *haystack, const char *needle, const char *repl){
  size_t needle_len;
  size_t repl_len;
  size_t haystack_len;
  const char *pos = strstr(haystack, needle);

  if(pos){
    needle_len = strlen(needle);
    repl_len = strlen(repl);
    haystack_len = strlen(haystack);

    if(needle_len != repl_len){
        memmove((void *)(pos + repl_len), (void *)(pos + needle_len), haystack_len - (pos - haystack) + 1);
    }

    memcpy((void *)pos, (void *)repl, repl_len);
  }
  return haystack;
}
// ----------------------------------------------------------------------------------------
char *malloc_strcpy(char *Source){
  char *Temp = NULL;

  if((Temp = malloc(strlen(Source)+1)) == NULL) {
    fprintf(stderr, "Memory not allocated\n");
    exit(EXIT_FAILURE);
  }
  strcpy(Temp, Source);
  return Temp;
}
// -----------------------------------------------------------------------------------------
void wdt_Setup(void)
{
  cli();  // disable all interrupts
  wdt_reset(); // reset the WDT timer
  /*
  WDTCSR configuration:
  WDIE = 1: Interrupt Enable
  WDE = 1 :Reset Enable
  WDP3 = 1 :For 8000ms Time-out
  WDP2 = 0 :For 8000ms Time-out
  WDP1 = 0 :For 8000ms Time-out
  WDP0 = 1 :For 8000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Set Watchdog settings:
  WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);  // Impostata a 8 sec. di timeout
  sei();
}
// ----------------------------------------------------------------------------------------
// IMPORTANTE : non usare Serial.println che blocca il reset 
ISR(WDT_vect) // Watchdog timer interrupt.
{
  // ... leggo il resetCnt presente sulla mem...
  char *ResetCntStr = EEPROM_ReadStr(mResetWdt_Cnt);  
  // ... aggiungo e poi memorizzo il cnt aggiornato ! 
  int ResetCnt = atoi(ResetCntStr) + 1;
  free(ResetCntStr);
 
  char CntBuf[4]; // Nr + '\0' 
  itoa(ResetCnt,CntBuf,10);
  EEPROM_WriteStr(mResetWdt_Cnt, CntBuf);  
  
  // Reset del modulo SIM800 : non va mi manda in blocco il modulo SIM al riavvio, non ricevo più sms
  digitalWrite(pSIM_Boot, LOW);
  // IMPORTANTE : non uso SIM_ExecuteCmd per evitare accidentalmente l'uso di Serial.println che bloccano il reset 
  SIM800.println("AT+CPOWD=1"); 
  delay(3000);
  //digitalWrite(pSIM_Boot, HIGH);   // lo fa al restart

  // Reset della scheda
  asm volatile("  jmp 0"); 
}
