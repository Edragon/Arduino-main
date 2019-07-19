void write_line_1 (int title, int data, int title_2, int data_2, int title_3, int data_3, int title_4, int data_4, int title_5, int data_5) {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255, 255, 255);

  TFTscreen.text(title, 0, 0);
  TFTscreen.text(title_2, 0, 10);

  TFTscreen.text(title_3, 0, 20);
  TFTscreen.text(title_4, 0, 30);
  TFTscreen.text(title_5, 0, 40);

  // data reflash
  TFTscreen.text(data, 40, 0);
  TFTscreen.text(data_2, 40, 10);

  TFTscreen.text(data_3, 40, 20);
  TFTscreen.text(data_4, 40, 30);
  TFTscreen.text(data_5, 40, 40);
  delay(250);

  TFTscreen.stroke(0, 0, 0);

  TFTscreen.text(data, 40, 0);
  TFTscreen.text(data_2, 40, 10);

  TFTscreen.text(data_3, 40, 20);
  TFTscreen.text(data_4, 40, 30);
  TFTscreen.text(data_5, 40, 40);
}



void O_title (int title, int Y) {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255, 255, 255);

  TFTscreen.text(title, 0, Y);
}

void O_data (int data, int Y) {
  TFTscreen.stroke(255, 255, 255);
  // data reflash
  TFTscreen.text(data, 50, Y);
}

void F_data (int data, int Y) {
  TFTscreen.stroke(0, 0, 0);
  // data reflash
  TFTscreen.text(data, 50, Y);
}



void simple_write_3(int data, int data2, int data3) {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255, 255, 255);

  //
//  String VAL_print = String(data);
//  VAL_print.toCharArray(Printout, 20);
//  
//  String VAL_print2 = String(data2);
//  VAL_print2.toCharArray(Printout1, 20);
//  
//  String VAL_print3 = String(data3);
//  VAL_print3.toCharArray(Printout2, 20);

  ltoa(data, Printout, 10);
  
//  sprintf (buff, "%ld", data);  //
//  sprintf (buff1, "%u", data2);  //
//  sprintf (buff2, "%u", data3);  //

  TFTscreen.text(Printout, 0, 0);
  TFTscreen.text(Printout1, 0, 10);
  TFTscreen.text(Printout2, 0, 20);

  delay(250);
  TFTscreen.stroke(0, 0, 0);

  TFTscreen.text(Printout, 0, 0);
  TFTscreen.text(Printout1, 0, 10);
  TFTscreen.text(Printout2, 0, 20);

}



void simple_write2(int data) {
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255, 255, 255);

  TFTscreen.text(data, 0, 10);

  delay(250);

  TFTscreen.stroke(0, 0, 0);

  TFTscreen.text(data, 0, 10);
}


void HLW8032_REG_PRINT() {
  //char data1 = COVT_data (hlw[0]);
  sprintf (buff, "%02X", hlw[1]);  // 5A flag

  sprintf (buff1, "%02X %02X %02X ", hlw[5], hlw[6], hlw[7] ); // voltage
  sprintf (buff2, "%02X %02X %02X ", hlw[11], hlw[12], hlw[13] ); // current
  sprintf (buff3, "%02X %02X %02X ", hlw[17], hlw[18], hlw[19] ); // power

  sprintf (buff4, "%02X", hlw[20]);  // update
  sprintf (buff5, "%02X %02X ", hlw[21], hlw[22] ); // power

  O_title ("FLAG:", 0);
  O_title ("VOLT: ", 10);
  O_title ("CURRENT: ", 20);
  O_title ("PWR: ", 30);
  O_title ("Update: ", 40);
  O_title ("PF: ", 50);

  O_data(buff, 0);
  O_data(buff1, 10);
  O_data(buff2, 20);
  O_data(buff3, 30);
  O_data(buff4, 40);
  O_data(buff5, 50);

  delay(300);
  F_data(buff, 0);
  F_data(buff1, 10);
  F_data(buff2, 20);
  F_data(buff3, 30);
  F_data(buff4, 40);
  F_data(buff5, 50);


  //write_line_1 ("FLAG:", buff, "VOL: ", buff1, "C: ",  buff2, "PWR:", buff3, "update:", buff4, "PF:", buff5);

}



void HLW8032_DATA_PRINT() {
  //char data1 = COVT_data (hlw[0]);
  sprintf (buff, "%02X", hlw[1]);  // 5A flag

  sprintf (buff1, "%s ", volt_temp); // voltage
  sprintf (buff2, "%s ", current_temp ); // current
  sprintf (buff3, "%s ", pwr_temp ); // power

  sprintf (buff4, "%s", pwr_used_temp);  // update
  sprintf (buff5, "%s ", pulse_temp ); // power

  O_title ("FLAG:", 0);
  O_title ("VOLT: ", 10);
  O_title ("CURRENT: ", 20);
  O_title ("PWR: ", 30);
  O_title ("Update: ", 40);
  O_title ("PF: ", 50);

  O_data(buff, 0);
  O_data(buff1, 10);
  O_data(buff2, 20);
  O_data(buff3, 30);
  O_data(buff4, 40);
  O_data(buff5, 50);

  delay(300);
  F_data(buff, 0);
  F_data(buff1, 10);
  F_data(buff2, 20);
  F_data(buff3, 30);
  F_data(buff4, 40);
  F_data(buff5, 50);


  //write_line_1 ("FLAG:", buff, "VOL: ", buff1, "C: ",  buff2, "PWR:", buff3, "update:", buff4, "PF:", buff5);

}
