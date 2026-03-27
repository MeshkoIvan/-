#include "project.h"

int main(void)
{
    CyGlobalIntEnable; 

    for(;;)
    {
        /* Якщо Button_Read() == 0, значить кнопка ФІЗИЧНО натиснута */
        if (Button_Read() == 0) 
        {
            /* LED_Write(0) — подаємо "нуль", щоб світлодіод засвітився */
            LED_Write(0); 
        }
        else
        {
            /* LED_Write(1) — подаємо "одиницю", щоб вимкнути його */
            LED_Write(1); 
        }
    }
}