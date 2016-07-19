#include <stdio.h>
#include <stdlib.h>
 
 
int main() {
 
    // Initiating
    // Creating file variables
    FILE*fp0;
    FILE*fp1;
    FILE*fp2;
    FILE*fp3;
    FILE*scale;
    FILE*txt =fopen("/home/udooer/Desktop/log.txt","w+");
 
 
    // Buffer values
    char value0[12];
    char value1[12];
    char value2[12];
    char value3[12];
    char Scale[12];
     
     
    // Voltage Values
    float scale_v;
    float v0;
    float v1;
    float v2;
    float v3;
     
    // Kalman Values 
    // Set values
    float A = 1;
    float H = 1;
    float Q = 0.001;
    float R = 0.1;
    float X= 3;
    float P = 1;
    float I = 1;
    // Intermediaries 
    float X_pre;
    float P_pre;
    float y;
    float S;
    float K;
 
    // Counter variable 
    int counter = 0;
     
    //Infinite Loop
    for( ; ; ){
        ////////////////////////////////////////////////////////////////
        //Retrieving Values
        ////////////////////////////////////////////////////////////////
         
        //////////////////////
        //////Scale Value/////
        //////////////////////
        scale = fopen("/sys/bus/iio/devices/iio:device0/in_voltage_scale","r");
        fscanf(scale, "%s*", Scale);
        //printf("value is  Scale: %s\n" , Scale);
        scale_v= (atof(Scale));
        //printf("values of scale_v: %f\n", sclae_v);
 
        //////////////////////
        ////Voltage Values////
        //////////////////////
 
        //Reading voltage 0 values
        fp0 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw","r");
        fscanf(fp0, "%s", value0);
        //printf("value is  : %s\n" , value0);
 
        //Convert voltage 0 and multiplying by scale value
        v0 = (atof(value0));
        v0 = v0*scale_v;
        //printf("value is  v0: %f\t\t\t" , v0);
 
        //Reading voltage 1 values
        fp1 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw","r");
        fscanf(fp1, "%s", value1);
        //printf("value is  : %s\n" , value1);
 
        //Convert voltage 1 and multiplying by scale value
        v1 = (atof(value1));
        v1 = v1*scale_v;
        //printf("value is  v1: %f\t\t\t" , v1);
 
        //Reading voltage 2 values
        fp2 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage2_raw","r");
        fscanf(fp2, "%s", value2);
        //printf("value is  value2: %s\n" , value2);
 
        //Convert voltage 2 and multiplying by the scale value
        v2 = (atof(value2));
        v2 = v2*scale_v;
        //printf("value is  v2: %f\t\t\t" , v2);
         
        //Reading voltage 3 values
        fp3 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage3_raw","r");
        fscanf(fp3, "%s", value3);
        //printf("value is  : %s\n" , value3);
 
        //Convert voltage 3 and multiplying by scale value
        v3 = (atof(value3));
        v3 = v3*scale_v;
        //printf("value is  v3: %f\t\t\t" , v3);
 
        //Printing the counter value
        printf("counter: %i\n", counter);
 
        ////////////////////////////////////////////////////////////////
        //CALCULATIONS
        ////////////////////////////////////////////////////////////////
 
        //Difference between values
        //Reference Voltage
        float vref = v0-v1;
        printf("value of Reference:%f\t", vref);
         
        //Output Voltage
        float vout = v3-v2;
        printf("value of Output:%f\n" , vout);
         
        // FILTER 
        //Calculations
        X_pre = A*X;
        P_pre = P + Q;
        y = vout - H*X_pre;
        S = P_pre+R;
        K = P_pre*H*(1/S);
        X = X_pre+K*y;
        P = (I-K*H)*P_pre;
         
        //Balacanced value
        float tf = X/vref;
        printf("value of balance :%f\n", tf);
         
        //COUNTER
        counter++;
         
         
        //Writing to a log file
        fprintf(txt, "Counter:%i\t\t Balanced Value: %f\n Filter Value:%f\t\t Output Value:%f \n\n" ,counter,tf,X,vout);
         
         
        //DELAY: It takes 2 samples per second
        int i; 
        for (i=0; i<1000000  ; i++){
            }
             
        //Closing files
        fclose(fp0);    
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(scale);
        // Just to make things neater
        printf("\n");       
    }
    fclose(txt);
    return 0;
}
