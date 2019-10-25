

/*   Hardware

           [    RTC DS3231    ->  ARDUINO UNO        ]
            -----------------------------------------
            SDA pin   -> Analógico 4 (SDA)
            SCL pin   -> Analógico 5 (SCL)
            VCC pin   -> 5V
            GND pin   -> GND
            -----------------------------------------

            [         74HC595   -> DISPLAY          ]
            -----------------------------------------
            pin 15 (Out0) está no PinA do DISPLAY
            pin 1 (Out1) está no PinB do DISPLAY
            pin 2 (Out2) está no PinC do DISPLAY
            pin 3 (Out3) está no PinD do DISPLAY
            pin 4 (Out4) está no PinE do DISPLAY
            pin 5 (Out5) está no PinF do DISPLAY
            pin 6 (Out6) está no PinG do DISPLAY
            pin 7 (Out7) está no PinDP do DISPLAY
            -----------------------------------------

            [        74HC595   ->  ARDUINO UNO       ]
            -----------------------------------------
            Pin 16 -> Vcc
            Pin 10 -> Vcc
            Pin 8  -> GND
            Pin 13 -> GND
            pin 11 -> D8
            pin 12 -> D12 - - -ligar um capacitor eletrolitico de 1 microFaraday (uF) deste pino para o GND
            pin14  -> D13
            ---------------------------------------------

            [          Display  ->  ARDUINO UNO         ]
            ---------------------------------------------
            pin 12 -> D5
            pin 9 -> D6
            pin 8 -> D9
            pin 6 -> D10 
            ---------------------------------------------

            [   OUTROS COMPONENTES   ->    ARDUINO UNO  ]
            ---------------------------------------------
              Botão1 -> pin D2 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão2 -> pin D7 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão3 -> pin D4 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão4 -> pin D3 (interrupção externa) - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Buzzer passivo -> pin D11
              Potenciômetro -> pin A0

  //===========================================================================

  //======================================================================================================
  // --- Bibliotecas Auxiliares ---*/
#include <DS3231.h>          //Inclui a biblioteca do DS3231 Shield

//============================================================================================================================
//TONS PARA PARA O TOQUE


//===========================================================================================================================
//---- Declaração dos pinos no Arduino UNO
#define pinSH_CP  8  //Pino Clock
#define pinST_CP 12  //Pino Latch
#define pinDS    13  //Pino Data
#define qtdeCI   1//quantos CI's estamos utilizando, apenas 1
#define Seg1 5 //pino PWM
#define Seg2 6//pino PWM
#define Seg3 9//pino PWM
#define Seg4 10//pino PWM
#define pinPot 0// Pino analógico do potenciometro
#define botao1 2//botao para mudar hora
#define botao2 7//botao para diminuir as horas
#define botao3 4//botao para aumentar as horas
#define botao4 3//botão para ativar e desativar o despertador
#define buzzer 11//saida pwm para buzzer

//VARIÁVEIS GLOBAIS
int botao_status1 = 1;
int botao_status2 = 1;
int cont1 = 0; //contador responsável em alternar entre os minutos, horas e "nada"
int cont2 = 0; //contador alterado pela interrupção
int minutosDespertar = 0;
int horasDespertar = 0;
//=======================================================================================================
//---------------------------------------------------------
//                    Declaração Das Funções
//------------------------------------------------------------

//pino: número da porta do CI
//estado: valor HIGH ou LOW
void ci74HC595Write(byte pino, bool estado);//funciona igualmente
//a função digitalWrite(variavel,valor) para uma porta que está
//em OUTPUT
void mostrarHoras(int horas, int minutos);
void botaoApertadoHoras();
void botaoApertadoDespertador();
void alternarHora(int horas, int minutos);
void mudarMinutos(int horas, int minutos);
void mudarHoras(int horas, int minutos);
void numero0();
void numero1();
void numero2();
void numero3();
void numero4();
void numero5();
void numero6();
void numero7();
void numero8();
void numero9();
void display1(int pot);
void display2(int pot);
void display3(int pot);
void display4(int pot);
void apagarDisplay();
void despertador();
void mostrarDespertador();
void minutosDespetador();
void horasDespertador();
void toque(int horas, int minutos);

// ======================================================================================================

// --- Declaração de Objetos ---
DS3231  rtc(SDA, SCL);
Time t;
int horas, minutos;

// ======================================================================================================

// --- Configurações Iniciais ---
void setup() {
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  pinMode(pinDS, OUTPUT);
  pinMode(pinPot, INPUT);
  pinMode(Seg1, OUTPUT);
  pinMode(Seg2, OUTPUT);
  pinMode(Seg3, OUTPUT);
  pinMode(Seg4, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);//Inicia comunicações Serial em 115200 baud rate
  rtc.begin();//Inicializa RTC
  
}

// ======================================================================================================

// --- Loop Infinito ---
void loop() {
  //variáveis das horas
  t = rtc.getTime();
  horas = t.hour;
  minutos = t.min;
  Serial.print("Cont2 = ");
  Serial.print(cont2);
  Serial.print("  ");
  Serial.print("Cont1 = ");
  Serial.print(cont1);
  Serial.print("  ");
  botaoApertadoDespertador();
 if(cont1==0&& cont2>=1 && cont2!=3){
  mostrarDespertador();
   
    alternarDespertador();
 }
  if (cont2 == 0 || cont2 == 3) {
    //chamamento da função para mostrar as horas e os minutos onde, visualmente, "tudo" acontece
    mostrarHoras(horas, minutos);
    //sempre verifica se o botao1 foi apertado
    botaoApertadoHoras();
    //sempre verifica o cont1 para ver se começará a mudar as horas e os minutos ou não
    alternarHora(horas, minutos);
    //verifica o cont2, mudado pela interrupção, para começar a mudar as horas do despertador
  }
 
 if((cont2 == 3)&&(minutosDespertar == minutos) && (horasDespertar == horas)) {
  t = rtc.getTime();
  horas = t.hour;
  minutos = t.min;
  Serial.print("   Repetindo  ");
  toque(horas, minutos);
 }
  //Imprime o horário
  Serial.println(rtc.getTimeStr());

}
//end loop
//*********************************************************************************************************************
//====================================================================================================================
//verifica se o botão 1 mudou de estado
void botaoApertadoHoras() {
  int botao = digitalRead(botao1);
  Serial.print(botao);
  if ((botao != botao_status1) && (botao == HIGH)) {
    cont1++;
    Serial.print("Botao apertado\n");
  }
  botao_status1 = botao;

  Serial.print("   ");
}
  void botaoApertadoDespertador(){
     int botao = digitalRead(botao4);
   
    if ((botao != botao_status2) && (botao == HIGH)) {
      cont2++;
      //delay(50);
    }
    botao_status2 = botao;
  }
void alternarDespertador(){
    switch (cont2) {
      case 1:
        minutosDespertador();
        break;
      case 2:
        horasDespertador();
        break;
        case 3://não precia fazer nada
        //é só para que o despertador fique "ligado"
        break;
      default:
        cont2 = 0;
        break;
    }
  }
//Função para mudar a hora quando o botão for apertado
void alternarHora(int horas, int minutos) {
  switch (cont1) {
    case 1:
      mudarMinutos(horas, minutos);
      break;
    case 2:
      mudarHoras(horas, minutos);
      break;
    default:
      cont1 = 0;
      break;
  }
}

void mudarMinutos(int horas, int minutos) {
  int aux = minutos;
  while (cont1 == 1) {
        Serial.print("Cont1 = ");
    Serial.print(cont1);
    Serial.print("  ");
    mostrarHoras(horas, minutos);
    if (digitalRead(botao2) == HIGH) {
      minutos++;
      if (minutos > 59) {
        minutos = 0;
      }
      delay(100);
    } else if (digitalRead(botao3) == HIGH) {
      minutos--;
      if (minutos < 0) {
        minutos = 59;
      }
      delay(100);
    }
    botaoApertadoHoras();
  }
  if (aux != minutos) { //se os minutos forem alterados, então setará no RTC também
    rtc.setTime(horas, minutos, 0);
  }
}

void mudarHoras(int horas, int minutos) {
  int aux = horas;
  while (cont1 == 2) {
        Serial.print("Cont1 = ");
    Serial.print(cont1);
    Serial.print("  ");
    mostrarHoras(horas, minutos);
    if (digitalRead(botao2) == HIGH) {
      horas++;
      if (horas > 23) {
        horas = 0;
      }
      delay(100);
    } else if (digitalRead(botao3) == HIGH) {
      horas--;
      if (horas < 0) {
        horas = 23;
      }
      delay(100);
    }
    botaoApertadoHoras();
  }
  if (aux != horas) { //se as horas forem alteradas também setará no RTC
    rtc.setTime(horas, minutos, 0);
  }
}

void minutosDespertador() {
  //mudar as minutos do despertador
  while (cont2 == 1) {
    Serial.print("Cont2 = ");
    Serial.print(cont2);
    Serial.print("  ");
    if (digitalRead(botao2) == HIGH) {
      minutosDespertar++;
      if (minutosDespertar > 59) {
        minutosDespertar = 0;
      }
      delay(100);
    } else if (digitalRead(botao3) == HIGH) {
      minutosDespertar--;
      if (minutosDespertar < 0) {
        minutosDespertar = 59;
      }
      delay(100);
    } 
    mostrarDespertador();
    botaoApertadoDespertador();
  }
}

void horasDespertador() {
    //mudar as horas do depertador
  while (cont2 == 2) {
    Serial.print("Cont2 = ");
    Serial.print(cont2);
    Serial.print("  ");
    if (digitalRead(botao2) == HIGH) {
      horasDespertar++;
      if (horasDespertar > 23) {
        horasDespertar = 0;
      }
      delay(100);
    } else if (digitalRead(botao3) == HIGH) {
      horasDespertar--;
      if (horasDespertar < 0) {
        horasDespertar = 23;
      }
      delay(100);
    }
    mostrarDespertador();
    botaoApertadoDespertador();
  }
}
//==============================================================================================================================================
void mostrarDespertador() {
  //Lendo valor do potenciômetro e convertendo valor do valor analogico para a saída digital utilizando PWM
  int pot = map(analogRead(pinPot), 0, 1023, 5, 255);
  int horasDezena = horasDespertar / 10; //Pega o resultado inteiro (quociente). Ex.:24/10 = 2
  int horasUnidade = horasDespertar % 10; //Pega o resto enquanto a conta é inteira. Ex.:24%10 = 4
  int minutosDezena = minutosDespertar / 10; //Recebe o primeiro valor dos minutos
  int minutosUnidade = minutosDespertar % 10; //Recebe o segundo valor dos minutos
  //horas dezena
  if (horasDezena == 0) {
    numero0();
  }
  else if (horasDezena == 1) {
    numero1();
  }
  else {
    numero2();
  }
  display1(pot);//passando o valor do potenciometro para controlar a luminosidade
  apagarDisplay();

  //hora unidade
  if (horasUnidade == 0) {
    numero0();
  }
  else if (horasUnidade == 1) {
    numero1();
  }
  else if (horasUnidade == 2) {
    numero2();
  }  else if (horasUnidade == 3) {
    numero3();
  }
  else if (horasUnidade == 4) {
    numero4();
  }
  else if (horasUnidade == 5) {
    numero5();
  }
  else if (horasUnidade == 6) {
    numero6();
  }
  else if (horasUnidade == 7) {
    numero7();
  }
  else if (horasUnidade == 8) {
    numero8();
  }
  else {
    numero9();
  }
  display2(pot);
  apagarDisplay();

  //minutos dezena
  //Só vai até o 59 minutos, então até o 5
  if (minutosDezena == 0) {
    numero0();
  }
  else if (minutosDezena == 1) {
    numero1();
  }
  else if (minutosDezena == 2) {
    numero2();
  }
  else if (minutosDezena == 3) {
    numero3();
  }
  else if (minutosDezena == 4) {
    numero4();
  }
  else {
    numero5();
  }
  display3(pot);
  apagarDisplay();

  //minutos unidade
  //vai até do 0 ao 9
  if (minutosUnidade == 0) {
    numero0();
  }
  else if (minutosUnidade == 1) {
    numero1();
  }
  else if (minutosUnidade == 2) {
    numero2();
  }
  else if (minutosUnidade == 3) {
    numero3();
  }
  else if (minutosUnidade == 4) {
    numero4();
  }
  else if (minutosUnidade == 5) {
    numero5();
  }
  else if (minutosUnidade == 6) {
    numero6();
  }
  else if (minutosUnidade == 7) {
    numero7();
  }
  else if (minutosUnidade == 8) {
    numero8();
  }
  else {
    numero9();
  }
  display4(pot);
  apagarDisplay();
}


void toque(int horas, int minutos) {
 /* 
  Asa branca
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


  // change this to make the song slower or faster
  int tempo = 120;



  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int melody[] = {

    // Asa branca - Luiz Gonzaga
    // Score available at https://musescore.com/user/190926/scores/181370

    NOTE_G4, 8, NOTE_A4, 8, NOTE_B4, 4, NOTE_D5, 4, NOTE_D5, 4, NOTE_B4, 4,
    NOTE_C5, 4, NOTE_C5, 2, NOTE_G4, 8, NOTE_A4, 8,
    NOTE_B4, 4, NOTE_D5, 4, NOTE_D5, 4, NOTE_C5, 4,

    NOTE_B4, 2, REST, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_A4, 8,
    NOTE_B4, 4, NOTE_D5, 4, REST, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8,
    NOTE_G4, 4, NOTE_C5, 4, REST, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8,

    NOTE_A4, 4, NOTE_B4, 4, REST, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8,
    NOTE_G4, 2, REST, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_A4, 8,
    NOTE_B4, 4, NOTE_D5, 4, REST, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8,

    NOTE_G4, 4, NOTE_C5, 4, REST, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8,
    NOTE_A4, 4, NOTE_B4, 4, REST, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8,
    NOTE_G4, 4, NOTE_F5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, 8,

    NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_G4, 8,
    NOTE_G4, 4, NOTE_F5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, 8,
    NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_G4, 8,
    NOTE_G4, -2, REST, 4

  };
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    Serial.println("Tocando");
    mostrarHoras(horas, minutos);
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      mostrarHoras(horas, minutos);
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      mostrarHoras(horas, minutos);
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    mostrarHoras(horas, minutos);
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
mostrarHoras(horas, minutos);
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
    if (cont2 != 3) {
      break;
    }
    mostrarHoras(horas, minutos);
    botaoApertadoDespertador();
  }
}

//FUNÇÃO PARA MOSTRAR AS HORAS NO DISPLAY
void mostrarHoras(int horas, int minutos) {
  //Lendo valor do potenciômetro e convertendo valor do valor analogico para a saída digital utilizando PWM
  int pot = map(analogRead(pinPot), 0, 1023, 5, 255);
  int horasDezena = horas / 10; //Pega o resultado inteiro (quociente). Ex.:24/10 = 2
  int horasUnidade = horas % 10; //Pega o resto enquanto a conta é inteira. Ex.:24%10 = 4
  int minutosDezena = minutos / 10; //Recebe o primeiro valor dos minutos
  int minutosUnidade = minutos % 10; //Recebe o segundo valor dos minutos
  //hora dezena
  if (horasDezena == 0) {
    numero0();
  }
  else if (horasDezena == 1) {
    numero1();
  }
  else {
    numero2();
  } 

  display1(pot);//passando o valor do potenciometro para controlar a luminosidade

  apagarDisplay();

  //hora unidade
  if (horasUnidade == 0) {
    numero0();
  }
  else if (horasUnidade == 1) {
    numero1();
  }
  else if (horasUnidade == 2) {
    numero2();
  }  else if (horasUnidade == 3) {
    numero3();
  }
  else if (horasUnidade == 4) {
    numero4();
  }
  else if (horasUnidade == 5) {
    numero5();
  }
  else if (horasUnidade == 6) {
    numero6();
  }
  else if (horasUnidade == 7) {
    numero7();
  }
  else if (horasUnidade == 8) {
    numero8();
  }
  else {
    numero9();
  }
  display2(pot);
  apagarDisplay();

  //minutos dezena
  //Só vai até o 59 minutos, então até o 5
  if (minutosDezena == 0) {
    numero0();
  }
  else if (minutosDezena == 1) {
    numero1();
  }
  else if (minutosDezena == 2) {
    numero2();
  }
  else if (minutosDezena == 3) {
    numero3();
  }
  else if (minutosDezena == 4) {
    numero4();
  }
  else {
    numero5();
  }
  display3(pot);
  apagarDisplay();

  //minutos unidade
  //vai até do 0 ao 9
  if (minutosUnidade == 0) {
    numero0();
  }
  else if (minutosUnidade == 1) {
    numero1();
  }
  else if (minutosUnidade == 2) {
    numero2();
  }
  else if (minutosUnidade == 3) {
    numero3();
  }
  else if (minutosUnidade == 4) {
    numero4();
  }
  else if (minutosUnidade == 5) {
    numero5();
  }
  else if (minutosUnidade == 6) {
    numero6();
  }
  else if (minutosUnidade == 7) {
    numero7();
  }
  else if (minutosUnidade == 8) {
    numero8();
  }
  else {
    numero9();
  }
  display4(pot);
  apagarDisplay();
}
//===================================================================================================

//FUNÇÕES PARA IMPRIMIR OS NÚMEROS NO DISPLAY
void numero0() {
  //numero 0
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 0); //E ligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 1); //G desligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);
}
void numero1() {
  //numero 1
  ci74HC595Write(0, 1); //A desligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 1); //D desligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 1); //F desligado
  ci74HC595Write(6, 1); //G desligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);
}
void numero2() {
  //numero 2
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 1); //C desligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 0); //E ligado
  ci74HC595Write(5, 1); //F desligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  // delay(1);
}
void numero3() {
  //numero 3
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 1); //F desligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);
}
void numero4() {
  //numero 4
  ci74HC595Write(0, 1); //A desligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 1); //D desligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);
}
void numero5() {
  //numero 5
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 1); //B desligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);
}
void numero6() {
  //numero 6
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 1); //B desligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 0); //E ligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);

}
void numero7() {
  //numero 7
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 1); //D desligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 1); //F desligado
  ci74HC595Write(6, 1); //G desligado
  ci74HC595Write(7, 1); //DP desligado
  // delay(1);

}
void numero8() {
  //numero 8
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 0); //E ligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  //delay(1);

}
void numero9() {
  //numero 9
  ci74HC595Write(0, 0); //A ligado
  ci74HC595Write(1, 0); //B ligado
  ci74HC595Write(2, 0); //C ligado
  ci74HC595Write(3, 0); //D ligado
  ci74HC595Write(4, 1); //E desligado
  ci74HC595Write(5, 0); //F ligado
  ci74HC595Write(6, 0); //G ligado
  ci74HC595Write(7, 1); //DP desligado
  // delay(1);

}

void display1(int pot) {
  //segmento 1 ligado
  analogWrite(Seg1, pot);
  digitalWrite(Seg2, LOW);
  digitalWrite(Seg3, LOW);
  digitalWrite(Seg4, LOW);
  if(cont1==2 || cont2==2){
    ci74HC595Write(7, 0); //DP ligado
  }
  delay(2);
}
void display2(int pot) {
  //segmento 2 ligado
  digitalWrite(Seg1, LOW);
  analogWrite(Seg2, pot);
  digitalWrite(Seg3, LOW);
  digitalWrite(Seg4, LOW);
      if((cont1==0 && cont2==0) || (cont1==0 && cont2==3)){
    ci74HC595Write(7, 0); //DP ligado
  }
  delay(2);
}
void display3(int pot) {
  //segmento 3 ligado
  digitalWrite(Seg1, LOW);
  digitalWrite(Seg2, LOW);
  analogWrite(Seg3, pot);
  digitalWrite(Seg4, LOW);
    if(cont1==1 || cont2==1){
    ci74HC595Write(7, 0); //DP ligado
  }
  delay(3);
}
void display4(int pot) {
  //segmento 4 ligado
  digitalWrite(Seg1, LOW);
  digitalWrite(Seg2, LOW);
  digitalWrite(Seg3, LOW);
  analogWrite(Seg4, pot);
   if(cont2==3){
    ci74HC595Write(7, 0); //DP ligado
  }
  delay(3);
}

void apagarDisplay() {
  digitalWrite(Seg1, LOW);
  digitalWrite(Seg2, LOW);
  digitalWrite(Seg3, LOW);
  digitalWrite(Seg4, LOW);
  // delay(2);
}

//====================================================================================================================

//*********************************************************************************************************************

//início do controle dos pulsos do 74HC595
void ci74HC595Write(byte pino, bool estado) {//É como o analogWrite(Pino, Valor)
  static byte ciBuffer[qtdeCI];
  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);
  for (int nC = qtdeCI - 1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {
      digitalWrite(pinSH_CP, LOW);  //Baixa o Clock
      digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
      digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
      digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento
    }
  }
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}
