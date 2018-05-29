/*-------------------------------------------------
  I2C経由で状態表示用LEDシステムを制御するプログラム
  by RockSolidHead
*-------------------------------------------------*/
#include  <Wire.h>
#include  "LuminousCommon.h"

byte  i2cBuffer[33];

void setup() {
  Serial.begin(19200);
  Wire.begin(); // I2c Start,Master
  Serial.println("Luminous Test program.");
  Serial.print("Waiting");
  while( !IsWakeUpLUMI() ) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Luminous Wake UP!");
  randomSeed(100);
}
#define WAIT  600
void loop() {
  int i,j;
  int c1,c2;

  Serial.println("---- BLINK TEST ----");

  Serial.println("フルカラーLED 各LEDを個別に設定");
  for( i=0 ; i<LED_FULL_COLOR_NUM ; i++ ) {
    c1=random(0,14);
    do { 
      c2=random(0,14);
      if( c2!=c1 )  break; 
    }while(1);
    setFCLmode((byte)(0xff & i ), LED_BLINK|LED_SPEED_LEVEL3, (byte)(0xff & c1),(byte)(0xff & c2) );
  }
  delay(5000);
  
  Serial.println("フルカラーLED 全てのLEDを一括でサクラ色で点滅");
  setAllFCLmode(LED_BLINK| LED_SPEED_LEVEL5,LED_SAKURA,LED_BLACK);
  delay(5000);

  Serial.println("フルカラーLED 全体の明るさを制御");
  setAllFCLmode(LED_ON,LED_WHITE,LED_BLACK);
  Serial.println(" 明るく");
  for( i=0 ; i< 5 ; i++ ) {
    setFCLbright( 1 );
    delay(100);
  }
  Serial.println(" 暗く");
  for( i=0 ; i< 5 ; i++ ) {
    setFCLbright( 2 );
    delay(100);
  }

  setAllFCLmode(LED_OFF,LED_BLACK,LED_BLACK);
  Serial.println("モノカラーLEDを一括で同じ点滅設定");
  i2cBuffer[0]=LUM_MCL_ALL_SET;
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    i2cBuffer[1+i]= LED_BLINK|LED_SPEED_LEVEL6;
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("モノカラーLED 長く消灯１回点灯");
  i2cBuffer[0]=LUM_MCL_ALL_SET;
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    i2cBuffer[1+i]= LED_BLINK_OFFON|LED_SPEED_LEVEL6;
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("モノカラーLED 長く消灯２回点灯");
  i2cBuffer[0]=LUM_MCL_ALL_SET;
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    i2cBuffer[1+i]= LED_BLINK_OFFONON|LED_SPEED_LEVEL6;
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("モノカラーLED　だんだん明るくだんだん暗く");
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    if( i<2 ) {//ON/OFFしかできなLEDは点滅設定
      i2cBuffer[1+i]= LED_OFF;
    } else {
      i2cBuffer[1+i]= LED_BLINK_STUD|LED_SPEED_LEVEL13;
    }
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("モノカラーLED　急に明る急に暗く");
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    if( i<2 ) {//ON/OFFしかできなLEDは点滅設定
      i2cBuffer[1+i]= LED_OFF;
    } else {
      i2cBuffer[1+i]= LED_BLINK_ST2UD|LED_SPEED_LEVEL13;
    }
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("モノカラーLED　だんだん明るく消灯");
  for( i=0; i<LED_MONO_COLOR_NUM ; i++ ) {
    if( i<2 ) {//ON/OFFしかできなLEDは点滅設定
      i2cBuffer[1+i]= LED_OFF;
    } else {
      i2cBuffer[1+i]= LED_BLINK_STU|LED_SPEED_LEVEL13;
    }
  }
  sendMCLmodeAll();
  delay(10000);

  Serial.println("フルカラーLED 任意のRGBカラーを設定");
  Serial.println("だんだん明るくだんだん暗く");
  for( j=0;j<LED_FULL_COLOR_NUM;j++ ) {
    setFCLmode((byte)(0xff & j ), LED_BLINK_STUD|LED_SPEED_LEVEL13, LED_BLACK,LED_BLACK );   
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),FRONT );
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),BACK );
  }
  delay(5000);
  Serial.println("急に明る急に暗く");
  for( j=0;j<LED_FULL_COLOR_NUM;j++ ) {
    setFCLmode((byte)(0xff & j ), LED_BLINK_ST2UD|LED_SPEED_LEVEL13, LED_BLACK,LED_BLACK );   
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),FRONT );
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),BACK );
  }
  delay(5000);
  Serial.println("だんだん明るく消灯");
  for( j=0;j<LED_FULL_COLOR_NUM;j++ ) {
    setFCLmode((byte)(0xff & j ), LED_BLINK_STU|LED_SPEED_LEVEL13, LED_BLACK,LED_BLACK );   
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),FRONT );
    setFCLcolor( j , random(0,256), random(0,256), random(0,256),BACK );
  }
  delay(5000);


  Serial.println("Luminous を再起動");
  resetLuminous();
  Serial.print("再起動待ち");
  while( !IsWakeUpLUMI() ) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Luminous 起動確認");  

}
// Luminous 起動確認関数
boolean  IsWakeUpLUMI() {
  boolean flag;
  unsigned long cnt;

  cnt=0;
  flag=false;
  Wire.requestFrom(LUM_ADDRESS,1);
  while(Wire.available())  {
    byte c = Wire.read();
    if( c== 255 ) {
      flag=true;
      break;
    }
    cnt++;
    if( cnt==0 ) { // cnt is overflow
      flag=false;
      break;
    }
  }  
  return( flag );
}
//フルカラーLED １つに設定
void  setFCLmode(  int no , int ledMode , int color1 , int color2 ) {
  Wire.beginTransmission(LUM_ADDRESS);
  Wire.write(LUM_FCL_SET) ;
  Wire.write((byte)(0xff & no )) ;
  Wire.write((byte)(0xff & ledMode)) ;
  Wire.write((byte)(0xff & color1)) ;
  Wire.write((byte)(0xff & color2)) ;
  Wire.endTransmission();    delay(WAIT);
}
//フルカラーLED 全てに同一の設定
void  setAllFCLmode( int ledMode , int color1 , int color2 ) {
  Wire.beginTransmission(LUM_ADDRESS);
  Wire.write(LUM_FCL_ALL_SET) ;
  Wire.write((byte)(0xff & ledMode)) ;
  Wire.write((byte)(0xff & color1)) ;
  Wire.write((byte)(0xff & color2)) ;
  Wire.endTransmission();    delay(WAIT);
}
//フルカラーLED 全ての明るさを設定
void  setFCLbright(int updw ) {
  if( updw==1 || updw==2 ) {
    Wire.beginTransmission(LUM_ADDRESS);
    Wire.write(LUM_FCL_BRIGHT) ;
    Wire.write((byte)(0xff & updw)) ;
    Wire.endTransmission();    delay(WAIT);
  }
}
//
void  setFCLcolor( int no , int r , int g,int b , int fb ) {
    Wire.beginTransmission(LUM_ADDRESS);
    Wire.write(LUM_FCE_COLOR_SET) ;
    Wire.write((byte)(0xff & no )) ;
    Wire.write((byte)(0xff & r)) ;
    Wire.write((byte)(0xff & g)) ;
    Wire.write((byte)(0xff & b)) ;
    Wire.write((byte)(0xff & fb)) ;
    Wire.endTransmission();    delay(WAIT);
}

//モノカラーLED 全てを同一設定
void  sendMCLmodeAll() {
  Wire.beginTransmission(LUM_ADDRESS);
  for(int i=0;i<(LED_MONO_COLOR_NUM+1);i++ ) {
    Wire.write(i2cBuffer[i]) ;
  }
  Wire.endTransmission();    delay(WAIT);
}
//モノカラーLED １つを設定
void  setMCLmode( int no , int ledMode) {
  Wire.beginTransmission(LUM_ADDRESS);
  Wire.write(LUM_MCL_SET);
  Wire.write((byte)(0xff & no ));
  Wire.write((byte)(0xff & ledMode));
  Wire.endTransmission();    delay(WAIT);

}
//Luminousを再起動
void  resetLuminous() {
  Wire.beginTransmission(LUM_ADDRESS);
  Wire.write(LUM_INIT);
  Wire.endTransmission();  
}

