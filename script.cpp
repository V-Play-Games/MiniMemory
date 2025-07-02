#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// Buttons
int BUT1 = 0;
int BUT2 = 1;
int BUT3 = 2;
int BUT4 = 3;

// LEDs
int LED1 = 4;
int LED2 = 5;
int LED3 = 10;
int LED4 = 9;

// Game Mode
#define NOT_PLAYING -1
#define GAME_INIT 0
#define DIFFICULTY_1 1
#define DIFFICULTY_2 2
#define DIFFICULTY_3 3
#define DIFFICULTY_4 4
#define PLAYING 5
#define GIVING_OUTPUT 6
#define TAKING_INPUT 7
#define LOW 0
#define HIGH 1

int gameMode = -1;
int difficulty = 0;
long lastInput = 0;
int INPUT = 0;
int OUTPUT = 1;
std::string sequence = "";

void setup()
{
  // weird ahh setup
    pinMode(BUT1, INPUT);
    pinMode(BUT2, INPUT);
    pinMode(BUT3, INPUT);
    pinMode(BUT4, INPUT);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
}

void loop()
{
  // boring ahh repititive code
    bool but1p = buttonPressed(BUT1);
    bool but2p = buttonPressed(BUT2);
    bool but3p = buttonPressed(BUT3);
    bool but4p = buttonPressed(BUT4);

    switch (gameMode)
    {
    case NOT_PLAYING:
        if (but1p && but2p && but3p && but4p)
        {
            lastInput = millis();
            gameMode = GAME_INIT;
        }
        break;
    case GAME_INIT:
        if (but1p && but2p && but3p && but4p)
        {
            if (millis() >= lastInput + 3000)
            {
                gameMode = DIFFICULTY_1;
            }
        }
        else
        {
            lastInput = 0;
            gameMode = NOT_PLAYING;
        }
        break;
    case DIFFICULTY_1:
        if (but1p && !but2p && !but3p && !but4p)
        {
            difficulty = DIFFICULTY_1;
            gameMode = PLAYING;
        }
        break;
    case DIFFICULTY_2:
        if (!but1p && but2p && !but3p && !but4p)
        {
            difficulty = DIFFICULTY_1;
            gameMode = PLAYING;
        }
        break;
    case DIFFICULTY_3:
        if (!but1p && !but2p && but3p && !but4p)
        {
            difficulty = DIFFICULTY_1;
            gameMode = PLAYING;
        }
        break;
    case DIFFICULTY_4:
        if (!but1p && !but2p && !but3p && but4p)
        {
            difficulty = DIFFICULTY_1;
            gameMode = PLAYING;
        }
        break;
    case PLAYING:
        srand((unsigned)time(0));
        sequence += std::to_string((rand() % 6) + 1);
        gameMode = GIVING_OUTPUT;
        break;
    case GIVING_OUTPUT:
        for (char c : sequence)
        {
            int led = c - '0';
            if (led >= 1 && led <= 4)
            {
                pinMode(LED1 + led - 1, HIGH);
                delay(500 / difficulty);
                pinMode(LED1 + led - 1, LOW);
            }
        }
        gameMode = TAKING_INPUT;
        break;
    case TAKING_INPUT:
        static bool waitingForRelease = false;
        static size_t inputIndex = 0;
        static long debounceTime = 0;

        if (!waitingForRelease) {
            int pressedButton = 0;
            int pressedCount = 0;
            // pretyy formattinggg
            if (but1p) { pressedButton = 1; pressedCount++; }
            if (but2p) { pressedButton = 2; pressedCount++; }
            if (but3p) { pressedButton = 3; pressedCount++; }
            if (but4p) { pressedButton = 4; pressedCount++; }

            if (pressedCount == 1 && millis() - debounceTime > 200) {
                debounceTime = millis();
                waitingForRelease = true;

                if (inputIndex < sequence.size()) {
                    if (pressedButton == (sequence[inputIndex] - '0')) {
                        inputIndex++;
                        if (inputIndex == sequence.size()) {
                            inputIndex = 0;
                            sequence.clear();
                            gameMode = NOT_PLAYING;
                        }
                        // else wait for next input :)
                    } else {
                        // BAD, VERY BAD input
                        inputIndex = 0;
                        gameMode = NOT_PLAYING;
                    }
                } else {
                    // too much ahh input, reset
                    inputIndex = 0;
                    gameMode = NOT_PLAYING;
                }
            }
        } else {
            if (!but1p && !but2p && !but3p && !but4p) {
                waitingForRelease = false;
            }
        }
        break;
    }
}

bool buttonPressed(int button)
{
    return digitalRead(button) == HIGH;
}
