void bmp_print() {
    Serial.print("B_T: ");
    Serial.print(bme.readTemperature());
    Serial.print(" C; ");
    
    Serial.print("B_P: ");
    Serial.print(bme.readPressure());
    Serial.print(" Pa; ");

    Serial.print("B_A: ");
    Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.print(" m; ");
}
