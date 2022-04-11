const int NUMBER_OF_STRINGS = 2;
String results[NUMBER_OF_STRINGS] = {
    "Here your texts",
    "That will show randomly"
};

String getRandomText() {
    long randomNumber = random(NUMBER_OF_STRINGS);
    Serial.print("print " + String(randomNumber) + " message");
    return results[randomNumber];
}
