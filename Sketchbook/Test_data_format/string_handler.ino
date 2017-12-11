


void str_handler() {
  String pre_str_volt = "string print value: ";

  String str_volt = String(f_volt);
  //consider pointer in case 
  //String str_topic = String((char*)topic);
   
  // print combined string
  Serial.println(pre_str_volt + f_volt);

  // combine string as new one, and print, str + var_str + float + str + int
  combine_str = "print combined str: " + pre_str_volt + f_volt + "..end of test.." + 123;
  Serial.println(combine_str);

  delay(1000);
}

void str_tochar() {
  char array1[5];
  combine_str.toCharArray(array1, 5);
  Serial.println(array1);

  int str_len = combine_str.length() + 1;
  char combined_char[str_len];
  combine_str.toCharArray(combined_char, str_len);
  Serial.println(combined_char);
  delay(1000);
}

// find a string in string
void find_str() {
  char *str = "The quick brown fox jumped over the lazy dogs.";
  char *p;

  p = strstr(str, "lazy");
  Serial.println(p); // "lazy dogs."

  // p is NULL after this, since the string "wombat" isn't in str:
  p = strstr(str, "wombat");
  Serial.println(p); // "lazy dogs."
  delay(1000);
}

