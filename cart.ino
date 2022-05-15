/*
      Arduino         MFRC522       LCD       SIM900(19200bps) NodeMCU(9600bps)

        A0              -            -          -               -
        A1              -            -          -               -
        A2              -            -          -               -
        A3              -            -          -               -
        A4              -            SDA        -               -
        A5              -            SCL        -               -
        D0              -            -          -               -
        D1              -            -          -               -
        D6              -            -          TXD             -
        D7              -            -          RXD             -
        D4              -            -          -               D4 ~GPIO2(WiFi status)
        D5              -            -          -               -
        D2(Rx)          -            -          -               D6(Tx) ~GPIO12
        D3(Tx)          -            -          -               D5(Rx) ~GPIO14
        D8              SDA2         -          -               -
        D9              RST          -          -               -
        D10             SDA1         -          -               -
        D11             MOSI         -          -               -
        D12             MISO         -          -               -
        D13             SCK          -          -               -
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>

#define RST_PIN 9
#define SS_1_PIN 10
#define SS_2_PIN 8
#define NR_OF_READERS 2
#define statusPin 4

char readPh[10];
char readProductInfo[16];
byte readCard[4];
float total = 0.00;
int item_count = 0;
String CustomerName = "";
String CustomerNumber = "";
String UID = "";
String UserMessageBody = "";
String InternalMessageBody = "";
String countrycode = "+91";
String num_1 = "+918861361250";
byte ssPins[] = {SS_1_PIN, SS_2_PIN};
int stat = 0;
char data[100];
String out = "";
long randNumber;
int customerStat = 0;

MFRC522 mfrc522[NR_OF_READERS];
MFRC522::StatusCode status;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial SIM900(6, 7);
SoftwareSerial mcuSerial(2, 3); // RX, TX
void setup()
{
  Serial.begin(9600);
  SIM900.begin(19200);
  mcuSerial.begin(9600);
  // while (!Serial
  pinMode(statusPin, INPUT);

  SPI.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    //Serial.print(F("Reader "));
    //Serial.print(reader);
    //Serial.println(F(" Reading..."));
  }

  lcd.setCursor(0, 0);
  lcd.print("Scan to create");
  lcd.setCursor(0, 1);
  lcd.print("your cart");
}
void loop()

{
  stat = digitalRead(statusPin);

  if (stat == 0)
  {

    lcd.setCursor(0, 0);
    lcd.print("Waiting for WiFi");
    lcd.setCursor(0, 1);
    lcd.print("            ");

    readserial();
    lcd.setCursor(0, 1);
    lcd.print(out);
    // Serial.println(out);
  }
  else
  {

    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
    {
      if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial())
      {
        //Serial.println("Reader: " + reader);
        // Serial.println(reader);
        if (reader == 0 && customerStat >= 1)
        {

          isProduct();
        }
        if (reader == 1)
        {
          isCustomer();
          customerStat = customerStat + 1;
        }
        mfrc522[reader].PICC_HaltA();
        mfrc522[reader].PCD_StopCrypto1();
      }
    }
  }
}

void isProduct()
{
  uint8_t pageAddr = 0x06;
  byte buffer[18];
  byte size = sizeof(buffer);
  byte bufferP[16];
  String pdtName = "";
  String qty = "";
  String price = "";
  float Fqty = 000.00;
  float Fprice = 000.00;
  randNumber = random(1000, 9999);
  status = (MFRC522::StatusCode)mfrc522[0].MIFARE_Read(pageAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522[0].GetStatusCodeName(status));
    return;
  }

  for (byte i = 0; i < 16; i++)
  {
    // Serial.write(buffer[i]);
    readProductInfo[i] = buffer[i];
  }

  for (int k = 0; k < 6; k++)
  {
    qty = qty += readProductInfo[k];
  }

  for (int l = 6; l < 12; l++)
  {
    price = price += readProductInfo[l];
  }
  for (int m = 12; m < 16; m++)
  {
    pdtName = pdtName += readProductInfo[m];
  }
  Fqty = qty.toFloat();
  Fprice = price.toFloat();

  total = total + Fprice;
  item_count = item_count + 1;

  //Serial.println("Product: " + pdtName);
  //Serial.println("qty: " + String(Fqty));
  //Serial.println("price: " + String(Fprice));
 // Serial.println("Total: " + String(total));
  mcuSerial.print("@p ");
  mcuSerial.print(pdtName + " ");
  mcuSerial.print(String(randNumber) + " ");
  mcuSerial.print(String(Fqty) + " ");
  mcuSerial.print(String(Fprice) + " ");
  mcuSerial.print(UID + " ");

  lcd.clear();
  lcd.print("Cart:");
  lcd.setCursor(6, 0);
  lcd.print(item_count);
  lcd.setCursor(0, 1);
  lcd.print("Total:");
  lcd.setCursor(7, 1);
  lcd.print(total);
}

void isCustomer()
{
  byte block = 4;
  byte len = 18;
  byte buffer1[18];
  String ph = "";
  String Cname = "";
  String val = "";

  Serial.println("Customer");

  for (int i = 0; i < mfrc522[1].uid.size; i++)
  {
    readCard[i] = mfrc522[1].uid.uidByte[i];

    for (int j = 0; j < 1; j++)
    {
      val = val += readCard[i];
    }
  }
  String _uid = String(val.toInt(), HEX);
  UID = _uid;
  //Serial.println("Card UID:" + _uid);

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  status = mfrc522[1].PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522[1].uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522[1].GetStatusCodeName(status));
    return;
  }

  status = mfrc522[1].MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522[1].GetStatusCodeName(status));
    return;
  }

  for (uint8_t k = 0; k < 16; k++)

  {
    if (buffer1[k] != 32)
    {
      readPh[k] = buffer1[k];
    }
  }
  for (int i = 1; i < 11; i++)
  {
    ph = ph += readPh[i];
  }

  byte buffer2[18];
  block = 1;

  status = mfrc522[1].PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522[1].uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522[1].GetStatusCodeName(status));
    return;
  }

  status = mfrc522[1].MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522[1].GetStatusCodeName(status));
    return;
  }

  for (uint8_t i = 0; i < 16; i++)
  {

    for (int j = 0; j < 1; j++)
    {
      Cname = Cname += String((char)buffer2[i]);
    }
  }
  CustomerName = Cname;
  CustomerName.trim();
  ph.trim();
  CustomerNumber = countrycode += ph;

  if (customerStat == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hi " + CustomerName);
    lcd.setCursor(0, 1);
    lcd.print("Scan Products");
    mcuSerial.print("$ ");
    mcuSerial.print(UID + " ");
    mcuSerial.print(CustomerName + " ");
    mcuSerial.print(CustomerNumber + " ");
  }
  if (customerStat >= 1)
  {
    //Serial.println("name: " + Cname);
    //Serial.println("Ph: " + ph);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(CustomerName);
    lcd.setCursor(0, 1);
    lcd.print(ph);
    Send_msg();
  }

  total = 00.00;
  item_count = 0;
}

void Send_msg()
{
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending message");
  lcd.setCursor(0, 1);
  lcd.print("to: " + CustomerName);

  UserMessageBody = "Hi " + CustomerName + " \nYour order is Completed \nCart total is: Rs " + total + " \n" + item_count + " item(s) \nThank you for shopping with Freshyy";
  InternalMessageBody = "" + UID + " " + CustomerName + " " + CustomerNumber + " " + total + " " + item_count + "";

  sendSMSUSER(UserMessageBody, CustomerNumber);
  delay(1000);
  mcuSerial.print("% ");
  mcuSerial.print(UID + " ");
  mcuSerial.print(total);
  sendSMSINTERNAL(InternalMessageBody, num_1);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message Send");
  lcd.setCursor(0, 1);
  lcd.print("successfully");

  CustomerName = "";
  CustomerNumber = "";
  UserMessageBody = "";
  InternalMessageBody = "";
  UID = "";
  delay(2000);
  mcuSerial.print("!");
  reboot();
}
void reboot()
{
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1)
  {
  }
}

void sendSMSUSER(String userMessage, String number)
{
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);
  SIM900.println("AT+CMGS=\"" + number + "\"");
  delay(100);
  SIM900.println(userMessage);
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26);
  delay(100);
  SIM900.println();
  delay(5000);
}

void sendSMSINTERNAL(String internalMessage, String internalnumber)
{

  SIM900.print("AT+CMGF=1\r");
  delay(100);
  SIM900.println("AT+CMGS=\"" + internalnumber + "\"");
  delay(100);
  SIM900.println(internalMessage);
  delay(100);
  SIM900.println((char)26);
  delay(100);
  SIM900.println();
  delay(5000);
}

void readserial()
{
  short pos = -1;
  while (mcuSerial.available() && pos < 100)
    data[++pos] = mcuSerial.read();
  out = data;
  out.trim();
  // data[0] = (char)0;
  // memset(data, (char)0, sizeof(data));
}