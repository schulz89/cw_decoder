// Arduino program to decode CW beacon in morse code
// Copyright (C) 2023 Victor Hugo Schulz

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#define CW_IN 12
#define BUFFER_SIZE 500L
#define TIMER_SETTING 15000 // 15000 corresponds to 60ms

bool buffer[BUFFER_SIZE] = { false };
char morse[BUFFER_SIZE / 2];
char decoded[BUFFER_SIZE / 4];

uint16_t counter = 0;
bool end = false;

// (timer speed (Hz)) = (Arduino clock speed (16MHz)) / prescaler
void setup_timer()
{
    noInterrupts();
    TCCR1A = 0;            // Reset entire TCCR1A register
    TCCR1B = 0;            // Reset entire TCCR1B register
    TCCR1B |= B00000011;   // Set CS012 to 011 so we get prescaler 64
    TIMSK1 |= B00000010;   // Set OCIE1A to 1 so we enable compare match A
    OCR1A = TIMER_SETTING; // 15000 corresponds to 60ms
    TCNT1 = 0;             // Reset Timer 1 value to 0
    interrupts();
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(CW_IN, INPUT);
    Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop()
{
    while (!digitalRead(CW_IN)) { // Wait for CW to start
    }
    end = false; // Restarting the cycle
    setup_timer();
    bool previous = digitalRead(CW_IN);
    while (!end) {
        bool current = digitalRead(CW_IN);
        if(current != previous){
            TCNT1 = TIMER_SETTING - 1; // Anticipates timer 1 underflow to synchronize with symbol speed
            previous = current;
        }
    }
    // for (uint16_t i = 0; i < sizeof(buffer); i++) {
    //     Serial.print(buffer[i]);
    // }
    // Serial.print("\r\n");
    bin_to_morse();
    // Serial.println(morse);
    morse_to_characters();
    Serial.println(decoded);
}

void bin_to_morse()
{
    memset(morse, 0, sizeof(morse));
    uint16_t i = 0;
    uint16_t j = 0;
    while (i < sizeof(buffer) && j < sizeof(morse)) {
        if (buffer[i] && !buffer[i + 1]) {
            morse[j] = '.';
            j++;
            i += 2;
        } else if (buffer[i] && buffer[i + 1] && buffer[i + 2] && !buffer[i + 3]) {
            morse[j] = '-';
            j++;
            i += 4;
        } else if (!buffer[i] && !buffer[i + 1] && !buffer[i + 2] && !buffer[i + 3]
            && !buffer[i + 4] && !buffer[i + 5] && !buffer[i + 6] && !buffer[i + 7]
            && !buffer[i + 8] && !buffer[i + 9] && !buffer[i + 10] && !buffer[i + 11]) {
            morse[j] = ' ';
            morse[j + 1] = '/';
            morse[j + 2] = ' ';
            j += 3;
            i += 12;
        } else if (!buffer[i] && !buffer[i + 1] && !buffer[i + 2]) {
            morse[j] = ' ';
            j++;
            i += 3;
        } else {
            i++;
        }
    }
    buffer[j] = '\0';
}

void morse_to_characters()
{
    uint16_t i = 0;
    char* ptr = strtok(morse, " ");
    while (ptr != NULL) {
        // clang-format off
             if(strcmp(ptr, "/"    ) == 0) { decoded[i] = ' '; }
        else if(strcmp(ptr, "-----") == 0) { decoded[i] = '0'; }
        else if(strcmp(ptr, ".----") == 0) { decoded[i] = '1'; }
        else if(strcmp(ptr, "..---") == 0) { decoded[i] = '2'; }
        else if(strcmp(ptr, "...--") == 0) { decoded[i] = '3'; }
        else if(strcmp(ptr, "....-") == 0) { decoded[i] = '4'; }
        else if(strcmp(ptr, ".....") == 0) { decoded[i] = '5'; }
        else if(strcmp(ptr, "-....") == 0) { decoded[i] = '6'; }
        else if(strcmp(ptr, "--...") == 0) { decoded[i] = '7'; }
        else if(strcmp(ptr, "---..") == 0) { decoded[i] = '8'; }
        else if(strcmp(ptr, "----.") == 0) { decoded[i] = '9'; }
        else if(strcmp(ptr, ".-"   ) == 0) { decoded[i] = 'A'; }
        else if(strcmp(ptr, "-..." ) == 0) { decoded[i] = 'B'; }
        else if(strcmp(ptr, "-.-." ) == 0) { decoded[i] = 'C'; }
        else if(strcmp(ptr, "-.."  ) == 0) { decoded[i] = 'D'; }
        else if(strcmp(ptr, "."    ) == 0) { decoded[i] = 'E'; }
        else if(strcmp(ptr, "..-." ) == 0) { decoded[i] = 'F'; }
        else if(strcmp(ptr, "--."  ) == 0) { decoded[i] = 'G'; }
        else if(strcmp(ptr, "...." ) == 0) { decoded[i] = 'H'; }
        else if(strcmp(ptr, ".."   ) == 0) { decoded[i] = 'I'; }
        else if(strcmp(ptr, ".---" ) == 0) { decoded[i] = 'J'; }
        else if(strcmp(ptr, "-.-"  ) == 0) { decoded[i] = 'K'; }
        else if(strcmp(ptr, ".-.." ) == 0) { decoded[i] = 'L'; }
        else if(strcmp(ptr, "--"   ) == 0) { decoded[i] = 'M'; }
        else if(strcmp(ptr, "-."   ) == 0) { decoded[i] = 'N'; }
        else if(strcmp(ptr, "---"  ) == 0) { decoded[i] = 'O'; }
        else if(strcmp(ptr, ".--." ) == 0) { decoded[i] = 'P'; }
        else if(strcmp(ptr, "--.-" ) == 0) { decoded[i] = 'Q'; }
        else if(strcmp(ptr, ".-."  ) == 0) { decoded[i] = 'R'; }
        else if(strcmp(ptr, "..."  ) == 0) { decoded[i] = 'S'; }
        else if(strcmp(ptr, "-"    ) == 0) { decoded[i] = 'T'; }
        else if(strcmp(ptr, "..-"  ) == 0) { decoded[i] = 'U'; }
        else if(strcmp(ptr, "...-" ) == 0) { decoded[i] = 'V'; }
        else if(strcmp(ptr, ".--"  ) == 0) { decoded[i] = 'W'; }
        else if(strcmp(ptr, "-..-" ) == 0) { decoded[i] = 'X'; }
        else if(strcmp(ptr, "-.--" ) == 0) { decoded[i] = 'Y'; }
        else if(strcmp(ptr, "--.." ) == 0) { decoded[i] = 'Z'; }
        else                               { decoded[i] = '?'; }
        // clang-format on
        i++;
        if (i >= sizeof(decoded) - 1)
            break;
        ptr = strtok(NULL, " ");
    }
    decoded[i] = '\0';
}

ISR(TIMER1_COMPA_vect) // Timer 1 interruption
{
    TCNT1 = 0; // First, set the timer back to 0 so it resets for next interrupt
    bool data = digitalRead(CW_IN);
    digitalWrite(LED_BUILTIN, data); // Write new state to the LED
    buffer[counter] = digitalRead(CW_IN);
    counter++;
    if (counter >= BUFFER_SIZE) {
        TIMSK1 &= B11111101;
        counter = 0;
        end = true;
    }
}
