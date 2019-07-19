
bool read_data () {
  while (1) {
    if (mySerial.available() > 0) {
      // read first two bytes, 2nd bytes must be 5A to confirm
      int byte0 = mySerial.read();
      delay(2);
      int byte1 = mySerial.read();
      delay(2);

      if ( byte1 == 0x5A ) {
        hlw[0] = byte0;
        hlw[1] = byte1;
        // read and save all 23 bytes
        for (int i = 2; i < 23; i++) {
          int incomingByte = mySerial.read();
          delay(2);
          hlw[i] = incomingByte;
        }
        return true;
        break;
      }
    }
  }
}

// this is not using
//char COVT_data (int cov_data) {
//  String VAL_print = String(cov_data);
//  VAL_print.toCharArray(Printout, 4);
//  Serial.println(VAL_print);
//  return Printout;
//}

//void print_raw_data () {
//  Serial.println("Raw data - ");
//  for (int i = 0; i < 23; i++) {
//
//    Serial.print(hlw[i], BIN);
//    Serial.print(' ');
//  }
//  Serial.println();
//}


void volt_cal () {
  //220V = (volt_reg / volt_value ) * Volt_co
  // Volt_co = 220V / (volt_reg / volt_value )
  
  
  volt_reg   =  hlw[2] * 256L * 256L + hlw[3] * 256L + hlw[4];
  volt_value =  hlw[5] * 256L * 256L + hlw[6] * 256L + hlw[7];
  //volt_co = 220L / (volt_reg / volt_value);
  volt_co = 220L * volt_value / volt_reg;

  
}


void print_parse () {
  sprintf (buff, "Status register:          %02x", hlw[0]);
  Serial.println(buff);

  sprintf (buff, "Check register:           %02x", hlw[1]);
  Serial.println(buff);

  sprintf (buff, "VOLT default register:    %02x %02x %02x ", hlw[2], hlw[3], hlw[4]  );
  Serial.println(buff);

  sprintf (buff, "VOLT register:            %02x %02x %02x ", hlw[5], hlw[6], hlw[7]  );
  Serial.println(buff);

  sprintf (buff, "CURRENT default register: %02x %02x %02x ", hlw[8], hlw[9], hlw[10]  );
  Serial.println(buff);

  sprintf (buff, "CURRENT register:         %02x %02x %02x ", hlw[11], hlw[12], hlw[13]  );
  Serial.println(buff);

  sprintf (buff, "POWER default register:   %02x %02x %02x ", hlw[14], hlw[15], hlw[16]  );
  Serial.println(buff);

  sprintf (buff, "POWER register:           %02x %02x %02x ", hlw[17], hlw[18], hlw[19]  );
  Serial.println(buff);

  sprintf (buff, "Update register:          %02x ", hlw[20]);
  Serial.println(buff);

  sprintf (buff, "PF register:              %02x %02x ", hlw[21], hlw[22]);
  Serial.println(buff);

  sprintf (buff, "CHECK_SUM register:       %02x ", hlw[23]);
  Serial.println(buff);
}


// not for ZMPT
void process_data () {
  float voltage_ref, voltage_cal, volt;
  float currnt_ref, current_cal, current;
  float pwr_ref, pwr_cal, pwr;
  float pulse, pwr_used;


  voltage_ref =   hlw[2] * 65536 + hlw[3] * 256 + hlw[4];
  voltage_cal =   hlw[5] * 65536 + hlw[6] * 256 + hlw[7];
  volt = (voltage_ref / voltage_cal) * 1.88;   // voltage modulus = (4*470K)/(1K*1000) = 1.88

  currnt_ref =   hlw[8] * 65536 + hlw[9] * 256 + hlw[10];
  current_cal =  hlw[11] * 65536 + hlw[12] * 256 + hlw[13];
  current = (currnt_ref / current_cal) * 1;   // current modulus = 0.001/(R*1000) = 1

  pwr_ref =   hlw[14] * 65536 + hlw[15] * 256 + hlw[16];
  pwr_cal =   hlw[17] * 65536 + hlw[18] * 256 + hlw[19];
  pwr = (pwr_ref / pwr_cal) * 1.88 * 1;   //

  pulse = hlw[21] * 256 + hlw[22];

  pwr_used = (65536 * 2 + pulse) * ( pwr_ref * 1.88 * 1 ) / (3600 * 1000000000);

  // convert data to char array
  dtostrf(volt, 3, 1, volt_temp);
  dtostrf(current, 3, 1, current_temp);
  dtostrf(pwr, 4, 1, pwr_temp);
  dtostrf(pulse, 4, 1, pulse_temp);
  dtostrf(pwr_used, 4, 0, pwr_used_temp);

  //sprintf (buff2, "voltage is: %f",vl);           // float is NOT supported by arduino!!!


  sprintf (buff, "V: %s, C: %s, P: %s", volt_temp, current_temp, pwr_temp);
  sprintf (buff2, "PP: %s, PS: %s", pulse_temp, pwr_used_temp);
  Serial.println (buff);
  Serial.println (buff2);

  //Serial.println (pwr_ref);
  //Serial.println (pwr_cal);
  Serial.println (pwr);

}


//void LCD1602print () {
//  lcd.setCursor(0, 0);
//  sprintf (buff,  "%s V", volt_temp);
//  lcd.print(buff);
//
//  lcd.setCursor(8, 0);
//  sprintf (buff,  "%s A", current_temp);
//  lcd.print(buff);
//
//  lcd.setCursor(0, 1);
//  sprintf (buff,  "%s W", pwr_temp);
//  lcd.print(buff);
//
//  lcd.setCursor(8, 1);
//  sprintf (buff,  "%s WH", pwr_used_temp);
//  lcd.print(buff);
//}





void process_data_2 () {
  float voltage_ref, voltage_cal, volt;
  float currnt_ref, current_cal, current;
  float pwr_ref, pwr_cal, pwr;
  float pulse, pwr_used;
  //char volt_temp[7], current_temp[7], pwr_temp[7], pulse_temp[7], pwr_used_temp[7];

  voltage_ref =   hlw[2] * 65536 + hlw[3] * 256 + hlw[4];
  voltage_cal =   hlw[5] * 65536 + hlw[6] * 256 + hlw[7];

  volt = (voltage_ref / voltage_cal) * 1.88;   // voltage modulus = (4*470K)/(1K*1000) = 1.88

  currnt_ref =   hlw[8] * 65536 + hlw[9] * 256 + hlw[10];
  current_cal =  hlw[11] * 65536 + hlw[12] * 256 + hlw[13];
  current = (currnt_ref / current_cal) * 1;   // current modulus = 0.001/(R*1000) = 1

  pwr_ref =   hlw[14] * 65536 + hlw[15] * 256 + hlw[16];
  pwr_cal =   hlw[17] * 65536 + hlw[18] * 256 + hlw[19];
  pwr = (pwr_ref / pwr_cal) * 1.88 * 1;   //

  pulse = hlw[21] * 256 + hlw[22];

  pwr_used = (65536 * 2 + pulse) * ( pwr_ref * 1.88 * 1 ) / (3600 * 1000000000);

  // convert data to char array
  dtostrf(volt, 3, 1, volt_temp);
  dtostrf(current, 3, 1, current_temp);
  dtostrf(pwr, 4, 1, pwr_temp);
  dtostrf(pulse, 4, 1, pulse_temp);
  dtostrf(pwr_used, 4, 0, pwr_used_temp);
}
