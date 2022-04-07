# Relogio de Atmega328p
->  Ligações de hardware

#### [    RTC DS3231    ->  ARDUINO UNO        ]
 -----------------------------------------
            SDA pin   -> Analógico 4 (SDA)
            SCL pin   -> Analógico 5 (SCL)
            VCC pin   -> 5V
            GND pin   -> GND
         
#### [         74HC595   -> DISPLAY          ]
 -----------------------------------------
            pin 15 (Out0) está no PinA do DISPLAY
            pin 1 (Out1) está no PinB do DISPLAY
            pin 2 (Out2) está no PinC do DISPLAY
            pin 3 (Out3) está no PinD do DISPLAY
            pin 4 (Out4) está no PinE do DISPLAY
            pin 5 (Out5) está no PinF do DISPLAY
            pin 6 (Out6) está no PinG do DISPLAY
            pin 7 (Out7) está no PinDP do DISPLAY
         
#### [        74HC595   ->  ARDUINO UNO       ]
-----------------------------------------
            Pin 16 -> Vcc
            Pin 10 -> Vcc
            Pin 8  -> GND
            Pin 13 -> GND
            pin 11 -> D8
            pin 12 -> D12 - - -ligar um capacitor eletrolitico de 1 microFaraday (uF) deste pino para o GND
            pin14  -> D13
        
#### [          Display  ->  ARDUINO UNO         ]
 ---------------------------------------------
            pin 12 -> D5
            pin 9 -> D6
            pin 8 -> D9
            pin 6 -> D10 
      
#### [   OUTROS COMPONENTES   ->    ARDUINO UNO  ]
---------------------------------------------
              Botão1 -> pin D2 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão2 -> pin D7 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão3 -> pin D4 - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Botão4 -> pin D3 (interrupção externa) - - - Ordem de Ligação: positivo, botão, pino e resistor,GND
              Buzzer passivo -> pin D11
              Potenciômetro -> pin A0

