// Global Variables
int DHT11_Pin = 14; // DHT11 Data Pin

int Humidity = 0; 
int Temp = 0;
int TempComma = 0;
bool DHTError = false; // Checksum Error

// a Delay routine. Call DelayTimer(time in uSec)

void DelayTimer(long int DelayValue){
  long int DelayTime = micros();
  do {
    
  }while (micros()-DelayTime < DelayValue);
  }


// Main DHT Void

void DHT11(){

long int DataTime = 0;

byte Result[45];
byte DataArray = 0;
byte DataCounter = 0;
byte DHTData[4];

bool BlockDHT=false;
 
// Trigger Sensor (described in the Datasheet)

 pinMode(DHT11_Pin,OUTPUT); 
 digitalWrite(DHT11_Pin,HIGH);
 DelayTimer(250000); //Wait 250millisec
 digitalWrite(DHT11_Pin,LOW);
 DelayTimer(30000);   //Wait 30millisec
 digitalWrite(DHT11_Pin,HIGH);
 DelayTimer(50); //Wait 50microsec
 pinMode(DHT11_Pin,INPUT); 


// read the Bits and put them into a Result array (It will count 42 bits. The first two one are useless due my code)

do {
  if (digitalRead(DHT11_Pin) == 0 && BlockDHT == false) {BlockDHT = true;DataArray++;DataTime=micros();} //If DHT pin is low, go to next Dataset
  if (digitalRead(DHT11_Pin) == 1) {BlockDHT = false;Result[DataArray]=(micros()-DataTime);} // As long as DHT pin is Hight add time in Microseconds to Result
  

}while((micros()-DataTime) < 150); // if DTH Sensor high for more than 150 usec, leave loop

// Asign 1 or 0 to Result variable. If more than 80uS Data as "1"
// Starting at Data set 02. First two Datasets are ignored!

for (int  i=2; i< DataArray; i++) {
  if (Result[i] <= 90) Result[i]=0; else Result[i]=1;
                                  }


for (int  j=0; j< 5; j++){     // redo it for the 5 Bytes (40 Databits /8 = 5)
for (int  i=0; i< 8; i++) {bitWrite(DHTData[j], 7-i, Result[i+2+(j*8)]);}  // Create 5 Databytes from the 40 Databits (Ignoring the 2 first Databits)

}
// check checksum                            }

if (DHTData[4] == (DHTData[0]+DHTData[1]+DHTData[2]+DHTData[3])){Humidity = DHTData[0];Temp = DHTData[2];TempComma = DHTData[3];DHTError=false;} else DHTError=true; //If Checksum is worng, Temp=99 (Dataset 0-3 in addition = Dataset 4 = Checksum OK)

}

void setup() {
 Serial.begin(9600);
}

void loop() {
 DHT11();
  if (DHTError == false){Serial.print("Humidity = ");Serial.print(Humidity);Serial.print("% ");Serial.print(" Temp = ");Serial.print(Temp);Serial.print(",");Serial.print(TempComma);Serial.println("°C ");}
  DelayTimer(2500000); //wait 2,5 sec

}
