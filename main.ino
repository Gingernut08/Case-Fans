#include <math.h>

const int NO_MOTORS = 4;
const int MOTOR_DRIVERS[NO_MOTORS] = {10, 9, 8, 7};

const int NO_COLS = 3;
const int VAR_COL[NO_COLS] = {3, 4, 5};

const int NO_ROWS = 3;
const int VAR_ROW[NO_ROWS] = {A0, A1, A2};

const int SW = 6;

const int TOTAL_VALUES = NO_COLS * NO_ROWS;
int A_VALUES[TOTAL_VALUES];

const int NUM_THEMS = 5;
const int NUM_POTS  = 4;

bool switchPos = false;

float thermScaler = 2;
float potScaler = 1;

void readValues() {
    for (int c = 0; c < NO_COLS; c++) {
        digitalWrite(VAR_COL[c], HIGH);
        delayMicroseconds(5);
        for (int r = 0; r < NO_ROWS; r++) {
            A_VALUES[c * NO_ROWS + r] = analogRead(VAR_ROW[r]);
        }
        digitalWrite(VAR_COL[c], LOW);
    }
    switchPos = (digitalRead(SW) == LOW);
}

int modTherm(int val){
    float scaled = pow((float)val / 1023.0, thermScaler);
    int output = (int)(255.0 * scaled);
    return constrain(output, 0, 255);
}

int modPot(int val){
    float scaled = pow((float)val / 1023.0, potScaler);
    int output = (int)(255.0 * scaled);
    return constrain(output, 0, 255);
}

void setup() {
    for (int i = 0; i < NO_MOTORS; i++) {
        pinMode(MOTOR_DRIVERS[i], OUTPUT);
    }
    for (int i = 0; i < NO_COLS; i++) {
        pinMode(VAR_COL[i], OUTPUT);
        digitalWrite(VAR_COL[i], LOW);
    }
    for (int i = 0; i < NO_ROWS; i++) {
        pinMode(VAR_ROW[i], INPUT);
    }
    pinMode(SW, INPUT_PULLUP);
}

void loop() {
    readValues();
    if (switchPos) {
        for (int i = 0; i < NO_MOTORS; i++) {
            int pwm = A_VALUES[NUM_THEMS + i];
            analogWrite(MOTOR_DRIVERS[i], modPot(pwm));
        }
    } 
    else {
        for (int i = 0; i < NO_MOTORS; i++) {
            int pwm = (A_VALUES[i] + A_VALUES[NUM_THEMS]) / 2;
            analogWrite(MOTOR_DRIVERS[i], modTherm(pwm));
        }
    }
}
