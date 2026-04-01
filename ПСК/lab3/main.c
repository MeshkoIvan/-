#include "project.h"
#include <string.h>

// 1. Хелпер для зручного керування RGB (Active Low: 0 = світиться, 1 = вимкнено)
void SetLED(uint8_t r, uint8_t g, uint8_t b) {
    LED_R_Write(r);
    LED_G_Write(g);
    LED_B_Write(b);
}

// 2. Функція сканування матричної клавіатури 4x3
char ScanKeypad(void) {
    char key = 0;

    // Перевіряємо 1-й стовпець (С_1)
    C_1_Write(0); C_2_Write(1); C_3_Write(1);
    CyDelayUs(10); // Мікрозатримка для стабілізації напруги
    if (R_1_Read() == 0) key = '1';
    if (R_2_Read() == 0) key = '4';
    if (R_3_Read() == 0) key = '7';
    if (R_4_Read() == 0) key = '*';

    // Перевіряємо 2-й стовпець (С_2)
    C_1_Write(1); C_2_Write(0); C_3_Write(1);
    CyDelayUs(10);
    if (R_1_Read() == 0) key = '2';
    if (R_2_Read() == 0) key = '5';
    if (R_3_Read() == 0) key = '8';
    if (R_4_Read() == 0) key = '0';

    // Перевіряємо 3-й стовпець (С_3)
    C_1_Write(1); C_2_Write(1); C_3_Write(0);
    CyDelayUs(10);
    if (R_1_Read() == 0) key = '3';
    if (R_2_Read() == 0) key = '6';
    if (R_3_Read() == 0) key = '9';
    if (R_4_Read() == 0) key = '#';

    // Повертаємо всі стовпці у високий рівень (вимкнено)
    C_1_Write(1); C_2_Write(1); C_3_Write(1);

    return key;
}

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    SW_Tx_UART_Start();
    SW_Tx_UART_PutString("\r\n--- Keypad Lab Started ---\r\n");

    char key = 0;
    char last_key = 0;
    uint8_t is_started = 0; // Прапорець для відстеження самого першого натискання

    // Налаштування для перевірки пароля (Завдання 3)
    char password[] = "1234"; // Пароль > 2 символів
    char input_buf[5] = {0};  // Буфер для вводу
    uint8_t input_idx = 0;

    for(;;) {
        key = ScanKeypad();

        // --- Завдання 1: Логіка світіння та відпускання ---
        if (key == 0) {
            // Якщо нічого не натиснуто
            if (is_started == 0) {
                SetLED(0, 0, 0); // Початковий стан: Білий (усі компоненти включені)
            } else {
                SetLED(1, 1, 1); // Після відпускання (і якщо вже був клік): Чорний (вимкнено)
            }
        } 
        else {
            is_started = 1; // Користувач зробив перший клік, знімаємо "початковий стан"

            // --- Завдання 2: Використання switch(case) для кольорів ---
            // Кольори міксуються базовими каналами: R, G, B
            switch(key) {
                case '1': case '7': SetLED(0, 1, 1); break; // Red
                case '2': case '8': SetLED(1, 0, 1); break; // Green
                case '3': case '9': SetLED(1, 1, 0); break; // Blue
                case '4': case '*': SetLED(0, 0, 1); break; // Yellow (Red + Green)
                case '5': case '0': SetLED(0, 1, 0); break; // Purple (Red + Blue)
                case '6': case '#': SetLED(1, 0, 0); break; // Cyan (Green + Blue)
                default:            SetLED(1, 1, 1); break; // Про всяк випадок - вимкнено
            }
        }

        // --- Завдання 3: Логіка введення пароля ---
        // Відловлюємо "фронт" (moment of key press), щоб не зчитувати 100 разів при утриманні
        if (key != 0 && last_key == 0) {
            input_buf[input_idx] = key;
            input_idx++;
            
            SW_Tx_UART_PutChar(key); // Відображаємо введені символи в консолі

            // Якщо ввели 4 символи, перевіряємо
            if (input_idx >= 4) {
                input_buf[4] = '\0'; // Закриваємо масив символів як рядок
                
                if (strcmp(input_buf, password) == 0) {
                    SW_Tx_UART_PutString("\r\n >> PASSWORD CORRECT! ACCESS GRANTED.\r\n");
                } else {
                    SW_Tx_UART_PutString("\r\n >> WRONG PASSWORD!\r\n");
                }
                
                input_idx = 0; // Скидаємо буфер для наступної спроби
            }
        }

        last_key = key; // Оновлюємо стан кнопки
        CyDelay(20);    // Затримка для антидребізгу (debouncing)
    }
}