#include <iom8.h>
#include <ina90.h>

#define bit(n) (1 << n)
#define setBit(port, n) (port |= bit(n))
#define clearBit(port, n) (port &= ~bit(n))
int pow(int a, int b){
  int p;
  int value = 1;
  for (p = 0; p<a; p++){
    value *= b;
  }
  return value;
}

unsigned int k = 0;
#pragma vector = TIMER0_OVF_vect
__interrupt void f() {
    TCNT0 = 0x82;
    k++;
}

unsigned long s = 0;
unsigned int ibit[10]={
0x14,//0
0xBE,//1
0x4C,//2
0x8C,//3
0x27,//4
0x85,//5
0x84,//6
0x9E,//7
0x04,//8
0x0C//9
};
unsigned int dig[4] = {0, 0, 0, 0};
int j;
int l;
int count = 0;
int x = 0;
unsigned char i=0;
unsigned char num;
#pragma vector=ADC_vect
__interrupt void AdcInterrupt(){
  if (x<1024) {
    s+=ADC;
    x++;
  }
  else {
    s = s>>10;
    //s /= 10;
    for (l = 0; l<4; l++){
      dig[3-l] = s%10;
      s /= 10;
    }
    x = 0;
    s = 0;
  }
}
void main() {
    _CLI();
    DDRD = 0xFF;
    DDRC = 0x0F;
    PORTD = 0xFF;
    PORTC = 0x00;
    TCCR0 = 0x03;
    TCNT0 = 0x82; 
    setBit(TIMSK, 0);
    ADMUX |= 0xC4; 
    ADCSR |= 0xBF;
    _SEI();
    ADCSR |= 0x40;
    while (1) {
      if (k==5){
        clearBit(PORTC, i);
        if (i==4){
          i = 0;
        }
        else{
          i++;
        }
        num = ibit[dig[i]];
        for (j = 0; j <= 7;j++){
          if (num%2) {
            clearBit(PORTD, 7-j);
          }
          else{
            setBit(PORTD, 7-j);
          }
          num >>= 1;
        }
        if (i==2){
          setBit(PORTD, 5);
        }
        if ((i==0) & (dig[i]==0)){
        
        }
        else{
          setBit(PORTC, i);
        }
        k = 0;
        
      }
    }
}
