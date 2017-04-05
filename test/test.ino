#include "Wire.h"       // allows communication to i2c devices connected to arduino
#include "I2Cdev.h"     // I2Connection library (communication to serial port)
#include "MPU6050.h"    // IMU library
#include "Servo.h"      // servo control library
 
MPU6050 mpu; //defines the chip as a MPU so it can be called in the future
 
int16_t ax, ay, az;  // x y z orientation values from accelerometer
int16_t gx, gy, gz;  // x y z orientation values from gyrscope
/////////////////////////////////////////////////////////////////////////////////////// 
Servo outer; 
Servo inner;
///////////////////////////////////////////////////////////////////////////////////////
int valo;     // outer val
int prevValo; // outer prev val
///////////////////////////////////////////////////////////////////////////////////////
int vali;  //inner val
int prevVali; //outer prev val
/////////////////////////////////////////////////////////////////////////////////////// 
float interrx=0, interry=0, delerrx=0, delerry=0, prevx=0, prevy=0, x=0, y=0;
//initializes the IMU
void setup() 
{
    Wire.begin(); 
    Serial.begin(9600); 
 
    Serial.println("Initialize MPU");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "Connected" : "Connection failed"); 
    outer.attach(9); //servo on pin 9 for large ring y
    inner.attach(10);//servo on pin 10 for small ring x
}
/////////////////////////////////////////////////////////////////////////////////////// 
void loop() 
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int corr[3];
    corr[0]=0; corr[1]=0; corr[2]=15000;
    int axsp=0;
    int aysp=0;
    //int azsp=0;
    ax=ax/1000*1000-corr[0];
    ay=ay/1000*1000-corr[1];
    //az=az/1000*1000-corr[2];

    int errx=ax-axsp;
    int erry= ay-aysp;
    //int errz= az-azsp;
    //Serial.print(", ") ;
    //Serial.println(errz); //Don't need  component

    //Outer=x, Inner=y

    int Kpx=1, Kix=0, Kdx=1;
    int Kpy=1, Kiy=0, Kdy=1;

    interrx+=errx;
    interry+=erry;

    delerrx=prevx-ax;//prevx-ax
    delerry=prevy-ay;
    
    x= Kpx*errx +Kdx*delerrx + Kix*interrx;
    y= Kpy*erry +Kdy*delerry + Kiy*interry;

    prevx=ax;
    prevy=ay;
 
    valo = map(x, -17000, 17000, 179, 0);
    if (valo != prevValo)
    {
        outer.write(valo); 
        prevValo = valo; 
    }
    
     vali = map(y, -17000, 17000, 179, 0);
    if (vali != prevVali)
    {
        inner.write(vali); 
        prevVali = vali; 
    }

    /*Serial.print(errx);
    Serial.print(", ");
    Serial.print(erry);
    Serial.print(", ");*/
    Serial.print(vali);
    Serial.print(", ");
    Serial.println(valo); 

}
///////////////////////////////////////////////////////////////////////////////////////

