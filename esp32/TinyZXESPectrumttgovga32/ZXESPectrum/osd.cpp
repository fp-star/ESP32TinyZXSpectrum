#include "gbConfig.h"
#include "gbOptimice.h"
#include "dataFlash/gbtape.h"
#include "dataFlash/gbscr.h"
#include "dataFlash/gbrom.h"
#include "dataFlash/gbsna.h"
#include "osd.h"
#include "Disk.h"
#include "PS2Kbd.h"
#include "z80main.h"
#include "ZXESPectrum.h"
//#include "def/Font.h"
#include "keys.h"
#include "hardware.h"
#include "gbGlobals.h"
#include "gb_sdl_font8x8.h"
#include "Memory.h"

#ifdef use_lib_core_jsanchezv
 #include "Memory.h"
 #include "jsanchezv_z80sim.h"
#endif 

const unsigned char gb_const_monoBlue8[8]=
{//cache de colores optimizado
 0,16,32,36,48,56,60,62
};
const unsigned char gb_const_monoGreen8[8]=
{//cache de colores optimizado
 0,4,8,12,14,24,28,30
};
const unsigned char gb_const_monoRed8[8]=
{//cache de colores optimizado
 0,1,2,3,6,7,11,27
};
const unsigned char gb_const_grey8[8]=
{//escala de grises
 0,16,21,33,37,42,58,63
};
const unsigned char gb_const_colornormal8[8]=
{
  BLACK, BLUE, RED, MAGENTA, GREEN, CYAN, YELLOW, WHITE
};

#ifdef use_lib_use_bright
 const unsigned char gb_const_monoBlue8_bright[8]=
 {//cache de colores optimizado
  0,16,32,36,48,56,60,62
 };
 const unsigned char gb_const_monoGreen8_bright[8]=
 {//cache de colores optimizado
  0,4,8,12,14,24,28,30
 };
 const unsigned char gb_const_monoRed8_bright[8]=
 {//cache de colores optimizado
  0,1,2,3,6,7,11,27
 };
 const unsigned char gb_const_grey8_bright[8]=
 {//escala de grises
  0,16,21,33,37,42,58,63
 };
 const unsigned char gb_const_colornormal8_bright[8]=
 {
   BRI_BLACK, BRI_BLUE, BRI_RED, BRI_MAGENTA, BRI_GREEN, BRI_CYAN, BRI_YELLOW, BRI_WHITE
 };
#endif

//extern int gb_screen_xIni;
//extern int gb_screen_yIni;
extern unsigned char gb_cache_zxcolor[8];
#ifdef use_lib_use_bright
 extern unsigned char gb_cache_zxcolor_bright[8];
#endif
unsigned char gb_show_osd_main_menu=0;

#ifdef use_lib_vga8colors
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 7 //Blanco
 //#define ID_COLOR_MAGENTA 6 //Magenta
 #define ID_COLOR_MAGENTA 3 //Magenta
#else
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 7 //Blanco
 //#define ID_COLOR_MAGENTA 6 //Magenta
 #define ID_COLOR_MAGENTA 3 //Magenta
#endif 


//void SDLOSDClear()
//{
// //SDLClear(gb_osd_sdl_surface);
//}

#define max_gb_osd_screen 7
const char * gb_osd_screen[max_gb_osd_screen]={
 "Cols Left",
 "Pixels Top",
 "Color 8",
 "Mono Blue 8",
 "Mono Green 8",
 "Mono Red 8",
 "Mono Grey 8" 
};

#define max_gb_osd_screen_values 5
const char * gb_osd_screen_values[max_gb_osd_screen_values]={
 "0",
 "1",
 "2", 
 "3", 
 "4"
};

//#define max_gb_osd_speed_div_emulate 8
//const char * gb_osd_speed_div_emulate[max_gb_osd_speed_div_emulate]={
// "5",
// "2",
// "4",
// "8",
// "16",
// "32",
// "64",
// "0"
//};

#define max_gb_osd_delay_instructions 54
const char * gb_osd_delay_instructions[max_gb_osd_delay_instructions]={
 "(AUTO)",
 "0",
 "1",
 "2",
 "3",
 "4",
 "5",
 "6",
 "7",
 "8",
 "9",
 "10",
 "11",
 "12",
 "13",
 "14",
 "15",
 "16",
 "17",
 "18",
 "19",
 "20",
 "21",
 "22",
 "23",
 "24",
 "25",
 "26",
 "27",
 "28",
 "29",
 "30",
 "31",
 "32",
 "33",
 "34",
 "35",
 "36",
 "37",
 "38",
 "39",
 "40",
 "41",
 "42",
 "43",
 "44", 
 "45",
 "46",
 "47",
 "48",
 "49",
 "50",
 "51",
 "52"
};

#define max_gb_main_menu 12
const char * gb_main_menu[max_gb_main_menu]={
 "Load SNA",
 "Select ROM",
 "Load SCR TAP",
 "Select TAP",
 "Load SCR",
 "Speed",
 "Screen Adjust",
 "Force Mode",
 "Mouse",
 "Sound", 
 "Reset",
 "Return"
};

#ifdef use_lib_mouse_kempston
 #define max_gb_osd_mouse_menu 10
 const char * gb_osd_mouse_menu[max_gb_osd_mouse_menu]={
  "Mouse Detect",
  "right handed",
  "left handed",
  "Invert X",
  "Invert Y",
  "Speed Normal",
  "Speed 2x",
  "Speed 3x",
  "Speed 4x",
  "Mouse Enabled"
 };
#endif 

#define max_gb_machine_menu 2
const char * gb_machine_menu[max_gb_machine_menu]={
 "48K",
 "128K"
};

#define max_gb_speed_menu 5
const char * gb_speed_menu[max_gb_speed_menu]={
 "Normal",
 "2x",
 "4x",
 "8x",
 "16x"
};

#define max_gb_speed_videoaudio_options_menu 5
const char * gb_speed_videoaudio_options_menu[max_gb_speed_videoaudio_options_menu]={
 "Audio poll",
 "Video delay",
 "Video speed",
 "Keyboard poll",
 "Delay CPU"
};

#define max_gb_speed_sound_menu 9
const char * gb_speed_sound_menu[max_gb_speed_sound_menu]={
 "0",
 "1",
 "2",
 "3",
 "4",
 "5",
 "6",
 "7",
 "8"
};

#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};

#define max_gb_mode48_128_menu 2
const char * gb_mode48_128_menu[max_gb_mode48_128_menu]={
 "48K",
 "128K"
};

#define max_osd_sound_vol_values 5
const char * gb_osd_sound_vol_values[max_osd_sound_vol_values]={
 "100%",
 "75%",
 "50%",
 "25%",
 "5%"
};

#define max_gb_osd_sound_menu 2
const char * gb_osd_sound_menu[max_gb_osd_sound_menu]={
 "Sound Enabled",
 "Sound Volume"
};

#define max_gb_value_binary_menu 2
const char * gb_value_binary_menu[max_gb_value_binary_menu]={
 "0 (OFF)",
 "1 (ON)"
};


#ifdef use_lib_sound_ay8912
 //***************************
 void ResetSound()
 {
  gb_ay8912_reg_select=0;
  gb_ay8912_reg_value=0;
  gb_ay8912_A_frec_fine=0;
  gb_ay8912_B_frec_fine=0;
  gb_ay8912_C_frec_fine=0;
  gb_ay8912_A_frec_course=0;
  gb_ay8912_B_frec_course=0;
  gb_ay8912_C_frec_course=0;
  gb_ay8912_A_frec=gb_frecuencia_ini;
  gb_ay8912_B_frec=gb_frecuencia_ini;
  gb_ay8912_C_frec=gb_frecuencia_ini;
  gb_ay8912_noise_pitch=0;
  gb_ay8912_A_vol=0;
  gb_ay8912_B_vol=0;
  gb_ay8912_C_vol=0;
  gb_ay8912_mixer=0;
  #ifdef use_lib_resample_speaker
   gbContSPKChangeSamples = 0;  
  #endif 
 }
#endif


//*************************
void SetMode48K()
{
 #ifdef use_lib_core_linkefong
  rom_latch = 0;
  rom_in_use = 0;
  bank_latch = 0;
  paging_lock = 1;
 #else
  rom_latch_jsanchezv = 0;
  rom_in_use_jsanchezv = 0;
  bank_latch_jsanchezv = 0;
  paging_lock_jsanchezv = 1; 
 #endif 
}

//**********************************************
void SetMode128K()
{
 #ifdef use_lib_core_linkefong   
  rom_in_use = 1;
  rom_latch = 1;
  paging_lock = 1;
  bank_latch = 0;
  video_latch = 0;
  paging_lock = 1;
  bank_latch = 0;
  video_latch = 0;
 #else
  rom_in_use_jsanchezv = 1;
  rom_latch_jsanchezv = 1;
  paging_lock_jsanchezv = 1;
  bank_latch_jsanchezv = 0;
  video_latch_jsanchezv = 0;
  paging_lock_jsanchezv = 1;
  bank_latch_jsanchezv = 0;
  video_latch_jsanchezv = 0; 
 #endif 
}




//********************************************************************
void SDLClear()
{
 unsigned int a32= gb_cache_zxcolor[0];
 a32= a32|(a32<<8)|(a32<<16)|(a32<<24);
 for (int y=0; y<gb_topeY; y++){
  for (int x=0; x<gb_topeX_div4; x++){
   gb_buffer_vga32[y][x]= a32;
  }
 }
}

//***********************************************************
void jj_fast_putpixel(int x,int y,unsigned char c)
{
 gb_buffer_vga[y][x^2]= gb_cache_zxcolor[c];
}

//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 int auxId = car << 3; //*8
 unsigned char aux;
 unsigned char auxBit,auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  aux = gb_sdl_font_8x8[auxId + j];
  for (int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
  }
 }
}

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}

//****************************
//void SDLEsperaTeclado()
//{
// //SDL_WaitEvent(gb_osd_sdl_event);
//}

#ifdef use_lib_vga360x200
 #define gb_pos_x_menu 120
#else
 #define gb_pos_x_menu 80
#endif 
#define gb_pos_y_menu 50
#define gb_osd_max_rows 10

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 //jjvga vga.setTextColor(WHITE,BLACK);
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  //jjvga vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
  //jjvga vga.print("                    ");
  SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),ID_COLOR_BLACK,ID_COLOR_BLACK);
 }
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;
//jjvga   if (i == 0)
//jjvga    vga.setTextColor(CYAN,BLUE);
//jjvga   else
//jjvga    vga.setTextColor(WHITE,BLACK);
//jjvga   vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
//jjvga   vga.print(ptrValue[currentId]);
  SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));
  currentId++;
 }     
}

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax)
{
 unsigned char aReturn=0;
 unsigned char salir=0; 
//jjvga  #ifdef use_lib_vga320x240
//jjvga   vga.clear(BLACK);
//jjvga   vga.fillRect(0,0,320,240,BLACK);
//jjvga   vga.fillRect(0,0,320,240,BLACK);//Repeat Fix visual defect   
//jjvga  #else
//jjvga   #ifdef use_lib_vga320x200
//jjvga    vga.clear(BLACK);
//jjvga    vga.fillRect(0,0,320,200,BLACK);
//jjvga    vga.fillRect(0,0,320,200,BLACK);//Repeat Fix visual defect 
//jjvga   #else
//jjvga    #ifdef use_lib_vga360x200
//jjvga     vga.clear(BLACK);
//jjvga     vga.fillRect(0,0,360,200,BLACK);
//jjvga     vga.fillRect(0,0,360,200,BLACK);//Repeat Fix visual defect 
//jjvga    #endif
//jjvga   #endif  
//jjvga  #endif
 //jjvga vTaskDelay(2);
 //jjvga vga.setTextColor(WHITE,BLACK);
 //jjvga vga.setCursor((gb_pos_x_menu-(32)), gb_pos_y_menu-16); 
 //jjvga vga.print("Mod ZXESPectrum by Ackerman");
 //jjvga vga.setTextColor(BLACK,WHITE);
 //SDLClear(gb_osd_sdl_surface);  

 SDLClear();
 #ifdef use_lib_core_linkefong
  SDLprintText("ZXESPectrum LKF(Ackerman)",(gb_pos_x_menu-(32)),(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK);
 #else
  SDLprintText("ZXESPectrum JLS(Ackerman)",(gb_pos_x_menu-(32)),(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK);
 #endif 
 //for (int i=0;i<20;i++)
 for (int i=0;i<14;i++)
 { 
  SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
  //SDLprintChar(gb_osd_sdl_surface,' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,BLACK,WHITE,1);
  //jjvga vga.setCursor((gb_pos_x_menu+(i*6)), gb_pos_y_menu);
  //jjvga vga.print(" ");
 } 
 //SDLprintText(gb_osd_sdl_surface,cadTitle,gb_pos_x_menu,gb_pos_y_menu,BLACK,WHITE,1);
 //jjvga vga.setCursor(gb_pos_x_menu,gb_pos_y_menu);
 //jjvga vga.print(cadTitle);  
 //for (int i=0;i<aMax;i++)
 //{     
 // //SDLprintText(gb_osd_sdl_surface,ptrValue[i],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);  
 // vga.setTextColor(((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK));
 // vga.setCursor(gb_pos_x_menu,(gb_pos_y_menu+8+(i<<3)));
 // vga.print(ptrValue[i]);
 //}
 SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 OSDMenuRowsDisplayScroll(ptrValue,0,aMax);
 //SDL_Flip(gb_osd_sdl_surface); 
 while (salir == 0)
 {             
  //case SDLK_UP:  
  if (checkAndCleanKey(KEY_CURSOR_LEFT))
  {
   if (aReturn>10) aReturn-=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }
  if (checkAndCleanKey(KEY_CURSOR_RIGHT))
  {
   if (aReturn<(aMax-10)) aReturn+=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }  
  if (checkAndCleanKey(KEY_CURSOR_UP))
  {
   if (aReturn>0) aReturn--;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_CURSOR_DOWN))
  {
   if (aReturn < (aMax-1)) aReturn++;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_ENTER))
  {
   salir= 1;
  }
  //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
  if (checkAndCleanKey(KEY_ESC))
  {
   salir=1; aReturn= 255;    
  }
  //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
  //default: break;    
 }
 gb_show_osd_main_menu= 0;
 return aReturn;
}

#ifdef use_lib_mouse_kempston
void ShowTinyMouseMenu()
{ 
  unsigned char aSelNum,aSelMouseEnabled;
  aSelNum = ShowTinyMenu("Mouse",gb_osd_mouse_menu,max_gb_osd_mouse_menu);   
  switch (aSelNum)
  {
   case 0: gb_mouse_init_error= 0; break; //Init Detectar raton
   case 1: gb_force_left_handed= 0; break; //diestro
   case 2: gb_force_left_handed= 1; break; //zurdo    
   case 3: gb_mouse_invert_deltax= (~gb_mouse_invert_deltax)&0x01; break; //Invertir Delay X      
   case 4: gb_mouse_invert_deltay= (~gb_mouse_invert_deltay)&0x01; break; //Invertir Delta Y
   case 5: gb_mouse_inc=1; break;
   case 6: gb_mouse_inc=2; break;
   case 7: gb_mouse_inc=3; break;
   case 8: gb_mouse_inc=4; break;
   case 9:
    aSelMouseEnabled = ShowTinyMenu("Mouse Enabled",gb_value_binary_menu,max_gb_value_binary_menu);    
    switch (aSelMouseEnabled)
    {
     case 0: gb_mouse_init_error= 1; break; //Error 0 usa mouse
     case 1: gb_mouse_init_error= 0; break; //Error 1 apaga mouse
     default: break;
    }
    break;
   default: break;
  }
 //vTaskDelay(2);
}
#endif



//Activar desactivar sonido
void ShowTinySoundMenu()
{
 unsigned char aSelNum,aSelSoundON,aSelVol;
 aSelNum = ShowTinyMenu("SOUND MENU",gb_osd_sound_menu,max_gb_osd_sound_menu); 
 switch (aSelNum)
 {
  case 0:
   aSelSoundON = ShowTinyMenu("SOUND ENABLED",gb_value_binary_menu,max_gb_value_binary_menu); 
   #ifdef use_lib_sound_ay8912
    switch (aSelSoundON)
    {
     case 0: gb_mute_sound=1; break;
     case 1: gb_mute_sound=0; break;
     default: break;
    }
   #endif
   break;
  case 1:
   aSelVol = ShowTinyMenu("SOUND VOLUME",gb_osd_sound_vol_values,max_osd_sound_vol_values);
   #ifdef use_lib_sound_ay8912
    gbShiftLeftVolumen= aSelVol;
   #endif 
   break;
  default: break;
 }

}

//Menu SNA
void ShowTinySNAMenu()
{
 unsigned char aSelNum;     
 aSelNum = ShowTinyMenu("Machine SNA",gb_machine_menu,max_gb_machine_menu);
 if (aSelNum == 255)
  return;
 Z80EmuSelectTape(0);
 if (aSelNum == 0)
 {
  aSelNum = ShowTinyMenu("48K SNA",gb_list_sna_48k_title,max_list_sna_48);                
  if (aSelNum == 255)
   return;  
  //strcpy(cfg_arch,"48K");
  //cfg_arch = "48K";
  gb_cfg_arch_is48K = 1;  
  changeSna2Flash(aSelNum,1); //SNA 48K
 }
 else
 {
  aSelNum = ShowTinyMenu("128K SNA",gb_list_sna_128k_title,max_list_sna_128);                
  if (aSelNum == 255)
   return;  
  //strcpy(cfg_arch,"128K");
  //cfg_arch = "128K";
  gb_cfg_arch_is48K = 0;
  changeSna2Flash(aSelNum,0); //SNA 128K 
 }
 //vTaskDelay(2);
}

//Menu ROM
void ShowTinyROMMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Machine ROM",gb_machine_menu,max_gb_machine_menu);
 if (aSelNum == 255)
  return;
 Z80EmuSelectTape(0);
 if (aSelNum == 0)
 {
  aSelNum = ShowTinyMenu("48K ROM",gb_list_roms_48k_title,max_list_rom_48);
  if (aSelNum == 255)
   return;  
  //strcpy(cfg_arch,"48K");  
  //cfg_arch = "48K";
  gb_cfg_arch_is48K = 1;
  load_rom2flash(1,aSelNum);  
 }
 else
 {
  aSelNum = ShowTinyMenu("128K ROM",gb_list_roms_128k_title,max_list_rom_128);        
  if (aSelNum == 255)
   return;  
  //strcpy(cfg_arch,"128K");
  //cfg_arch = "128K";
  gb_cfg_arch_is48K = 0;
  load_rom2flash(0,aSelNum);  
 }
 //vTaskDelay(2);
 #ifdef use_lib_core_jsanchezv
  ReloadLocalCacheROMram_jsanchezv();
  gb_ptrSim->ResetCPU();
  //gb_core_reset_jsanchezv= 1;//core jose luis
  zx_reset_jsanchezv();
 #else 
  //SDL_Delay(2);
  zx_reset();     
 #endif 
}

//Menu TAPE
void ShowTinyTAPEMenu()
{
 #ifdef use_lib_core_linkefong
  unsigned char aSelNum;
  aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48); 
  if (aSelNum == 255)
   return;
  load_tape2Flash(aSelNum);
 #endif 
}

//Play sound tape
void ShowTinySelectTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 if (aSelNum == 255)
  return;
 Z80EmuSelectTape(aSelNum);
}

//Menu SCREEN
void ShowTinySCRMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K SCREEN",gb_list_scr_48k_title,max_list_scr_48);
 if (aSelNum == 255)
  return;
 load_scr2Flash(aSelNum);
}

//Menu velocidad emulador
void ShowTinySpeedMenu()
{
 unsigned char aSelNum,aSelNumSpeed;
 aSelNum = ShowTinyMenu("SPEED VIDEO AUDIO",gb_speed_videoaudio_options_menu,max_gb_speed_videoaudio_options_menu);
 if (aSelNum == 255)
  return;
 switch (aSelNum)
 {
  case 0: aSelNumSpeed= ShowTinyMenu("AUDIO Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_sound= aSelNumSpeed;
   break;
  case 1: aSelNumSpeed= ShowTinyMenu("Video DELAY ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_delay_emulate_ms = aSelNumSpeed;
   break;
  case 2: //gb_current_frame_crt_skip = ShowTinyMenu("Skip Frame",gb_value_binary_menu,max_gb_value_binary_menu);
   aSelNumSpeed = ShowTinyMenu("Video SPEED",gb_speed_menu,max_gb_speed_menu);
   #ifdef use_lib_vga_thread
    switch (aSelNumSpeed)
    {
     case 0: gbFrameSkipVideoMaxCont = 0; break;
     case 1: gbFrameSkipVideoMaxCont = 1; break;
     case 2: gbFrameSkipVideoMaxCont = 2; break;
     case 3: gbFrameSkipVideoMaxCont = 4; break;
     case 4: gbFrameSkipVideoMaxCont = 8; break;
    }
   #else
    switch (aSelNumSpeed)
    {
     case 0: gbDelayVideo = 20; break;
     case 1: gbDelayVideo = 25; break;
     case 2: gbDelayVideo = 30; break;
     case 3: gbDelayVideo = 40; break;
     case 4: gbDelayVideo = 50; break;
    } 
   #endif
   break;
  case 3: aSelNumSpeed= ShowTinyMenu("Keyboard Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_keyboard= aSelNumSpeed;
   break;
  case 4: aSelNumSpeed= ShowTinyMenu("Delay CPU",gb_osd_delay_instructions,max_gb_osd_delay_instructions);
   if (aSelNumSpeed == 255)
    return;
   if (aSelNumSpeed==0)
   {//Auto delay
    gb_auto_delay_cpu=1;
   }
   else
   {    
    gb_auto_delay_cpu=0;
    if (aSelNumSpeed==1)
    {
     gb_delay_tick_cpu_micros=0;
    }
    else
    {
     //gb_delay_tick_cpu_micros = (1<<(aSelNumSpeed-2));
     gb_delay_tick_cpu_micros = (aSelNumSpeed-2);
    }    
   }
   /*
   switch (aSelNumSpeed)
   {
    case 0: gb_current_delay_emulate_div_microsec= 5; break;
    case 1: gb_current_delay_emulate_div_microsec= 2; break;
    case 2: gb_current_delay_emulate_div_microsec= 4; break;    
    case 3: gb_current_delay_emulate_div_microsec= 8; break;
    case 4: gb_current_delay_emulate_div_microsec= 16; break;    
    case 5: gb_current_delay_emulate_div_microsec= 32; break;
    case 6: gb_current_delay_emulate_div_microsec= 64; break;
    case 7: gb_current_delay_emulate_div_microsec= 0; break;
   }
   */
   break;   
  default: break;
 }
}

//Ajustar pantalla
void ShowTinyScreenAdjustMenu()
{
 unsigned char aSelNum, auxCol; 
 aSelNum= ShowTinyMenu("Screen Adjust",gb_osd_screen,max_gb_osd_screen);
 switch (aSelNum)
 { 
  case 2:
   //OSD_PreparaPaleta8Colores();
   memcpy(gb_cache_zxcolor,gb_const_colornormal8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_colornormal8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 3:
   //OSD_PreparaPaleta64para8Colores();
   memcpy(gb_cache_zxcolor,gb_const_monoBlue8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoBlue8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 4:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoGreen8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoGreen8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 5:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_monoRed8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_monoRed8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
  case 6:
   //OSD_PreparaPaleta64para8Colores();   
   memcpy(gb_cache_zxcolor,gb_const_grey8,8);
   #ifdef use_lib_use_bright
    memcpy(gb_cache_zxcolor_bright,gb_const_grey8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;          
 } 
 auxCol= ShowTinyMenu("Cols or pixels",gb_osd_screen_values,max_gb_osd_screen_values); 
 switch (aSelNum)
 {
  //case 0: gb_screen_xIni = -(auxCol<<3); break;
  //case 1: gb_screen_yIni = (auxCol<=3)?(-auxCol):(-3); break;    
  case 0: gb_screen_xIni = auxCol; break; //32 bits 4 pixels
  case 1:
   switch (auxCol)
   {
     case 0: gb_screen_yIni = 0; break;
     case 1: gb_screen_yIni = -1; break;
     case 2: gb_screen_yIni = -2; break;
     case 3: gb_screen_yIni = -3; break;
     default: break;
   }
   break;
  default: break;
 }
 //Serial.printf("gb_screen_yIni %d %d\n",gb_screen_yIni,auxCol);
 //Serial.printf("gb_screen_yIni %d \n",gb_screen_yIni); 
}


//Retorna indice de cada bloque y total de bloques
int LoadBlocksTape(unsigned char idTape,int *arrayTape)
{
 int aReturn= 0; 
 //int cont=3;
 //SearchTapeByte(idTape,0x13,&cont);
 //SearchTapeByte(idTape,0x00,&cont);
 arrayTape[aReturn]= 0x03; aReturn++; //HEAD
 arrayTape[aReturn]= 0x18; aReturn++; //Program BASIC
 //arrayTape[aReturn]= 0x127; aReturn++; //HEAD SCREEN
 //arrayTape[aReturn]= 0x13C; aReturn++; //SCREEN
 //arrayTape[aReturn]= 0x1C40; aReturn++; //HEAD binary
 //arrayTape[aReturn]= 0x1C55; aReturn++;
 //arrayTape[aReturn]= 0x2C55; aReturn++;
 return aReturn;
}


//Selecciona cinta actual
void Z80EmuSelectTape(unsigned char aux)
{
 int numBlocks;    
 gb_current_tape = aux;
 gb_tape_read = 0;
 gb_contTape = 3;
 char cadLog[200]="";
 numBlocks = LoadBlocksTape(aux,gb_local_arrayTape);
 gb_local_numBlocks = numBlocks;
 //sprintf(cadLog,"Tape %d  %d %d %d",numBlocks,gb_local_arrayTape[0],gb_local_arrayTape[1],gb_local_arrayTape[2]);
 //SDLprintText(SDLOSDGetSurface(),cadLog,5,450,WHITE,BLACK,0); 
}



//Menu resetear
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu);   
 if (aSelNum == 255)
  return; 
 Z80EmuSelectTape(0);
 #ifdef use_lib_core_linkefong
  zx_reset();
 #else
  #ifdef use_lib_core_jsanchezv
   zx_reset_jsanchezv();
  #endif 
 #endif

 #ifdef use_lib_mouse_kempston
  gb_z80_mouse_x=0;
  gb_z80_mouse_y=0;
 #endif 

 if (aSelNum == 1)
  ESP.restart();
}

//*********************************
void ShowTinyForceModeMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Force Mode",gb_mode48_128_menu,max_gb_mode48_128_menu);
 switch (aSelNum)
 {
   case 0: SetMode48K(); break;
   case 1: SetMode128K(); break;
 }
}


//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= true;   
}

//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
// SDLprintText("Ackerman Mod ZXESP32 Win32 Emulator",10,250,BLACK,WHITE);
 if (checkAndCleanKey(KEY_F1)) 
 {
  gb_show_osd_main_menu= 1;
  #ifdef use_lib_sound_ay8912
   //ResetSound();
   gb_silence_all_channels = 1;
  #endif  
  //gb_show_osd_main_menu= 1;
  return;
 }

 if (gb_show_osd_main_menu == 1)
 {
  #ifdef use_lib_vga_thread
   gb_draw_thread= 0;
   vTaskDelay(10);
  #endif

  //#ifdef use_lib_sound_ay8912
  // ResetSound();
  // gb_silence_all_channels = 1;
  //#endif 
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu);
  switch (aSelNum)
  {
   case 0: ShowTinySNAMenu(); break;
   case 1: ShowTinyROMMenu(); break;
   case 2: ShowTinyTAPEMenu(); break;
   case 3: ShowTinySelectTAPEMenu(); break;
   case 4: ShowTinySCRMenu(); break;
   case 5: ShowTinySpeedMenu(); break;
   case 6: ShowTinyScreenAdjustMenu(); break;
   case 7: ShowTinyForceModeMenu(); break;
   case 8:
    #ifdef use_lib_mouse_kempston
     ShowTinyMouseMenu(); 
    #endif 
    break;
   case 9: ShowTinySoundMenu(); break;
   case 10: ShowTinyResetMenu(); break;
   default: break;
  }
 }
 #ifdef use_lib_sound_ay8912
  //gb_silence_all_channels = 0;
  gb_silence_all_channels = gb_mute_sound;
 #endif

 #ifdef use_lib_vga_thread
  gb_draw_thread= 1;
 #endif 
}
