int latchPin = 13;//ST_CP
int clockPin = 12;//SH_CP
int dataPin = 11; //DS
// define latch, clock and datapin


void setup ()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //set these three pins to output
}

//Display function (display word, display position, display if have decimal point)
void disNum(int Num_Qua, int Num_Pos, int Num_Dec)
{
  int Num_Qua_B = 0;
  int Num_Pos_B = 0;
  switch (Num_Qua)
  {
    case 0: Num_Qua_B = 192; break;
    case 1: Num_Qua_B = 249; break;
    case 2: Num_Qua_B = 164; break;
    case 3: Num_Qua_B = 176; break;
    case 4: Num_Qua_B = 153; break;
    case 5: Num_Qua_B = 146; break;
    case 6: Num_Qua_B = 130; break;
    case 7: Num_Qua_B = 248; break;
    case 8: Num_Qua_B = 128; break;
    case 9: Num_Qua_B = 144; break;
    default: Num_Qua_B = 255; break;
  };
  //this value convert to binary, which is adcdefg need to light up
  switch (Num_Pos)
  {
    case 1: Num_Pos_B = 1; break;
    case 2: Num_Pos_B = 2; break;
    case 3: Num_Pos_B = 4; break;
    case 4: Num_Pos_B = 8; break;
    case 5: Num_Pos_B = 16; break;
    case 6: Num_Pos_B = 32; break;
    case 7: Num_Pos_B = 64; break;
    case 8: Num_Pos_B = 128; break;
    default: Num_Pos_B = 0; break;
  };
  //this value convert to binary, which is the one light up now.
  if (Num_Dec == 1)
  {
    Num_Qua_B = Num_Qua_B + 128;
  };
  //128是Q7端口2进制值转换为10进制的结果。
  digitalWrite(latchPin, LOW); // set latch to low tolet the display ready to receive data
  int L = Num_Qua_B; int R = Num_Pos_B;
  shiftOut(dataPin, clockPin, MSBFIRST, R);
  shiftOut(dataPin, clockPin, MSBFIRST, L);
  digitalWrite(latchPin, HIGH); //restore to high TTL back
  //上面是74HC595的输出方式 每个74HC595能接受一个8位的2进制数值来电灯 第一个74HC595再次输入一个8位的数值他会把他现在的数值传给第二个串联的74HC595。
  delayMicroseconds(100);
}
//this is the display number function
void showNum(double number)// use double instead of int, int is not longer enough
{
  long  num = number;
  int ShowDec [8];
  for (int x = 1; x < 9; x++)
  {
    ShowDec [x] = 0;
  }
  //creat the array for decimal point, 0 is not to display, 1 is to display

  if (num  < 10000000) {
    if (num  < 1000000) {
      num = number * 100;
      ShowDec[6] = 1;
      ShowDec[7] = 0;
    }
    else {
      num = number * 10; ShowDec[7] = 1;
    }
  }
  //simple decimal point for two number display


  int ShowNumber [8];
  ShowNumber[1] = (num / 10000000) % 10;
  ShowNumber[2] =  (num / 1000000) % 10;
  ShowNumber[3] = (num / 100000) % 10;
  ShowNumber[4] = (num / 10000) % 10;
  ShowNumber[5] = (num / 1000) % 10;
  ShowNumber[6] = (num / 100) % 10;
  ShowNumber[7] = (num / 10) % 10;
  ShowNumber[8] = (num / 1) % 10;
  //get every number and store in the arrary



  int dectest = 0;
  for (int x = 1; x < 8; x++)
  {
    if ((dectest == 0) and (ShowNumber[x] == 0))
    {
      ShowNumber[x] = 11;
    }
    else
    {
      dectest = 1;
    }
  }
  //check if there is a "0" on the left front, if yes we ignore it


  disNum(ShowNumber[1], 1, ShowDec[1]);
  disNum(ShowNumber[2], 2, ShowDec[2]);
  disNum(ShowNumber[3], 3, ShowDec[3]);
  disNum(ShowNumber[4], 4, ShowDec[4]);
  disNum(ShowNumber[5], 5, ShowDec[5]);
  disNum(ShowNumber[6], 6, ShowDec[6]);
  disNum(ShowNumber[7], 7, ShowDec[7]);
  disNum(ShowNumber[8], 8, ShowDec[8]);
  //display number in sequency, flashing fast
}

void loop()
{
  showNum(13456.8);//simple function to display to number
}
