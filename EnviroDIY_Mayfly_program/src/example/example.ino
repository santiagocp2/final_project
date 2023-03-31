// MultiChannels
//
// rcarduino.blogspot.com
//
// A simple approach for reading and writing eight RC Channels using Arduino Due interrupts
//
// See related posts -
// http://rcarduino.blogspot.co.uk/2012/01/how-to-read-rc-receiver-with.html
// http://rcarduino.blogspot.co.uk/2012/03/need-more-interrupts-to-read-more.html
// http://rcarduino.blogspot.co.uk/2012/01/can-i-control-more-than-x-servos-with.html
//
// rcarduino.blogspot.com
//

//  
//
// rcarduino.blogspot.com
//
// A simple approach for reading three RC Channels using pin change interrupts
//
// See related posts -
// http://rcarduino.blogspot.co.uk/2012/01/how-to-read-rc-receiver-with.html
// http://rcarduino.blogspot.co.uk/2012/03/need-more-interrupts-to-read-more.html
// http://rcarduino.blogspot.co.uk/2012/01/can-i-control-more-than-x-servos-with.html
//
// rcarduino.blogspot.com
//

// include the pinchangeint library - see the links in the related topics section above for details
#include <PinChangeInt.h>
#include <Arduino.h>

// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
#define N 2
#define CH1 4
#define CH3 5

#define GPIO_IN1 6
#define GPIO_IN2 7
#define GPIO_IN3 11
#define GPIO_IN4 10

#define GPIO_ENA A2
#define GPIO_ENB A3

int ch[N];
int side;
int y;

void setup(){
  Serial.begin(57600);
  pinMode(CH1, INPUT);
  pinMode(CH3, INPUT);
  pinMode(GPIO_IN1 , OUTPUT);
  digitalWrite(GPIO_IN1, LOW);
  pinMode(GPIO_IN2 , OUTPUT);
  digitalWrite(GPIO_IN2, LOW);   
  pinMode(GPIO_IN3 , OUTPUT);
  digitalWrite(GPIO_IN3, LOW);
  pinMode(GPIO_IN4 , OUTPUT);
  digitalWrite(GPIO_IN4, LOW);   
}

void loop(){
  ch[0]=pulseIn(CH1, HIGH);
  ch[1]=pulseIn(CH3, HIGH);
  if (ch[1]> 1400 && ch[1] < 1560){
    stop();
  }else if(ch[1]>850 && ch[1]<1920){
    select_side(ch[1]);
    select_vel(ch[1]);

    if (ch[0]>= 850 && ch[0]<= 1400){
      left(y,side);
    }else if(ch[0] > 1400 && ch[0]<= 1560){
      up(y,side);
    }else if(ch[0] > 1560 && ch[0]<= 2150){
      right(y,side);
    }else{
      stop();
    }   
  }
  Serial.print(ch[0]);
  Serial.print(",");
  Serial.println(ch[1]);
  delay(500);
}

void select_side(int ch){
  if(ch >= 850 && ch <= 1400){
    side = 0;
  }else if(ch >= 1560 && ch <= 1920){
    side = 1;
  }
}

void select_vel(int ch){
  if(ch >= 850 && ch <= 1400){
    y = -0.2802*ch + 542.95;
  }else if(ch >= 1560 && ch <= 1920){
    y = 0.3269*ch - 369.17;
  }
  if( y < 150){
    y = 150;
  }else if(y>255){
    y = 255;
  }
}

void left(int16_t vel, int16_t side){
    if (side == 1){
      reverse();
    }else if(side == 0){
      front();
    }
    analogWrite(GPIO_ENA, 0);
    analogWrite(GPIO_ENB, vel); 
}

void right(int16_t vel, int16_t side){
    if (side == 1){
      reverse();
    }else if(side == 0){
      front();
    }
    analogWrite(GPIO_ENA, vel);
    analogWrite(GPIO_ENB, 0); 
}

void up(int16_t vel, int16_t side){
    if (side == 1){
      reverse();
    }else if(side == 0){
      front();
    }
    analogWrite(GPIO_ENB, vel);
    analogWrite(GPIO_ENA, vel);   
}

void front(){
    digitalWrite(GPIO_IN1, LOW);
    digitalWrite(GPIO_IN2, HIGH);
    digitalWrite(GPIO_IN3, HIGH);
    digitalWrite(GPIO_IN4, LOW);
}

void reverse(){
    digitalWrite(GPIO_IN1, HIGH);
    digitalWrite(GPIO_IN2, LOW);
    digitalWrite(GPIO_IN3, LOW);
    digitalWrite(GPIO_IN4, HIGH);  
}

void stop(){
    digitalWrite(GPIO_IN1, LOW);
    digitalWrite(GPIO_IN2, LOW);
    digitalWrite(GPIO_IN3, LOW);
    digitalWrite(GPIO_IN4, LOW);
    analogWrite(GPIO_ENA, 0);
    analogWrite(GPIO_ENB, 0); 
}