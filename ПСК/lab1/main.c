#include "project.h"
int main(void)
{
CyGlobalIntEnable; /* Enable global interrupts. */
/* Place your initialization/startup code here (e.g. MyInst_Start()) */
SW_Tx_UART_Start();
SW_Tx_UART_PutCRLF();
SW_Tx_UART_PutString("Software Transmit UART");
SW_Tx_UART_PutCRLF();
uint8_t last_state = 0;
for(;;)
{
/* Place your application code here. */
// button == 0 -> Button pressed
// button == 1 -> Button released
// LED_R_Write(0); -> Turn ON
// LED_R_Write(1); -> Turn OFF
if(Button_Read() == 1 && last_state == 0) //is button released
{
last_state = 1;
SW_Tx_UART_PutString("Button released");
SW_Tx_UART_PutCRLF();
LED_R_Write(0); // Turn ON
LED_G_Write(0);
LED_B_Write(0);
}
else if(Button_Read() == 0 && last_state == 1) //is button pressed
{
last_state = 0;
SW_Tx_UART_PutString("Button pressed");
SW_Tx_UART_PutCRLF();
LED_R_Write(1); // Turn OFF
LED_G_Write(1);
LED_B_Write(1);
}
}
}