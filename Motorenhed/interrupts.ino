void bMenu() {
  if (knapCheck == 0) {
    menuCount--;
    Serial.println("B");
    Serial.println(menuCount);
    knapCheck = 1;
  }
}
void fMenu() {
  if (knapCheck == 0) {
    menuCount++;
    Serial.println("F");
    Serial.println(menuCount);
    knapCheck = 1;
  }
}
