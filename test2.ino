#define left_motor 10
#define right_motor 9
#define IR1 19
#define IR2 18
#define IR3 17

int BLED = 3;  //define LED
int GLED = 2;
int RLED = 4;

int Linitial;  //initial value of differences read by sensor
int Rinitial;
int Lreal;  //real value of differences read by sensor when the car moves
int Rreal;
int thresh = 200; //threshold value before a car starts to adjust itself

int speedOffset = -22; //speed difference between left and right wheers to go straight
int rmaxSpeed = 150;
int lmaxSpeed = 150+speedOffset;
int turnFactor = 0.1;

long aveIR1;  //average value of three IRs when initializing
long aveIR2;
long aveIR3;
int sample = 150;

long left, middle, right;

void setup() {
    Serial.begin(9600);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(IR1, INPUT);
    pinMode(IR2, INPUT);
    pinMode(IR3, INPUT);
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    for (int k = 0; k < sample; k++){
        aveIR1 += analogRead(IR1);
        aveIR2 += analogRead(IR2);
        aveIR3 += analogRead(IR3);
    }
    aveIR1 /= sample;
    aveIR2 /= sample;
    aveIR3 /= sample;

    Linitial = aveIR1 - aveIR2;  //get initial value diffeence between three IRs
    Rinitial = aveIR3 - aveIR2;
}






void goStraight()
{
    analogWrite(left_motor, lmaxSpeed);   // offset 22                     
    analogWrite(right_motor, rmaxSpeed); 
    digitalWrite(GLED, HIGH); 
}

void turnLeft()
{
    analogWrite(left_motor, lmaxSpeed*turnFactor);
    analogWrite(right_motor, rmaxSpeed);
    digitalWrite(BLED, HIGH); 
    delay(100);
}

void turnRight()
{
    analogWrite(left_motor, lmaxSpeed);
    analogWrite(right_motor, rmaxSpeed*turnFactor);
    digitalWrite(RLED, HIGH); 
    delay(100);
}

void stop()
{
    digitalWrite(left_motor, LOW);                     
    digitalWrite(right_motor, LOW); 
    digitalWrite(RLED, HIGH);
    digitalWrite(BLED, HIGH);
}

// the loop function runs over and over again forever
void loop() {
    // analogWrite(left_motor, 128);   // offset 22
    // analogWrite(right_motor, 150);   
    left = analogRead(IR1);
    middle = analogRead(IR2);
    right = analogRead(IR3);

    Lreal = left - middle;
    Rreal = right - middle;

    // int Loffset = abs(Linitial) - abs(Lreal);
    // int Roffset = abs(Rinitial) - abs(Rreal);


    // if((Loffset < Lthresh) && (Roffset > Rthresh)) //if the car deviates to the left
    // {
    //     {
    //         analogWrite(left_motor, 150);
    //         analogWrite(right_motor, 0);
    //         digitalWrite(RLED, HIGH); 
    //         delay(100);
    //     }
    // }
    // else if((Loffset > Lthresh) && (Roffset < Rthresh)) //if the car deviates to the right
    // {
    // while(Roffset > Rthresh)
    // {
    // analogWrite(left_motor, 0);
    // analogWrite(right_motor, 150);
    // digitalWrite(BLED, HIGH); 
    // delay(100);
    // }
    // }
    // else if((Loffset <= Lthresh) && (Roffset <= Rthresh))//not sure
    // {
    // stopCar();
    // }
    // else 
    // {
    // goStraight();
    // }

    if(Lreal < thresh && Rreal > thresh){
        turnRight();
    }
    else if (Lreal > thresh && Rreal < thresh){
        turnLeft();
    }
    else if (Lreal > thresh && Rreal > thresh){
        goStraight();
    }
    else{ //both above thresh
        stop();
    }

    Serial.print(left);
    Serial.print(' ');
    Serial.print(middle);
    Serial.print(' ');
    Serial.print(right);
    Serial.print(' ');

    Serial.println();
    //delay(100);
}
