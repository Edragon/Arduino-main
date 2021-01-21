//concat() = append string

// strcpy
// strcpy(dest, "I like "); // dest is now "I like "

// strcat = connect stris

// strcat(dest, src);
void str_cat() {
  char dest[20] = "Hello";
  char *src = ", World!";
  char numbers[] = "12345678";

  printf("dest before strcat: \"%s\"\n", dest); // "Hello"

  strcat(dest, src);
  printf("dest after strcat:  \"%s\"\n", dest); // "Hello, world!"

  strncat(dest, numbers, 3); // strcat first 3 chars of numbers
  printf("dest after strncat: \"%s\"\n", dest); // "Hello, world!123"
}

//strcmp() = string compare


//strncpy
void strnCpy() {
  char s[10];
  char foo = "My hovercraft is full of eels."; // more than 10 chars

  strncpy(s, foo, 9); // only copy 9 chars into positions 0-8
  s[9] = '\0';        // position 9 gets the terminator
}

