#include "ssd1677.h"
#include <stdio.h>
// Déclaration des variables globales pour les périphériques SPI et GPIO
static SPI_HandleTypeDef *ssd1677_hspi;
static GPIO_TypeDef *ssd1677_cs_port;
static uint16_t ssd1677_cs_pin;
static GPIO_TypeDef *ssd1677_dc_port;
static uint16_t ssd1677_dc_pin;
static GPIO_TypeDef *ssd1677_reset_port;
static uint16_t ssd1677_reset_pin;
static GPIO_TypeDef *ssd1677_busy_port;
static uint16_t ssd1677_busy_pin;

// Fonction pour envoyer une commande au SSD1677
static void SSD1677_SendCommand(uint8_t command) {
    HAL_GPIO_WritePin(ssd1677_dc_port, ssd1677_dc_pin, GPIO_PIN_RESET); // Sélectionner le mode commande
    HAL_SPI_Transmit(ssd1677_hspi, &command, 1, HAL_MAX_DELAY); // Envoyer la commande via SPI
}

// Fonction pour envoyer des données au SSD1677
static void SSD1677_SendData(uint8_t data) {
    HAL_GPIO_WritePin(ssd1677_dc_port, ssd1677_dc_pin, GPIO_PIN_SET); // Sélectionner le mode données
    HAL_SPI_Transmit(ssd1677_hspi, &data, 1, HAL_MAX_DELAY); // Envoyer les données via SPI
}

// Fonction pour attendre que l'écran soit prêt (BUSY pin)
static void SSD1677_WaitUntilIdle(void) {
	uint8_t busy;
	printf("e-Paper busy \r\n ");
	do{
		busy = HAL_GPIO_ReadPin(ssd1677_busy_port, ssd1677_busy_pin);
	}while (busy);
	HAL_Delay(200);
	printf("e-Paper busy release \r\n ");
}

// Fonction d'initialisation du SSD1677
void SSD1677_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *dc_port, uint16_t dc_pin, GPIO_TypeDef *reset_port, uint16_t reset_pin, GPIO_TypeDef *busy_port, uint16_t busy_pin) {
    ssd1677_hspi = hspi; // Initialiser les pointeurs de périphériques
    ssd1677_cs_port = cs_port;
    ssd1677_cs_pin = cs_pin;
    ssd1677_dc_port = dc_port;
    ssd1677_dc_pin = dc_pin;
    ssd1677_reset_port = reset_port;
    ssd1677_reset_pin = reset_pin;
    ssd1677_busy_port = busy_port;
    ssd1677_busy_pin = busy_pin;

    SSD1677_Reset();

    SSD1677_SendCommand(0x12);
    HAL_Delay(300);


    SSD1677_SendCommand(0x46);
    SSD1677_SendData(0xF7);
    SSD1677_WaitUntilIdle();
    SSD1677_SendCommand(0x47);
    SSD1677_SendData(0xF7);
    SSD1677_WaitUntilIdle();

    SSD1677_SendCommand(0x01); // setting gate number
    SSD1677_SendData(0xDF);
    SSD1677_SendData(0x01);
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x03); // set gate voltage
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x04); // set source voltage
    SSD1677_SendData(0x41);
    SSD1677_SendData(0xA8);
    SSD1677_SendData(0x32);

    SSD1677_SendCommand(0x11); // set data entry sequence
    SSD1677_SendData(0x03);

    SSD1677_SendCommand(0x3C); // set border 
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x0C); // set booster strength
    SSD1677_SendData(0xAE);
    SSD1677_SendData(0xC7);
    SSD1677_SendData(0xC3);
    SSD1677_SendData(0xC0);
    SSD1677_SendData(0xC0);  

    SSD1677_SendCommand(0x18); // set internal sensor on
    SSD1677_SendData(0x80);

    SSD1677_SendCommand(0x2C); // set vcom value
    SSD1677_SendData(0x44);

    SSD1677_SendCommand(0x37); // set display option, these setting turn on previous function
    SSD1677_SendData(0x00);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);  
    SSD1677_SendData(0x4f);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);
    SSD1677_SendData(0xff);  

    SSD1677_SendCommand(0x44); // setting X direction start/end position of RAM
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x17);
    SSD1677_SendData(0x01);

    SSD1677_SendCommand(0x45); // setting Y direction start/end position of RAM
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0xDF);
    SSD1677_SendData(0x01);

    SSD1677_SendCommand(0x22); // Display Update Control 2
    SSD1677_SendData(0xCF);
    
}

// Fonction pour effacer l'écran
void SSD1677_Clear(uint8_t mode) {
    uint16_t i;
    uint16_t IMAGE_COUNTER = EPD_WIDTH * EPD_HEIGHT / 8;

    SSD1677_SendCommand(0x4E);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);
    SSD1677_SendCommand(0x4F);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++)
    {
        SSD1677_SendData(0xff);
    }
    if(mode)
        Load_LUT(1);
    else
        Load_LUT(0); 
    SSD1677_SendCommand(0x20);
    SSD1677_WaitUntilIdle();
}


// reset
void SSD1677_Reset(void)
{
    // Réinitialisation matérielle
    HAL_GPIO_WritePin(ssd1677_reset_port, ssd1677_reset_pin, GPIO_PIN_SET); // Mettre le pin RESET à haut
    HAL_Delay(20); // Attendre 20 ms
    HAL_GPIO_WritePin(ssd1677_reset_port, ssd1677_reset_pin, GPIO_PIN_RESET); // Mettre le pin RESET à bas
    HAL_Delay(2); // Attendre 100 ms
    HAL_GPIO_WritePin(ssd1677_reset_port, ssd1677_reset_pin, GPIO_PIN_SET); // Mettre le pin RESET à haut
    HAL_Delay(20); // Attendre 20 ms
}

void SSD1677_Sleep(void)
{
    SSD1677_SendCommand(0X10);   //deep sleep
    SSD1677_SendData(0x03);
}

void SSD1677_TurnOnDisplay(void)
{
    SSD1677_SendCommand(0x20);
	SSD1677_WaitUntilIdle(); 
}

void Load_LUT(uint8_t mode)
{
    uint8_t i;
    SSD1677_SendCommand(0x32);
    for (i = 0; i < 105; i++) {
        if(mode == 1)
            SSD1677_SendData(lut_1Gray_GC[i]);
        else if(mode == 0)
            SSD1677_SendData(lut_1Gray_A2[i]);
    }   
}

/******************************************************************************
function :  Display
******************************************************************************/
void DisplayFrame(const uint8_t *Image, bool isBase)
{
    uint16_t i;
    uint16_t IMAGE_COUNTER = EPD_WIDTH * EPD_HEIGHT / 8;

    SSD1677_SendCommand(0x4E);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);
    SSD1677_SendCommand(0x4F);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++) {
        SSD1677_SendData(Image[i]);
    }
    if(isBase) {
        SSD1677_SendCommand(0x26);
        for (i = 0; i < IMAGE_COUNTER; i++) {
            SSD1677_SendData(Image[i]);
        }
    }

    Load_LUT(1);
    SSD1677_SendCommand(0x20);
    SSD1677_WaitUntilIdle();  
}
/******************************************************************************
function :  Sends part the image buffer in RAM to e-Paper and displays
notes:
    1. Xstart must be a multiple of 8
    1. isGC: use GC mode to clear base-image
******************************************************************************/

void DisplayFrame_Part(const uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, bool isGC)
{
    uint16_t i, Width;
    Width = (Xend-Xstart)%8 == 0 ? (Xend-Xstart)/8 : (Xend-Xstart)/8+1;
    uint16_t IMAGE_COUNTER = Width * (Yend-Ystart);


	Xend -= 1;
	Yend -= 1;

	SSD1677_SendCommand(0x44);
	SSD1677_SendData(Xstart & 0xff);
	SSD1677_SendData((Xstart>>8) & 0x03);
	SSD1677_SendData(Xend & 0xff);
	SSD1677_SendData((Xend>>8) & 0x03);
	SSD1677_SendCommand(0x45);
	SSD1677_SendData(Ystart & 0xff);
	SSD1677_SendData((Ystart>>8) & 0x03);
	SSD1677_SendData(Yend & 0xff);
	SSD1677_SendData((Yend>>8) & 0x03);

    SSD1677_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SSD1677_SendData(Xstart & 0xFF);

    SSD1677_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SSD1677_SendData(Ystart & 0xFF);
    SSD1677_SendData((Ystart >> 8) & 0xFF);
	
	SSD1677_SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++)
	{
	    SSD1677_SendData(Image[i]);
	}
    if(isGC)
        Load_LUT(1);
    else
        Load_LUT(0);
}
/******************************************************************************
function :  Partial Display
note:
    1. iwidth must be a multiple of 8
    2. The update will clear other parts of the image data
******************************************************************************/
void DisplayFrame_Partial(const uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t iwidth,uint16_t iheight)
{
    uint16_t i, j;
    SSD1677_SendCommand(0x4E);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);
    SSD1677_SendCommand(0x4F);
    SSD1677_SendData(0x00);
    SSD1677_SendData(0x00);

    SSD1677_SendCommand(0x24);
    for(i=0; i<EPD_WIDTH; i++) {
        for(j=0; j<EPD_HEIGHT/8; j++) {
            if(i>=Ystart && i<Ystart+iheight && j>=Xstart/8 && j<(Xstart+iwidth)/8)
                SSD1677_SendData(Image[(j-Xstart/8) + (i-Ystart)*iwidth/8]);
            else
                SSD1677_SendData(0xff);
        }
    }

    Load_LUT(0);
    SSD1677_SendCommand(0x20);
    SSD1677_WaitUntilIdle();   
}
