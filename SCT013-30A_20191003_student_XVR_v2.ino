// Program to measure consumed current by a load with the SCT013-30A current sensor

/***** Include constants for the Pin where the Sensor is connected to *******/ 
/*****____________WRITE HERE THE CODE_________________*******/
const int SensorPin = A0, RefPin = A2;
const int Rshunt = 33;

// Definition of auxiliary variables 
unsigned long time_ant = 0, difTime = 0, act_time = 0;
const int sampleDuration = 20;
int count_integral = 0;
double rawSquaredSum = 0;
double Iant = 0;

unsigned long time_now = 0;
unsigned long time2_ant = 0;
unsigned long count = 0;
float sum1 = 0;
char comando;


/***** Include constants for the grid frecuency and current transformer properties *******/
/*****____________WRITE HERE THE CODE_________________*******/
// Definition of constant grid frequency (50 Hz)
double freq = 50;
// Definition of transformer reduction relationship
double n_trafo = 1000;

// Definition of measured current variable 
double Irms = 0;

/***** Write the void setup code command. Inside it, Set the data rate in bits per second for serial data transmission, 
so as to communicate with the computer. Use of the availables reates *******/
/*****____________WRITE HERE THE CODE_________________*******/
void setup() {
  // Initialize the serial port communications with a baudrate of 115200 bit/s
  Serial.begin(115200);          
}

// Loop code
void loop()
{
  act_time = micros();
  difTime = act_time - time_ant;

  int RawValue = 0;

  if (difTime >= 1000) {
    time_ant = act_time + (difTime - 1000);

    /***** Read the ADC input from the sensor and the voltage reference point *******/
    /*****____________WRITE HERE THE CODE_________________*******/
    double ADC_sensor = analogRead(SensorPin);
    double ADC_ref = analogRead(RefPin);

    
    /*****  Translate the ADC input measured to voltage values *******/
    /*****____________WRITE HERE THE CODE_________________*******/
    double V_sens = ADC_sensor/1023*5.0;
    double V_ref = ADC_ref/1023*5.0;

    // Calculate the instantaneous current using the voltage diference and the burder resistor value
    double Iinst =  n_trafo * (V_sens - V_ref) / Rshunt;
      
    // Print the instantaneous current
    //Serial.println(Iinst);
    
    // Calculate the integral
    rawSquaredSum += Iinst * Iinst * 0.001;
    // Count 20 ms
    count_integral++;
  }

    // Each 20 ms, calculte the RMS
  if (count_integral >= sampleDuration) 
  {
    // Calculate the RMS
    Irms = sqrt(freq * rawSquaredSum)-0.0906740976645437; 
    //Serial.println(Irms);
    
    // Counter and integral reset
    count_integral = 0;
    rawSquaredSum = 0;

    // Low-pass filter
    //double Ifilt = 0.95 * Iant + 0.05 * Irms;
    //Iant = Ifilt;
    // Increment the time counter
    count++;
    // Acumulate the ADC measurements each second, to calculate latter and average value each 5 seconds
    sum1 += Irms;

        // Print the RMS
    //Serial.print("Ifilt = ");
    //Serial.print(Ifilt);
  }
  
   // Check the current time in miliseconds
    time_now = millis();
    // Each 1 second, measure the A0 and A1 ports
    // Each 5 seconds, calculate the average value of the A0 and A1 measurments, and the state of the relay output pin
    // and write the values with the serial port using semicolons to separte them
    if (time_now - time2_ant > 5000) 
    {
      double Iaver=sum1/count;
      double Paver=Iaver*230;

      Serial.print(Iaver);
      Serial.print(";");
      Serial.print(Paver);
      Serial.println(";");
      
      // Reset the variables to calculate the avarage results
      sum1 = 0;
      // Reset the time counter and update the "5 second" time flag
      count = 0;
      time2_ant = time_now;
    }
}
