/*****************************************************************//**
 * @file main_sampler_test.cpp
 *
 * @brief Basic test of nexys4 ddr mmio cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "vga_core.h"
/**
 * blink once per second for 5 times.
 * provide a sanity check for timer (based on SYS_CLK_FREQ)
 * @param led_p pointer to led instance
 */
void ps2_check(Ps2Core *ps2_p) {
   int id;
   int lbtn, rbtn, xmov, ymov;
   char ch;
   unsigned long last;

   uart.disp("\n\rPS2 device (1-keyboard / 2-mouse): ");
   id = ps2_p->init();
   uart.disp(id);
   uart.disp("\n\r");
   last = now_ms();
   do {
      if (id == 2) {  // mouse
         if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov)) {
            uart.disp("[");
            uart.disp(lbtn);
            uart.disp(", ");
            uart.disp(rbtn);
            uart.disp(", ");
            uart.disp(xmov);
            uart.disp(", ");
            uart.disp(ymov);
            uart.disp("] \r\n");
            last = now_ms();

         }   // end get_mouse_activitiy()
      } else {
         if (ps2_p->get_kb_ch(&ch)) {
            uart.disp(ch);
            uart.disp(" ");
            last = now_ms();
         } // end get_kb_ch()
      }  // end id==2
   } while (now_ms() - last < 5000);
   uart.disp("\n\rExit PS2 test \n\r");

}
void plot_border(FrameCore *frame){
	frame->plot_line(150, 410, 500, 410, 4);    // bottom x wall
	frame->plot_line(150, 145, 150, 410, 4);    // left y wall
	frame->plot_line(500, 115, 500, 410, 4);	   // right y wall
	frame->plot_line(150, 115, 500, 115, 4);    // top x wall
}
void plot_walls(FrameCore *frame){
	//box lines
	frame->plot_line(345, 285, 375, 285, 4); //right x line
	frame->plot_line(275, 285, 305, 285, 4); //left x line
	frame->plot_line(275, 235, 275, 285, 4); //y line
	frame->plot_line(375, 235, 375, 285, 4); //y line
	frame->plot_line(275, 235, 375, 235, 4); //top x line
	frame->plot_line(305, 285, 305, 315, 4); //y line extension
	frame->plot_line(375, 245, 405, 245, 4); //x line extension
	//second box lines
	frame->plot_line(245, 315, 405, 315, 4); // bottom x line
	frame->plot_line(245, 205, 245, 315, 4); // left y line
	frame->plot_line(245, 205, 275, 205, 4); //top left x line
	frame->plot_line(405, 275, 405, 315, 4); // bottom right y line
	frame->plot_line(405, 205, 405, 245, 4); // top right y line
	frame->plot_line(405, 275, 435, 275, 4); //bottom right x line
	frame->plot_line(335, 205, 405, 205, 4); // top right x line
	//third box lines
	frame->plot_line(215, 175, 435, 175, 4); //top x line
	frame->plot_line(215, 175, 215, 345, 4); //left y line
	frame->plot_line(435, 175, 435, 345, 4); //right y line
	frame->plot_line(215, 345, 255, 345, 4); //bottom left x
	frame->plot_line(285, 345, 345, 345, 4); //bottom middle x
	frame->plot_line(385, 345, 435, 345, 4); //bottom right x
	frame->plot_line(285, 315, 285, 345, 4); //middle bottom y line
	//fourth box lines
	frame->plot_line(185, 145, 185, 255, 4); //top left y wall
	frame->plot_line(185, 285, 185, 375, 4); //bottom left y wall
	frame->plot_line(185, 145, 305, 145, 4); //top left x wall
	frame->plot_line(305, 145, 465, 145, 4); //top right x wall
	frame->plot_line(465, 145, 465, 205, 4); //top right y wall
	frame->plot_line(465, 305, 465, 375, 4); //bottom right y wall
	frame->plot_line(185, 375, 465, 375, 4); //bottom x wall
	frame->plot_line(150, 255, 185, 255, 4); //left x extension
	frame->plot_line(465, 275, 500, 275, 4); //right x extension
	frame->plot_line(465, 235, 465, 275, 4); //right y extension
}
void init_osd(OsdCore *osd_p) {
   osd_p->set_color(0x0f0, 0x001); // dark gray/green
   osd_p->bypass(0);
   osd_p->clr_screen();

   osd_p->wr_char(28, 20, 'T');
   sleep_ms(100);
   osd_p->wr_char(30, 20, 'H');
   sleep_ms(100);
   osd_p->wr_char(32, 20, 'E');
   sleep_ms(100);
   osd_p->wr_char(34, 20, ' ');
   sleep_ms(100);
   osd_p->wr_char(36, 20, 'M');
   sleep_ms(100);
   osd_p->wr_char(38, 20, 'A');
   sleep_ms(100);
   osd_p->wr_char(40, 20, 'Z');
   sleep_ms(100);
   osd_p->wr_char(42, 20, 'E');
   sleep_ms(100);
   osd_p->wr_char(44, 20, ' ');
   sleep_ms(100);
   osd_p->wr_char(46, 20, 'G');
   sleep_ms(100);
   osd_p->wr_char(48, 20, 'A');
   sleep_ms(100);
   osd_p->wr_char(50, 20, 'M');
   sleep_ms(100);
   osd_p->wr_char(52, 20, 'E');
   sleep_ms(5000);
}
void game_end(OsdCore *osd_p){
	osd_p->wr_char(24, 20, 'Y');
	sleep_ms(100);
	osd_p->wr_char(26, 20, 'O');
	sleep_ms(100);
	osd_p->wr_char(28, 20, 'U');
	sleep_ms(100);
	osd_p->wr_char(30, 20, ' ');
	sleep_ms(100);
	osd_p->wr_char(32, 20, 'B');
	sleep_ms(100);
	osd_p->wr_char(34, 20, 'E');
	sleep_ms(100);
	osd_p->wr_char(36, 20, 'A');
	sleep_ms(100);
	osd_p->wr_char(38, 20, 'T');
	sleep_ms(100);
	osd_p->wr_char(40, 20, ' ');
	sleep_ms(100);
	osd_p->wr_char(42, 20, 'T');
	sleep_ms(100);
	osd_p->wr_char(44, 20, 'H');
	sleep_ms(100);
	osd_p->wr_char(46, 20, 'E');
	sleep_ms(100);
	osd_p->wr_char(48, 20, ' ');
	sleep_ms(100);
	osd_p->wr_char(50, 20, 'M');
	sleep_ms(100);
	osd_p->wr_char(52, 20, 'A');
	sleep_ms(100);
	osd_p->wr_char(54, 20, 'Z');
	sleep_ms(100);
	osd_p->wr_char(56, 20, 'E');
	sleep_ms(100);
	osd_p->wr_char(58, 20, '!');
	sleep_ms(100);
}

GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
I2cCore adt7420(get_slot_addr(BRIDGE_BASE, S10_I2C));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);
FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
//SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));

void init_game(FrameCore *frame, SpriteCore *ghost){
	plot_border(frame);
	plot_walls(frame);
	ghost->move_xy(315, 250);
}
int main() {
   //uint8_t id, ;
	int MAX_HEIGHT = 115;
	int MIN_HEIGHT = 398;
	int MAX_WIDTH = 485;
	int MIN_WIDTH = 150;
    char ch;
    init_game(&frame, &ghost); // Initialize Character
    init_osd(&osd);
	ps2.init();
	//plot_line(int x0, int y0, int x1, int y1, int color)

	int x = 315;
	int y = 250;
	while(!ps2.get_kb_ch(&ch)){
		sleep_ms(100);
	}
	//Game Start
	while(1) {
		osd.clr_screen();
		//first box
		if((y == 270 ||  y == 282) && ((x > 275 && x < 305) || (x > 345 && x < 375))){ //bottom left y wall
			if(y == 270){
				y = y - 2;
			}
			if(y == 282){
				y = y + 2;
			}
		}


		//second box walls
		else if(x == 395 && ((y > 205 && y < 245) || (y > 275 && y < 315))){ //top right y wall entering from the left
			if(x == 395){
				x = x - 2;
			}
		}
		else if(x == 405 && ((y > 205 && y < 245) || (y > 275 && y < 315))){ //top right y wall entering from the right
			if(x == 405){
				x = x + 2;
			}
		}
		else if((y == 244 || y == 246) && (x > 375 && x < 405)){ //x extension from first box
			if(y == 246){ //entering from the bottom
				y = y + 2;
			}
			if(y == 244){
				y = y - 2; //entering from the top
			}

		}
		else if((y == 193 || y == 206) && ((x > 245 && x < 275) || (x > 335 && x < 405))){ // top x wall
			if(y == 193){ //entering from the top
				y = y - 2;
			}
			if(y == 206){ // entering from the bottom
				y = y + 2;
			}
		}
		else if((x == 304 || x == 306) && (y > 285 && y < 315)){ //y extension from first box
			if(x == 306){ //entering from the right
				x = x + 2;
			}
			if(x == 300){ //entering from the left
				x = x - 2;
			}
		}
		else if((y == 263 || y == 276) && (x > 405 && x < 435)){ //x extension from second box
			if(y == 263){ //entering from the top
				y = y - 2;
			}
			if(y == 276){ //entering from the bottom
				y = y + 2;
			}
		}
		else if((x == 235 || x == 246) && (y > 205 && y < 315)){ //left wall
			if(x == 235){ //entering from the left
				x = x - 2;
			}
			if(x == 246){ // entering from the right
				x = x + 2;
			}
		}
		else if((x == 425|| x == 435) && (y > 175 && y < 345)){ //right wall
			if(x == 425){ //entering from the left
				x = x - 2;
			}
			if(x == 435){ // entering from the right
				x = x + 2;
			}
		}
		else if((y == 305 || y == 316) && (x > 245 && x < 405)){ // bottom wall
			if(y == 316){ //entering from the bottom
				y = y + 2;
			}
			if(y == 305){ //entering from the top
				y = y - 2;
			}
		}
		//third walls
		else if((x == 203 || x == 216) && (y > 175 && y < 345)){ // third box left y wall
			if(x == 203){ //entering from the left
				x = x - 2;
			}
			if(x == 216){ //entering from the right
				x = x + 2;
			}
		}
		else if((y == 332 || y == 346) && ((x > 215 && x < 255) || (x > 285 && x < 345) || (x > 385 && x < 435))){ //bottom y walls
			if(y == 332){ //entering from the top
				y = y - 2;
			}
			if(y == 346){ //entering from the bottom
				y = y + 2;
			}
		}
		else if((x == 286 || x == 273) && (y > 315 && y < 345)){ //y extension
			if(x == 286){
				x = x + 2;
			}
			if(x == 273){
				x = x - 2;
			}
		}
		else if((y == 163 || y == 176) && (x > 215 && x < 435)){ // top x wall
			if(y == 163){ // entering from the top
				y = y - 2;
			}
			if(y == 176){ // entering from the bottom
				y = y + 2;
			}
		}
		//fourth walls
		else if((y == 135 || y == 146) && (x > 185 && x < 465)){ //top wall
			if(y == 135){
				y = y - 2;
			}
			if(y == 146){
				y = y + 2;
			}
		}
		else if((y == 365 || y == 376) && (x > 185 && x < 465)){ //top wall
			if(y == 365){
				y = y - 2;
			}
			if(y == 376){
				y = y + 2;
			}
		}
		else if((x == 173 || x == 186) && ((y > 145 && y < 255) || (y > 285 && y < 375))){ // top and bottom left y wall
			if(x == 186){ //entering form the right
				x = x + 2;
			}
			if(x == 173){ //entering from the left
				x = x - 2;
			}
		}
		else if((x == 453 || x == 466) && ((y > 145 && y < 205) || (y > 305 && y < 375) || (y > 235 && y < 275))){ // right y wall
			if(x == 453){ // entering from the left
				x = x - 2;
			}
			if(x == 466){ // entering from the right
				x = x + 2;
			}
		}
		else if((y == 256 || y == 243) && (x > 150 && x < 185)){ // left x extension
			if(y == 256){ //entering from the bottom
				y = y + 2;
			}
			if(y == 243){ // entering from the top
				y = y - 2;
			}
		}

		//small box walls
		else if((x == 276 || x == 372) && (y > 235 && y < 285)){	//entering from the right
			if(x == 276){
				x = x + 2;
			}
			if(x == 372){
				x = x + 2;
			}
		}
		else if((x == 263 || x == 365) && (y > 235 && y < 285)){	//entering from the left
			if(x == 263){
				x = x - 2;
			}
			if(x == 365){
				x = x - 2;
			}
		}
		else if((y == 235 || y == 225) && (x > 275 && x < 365)){ //entering from the bottom and top
			if(y == 235){ //entering from the bottom
				y = y + 2;
			}
			if(y == 225){ //entering from the top
				y = y - 2;
			}
		}
		else if((y == 276 || y == 263) && (x > 465 && x < 500)){ //right x extension
			if(y == 276){ // entering from the bottom
				y = y + 2;
			}
			if(y == 263){ // entering from the top
				y = y - 2;
			}
		}
		else if(x == 151 && (y > 145 && y < 410)){
			x = x + 2;
			uart.disp("THIS HAPPENSKJLDFJLK:FDLJKFDJ");
		}


		else if(x == MAX_WIDTH || y == MAX_HEIGHT || y == MIN_HEIGHT){
			if(x == MAX_WIDTH){
				x = MAX_WIDTH - 2;
			}
			if(y == MIN_HEIGHT){
				y = MIN_HEIGHT - 2;
			}
			if(y == MAX_HEIGHT){
				y = MAX_HEIGHT + 2;
			}
		}
		else{
			if (ps2.get_kb_ch(&ch)) {
				uart.disp(ch);
				uart.disp(" ");
			} // end get_kb_ch()
			if(ch == 'd'){
				x = x + 1;
				sleep_ms(15);
			}
			if(ch == 'a'){
				x = x - 1;
				sleep_ms(15);
			}
			if(ch == 'w'){
				y = y - 1;
				sleep_ms(15);
			}
			if(ch == 's'){
				y = y + 1;
				sleep_ms(15);
			}
		}
		ghost.move_xy(x, y);
		sleep_ms(15);
		uart.disp(x);
		uart.disp("\n");
		uart.disp(y);
		uart.disp("\n");
		if(x < 145){
			game_end(&osd);
			sleep_ms(8000);
			x = 315;
			y = 250;
			osd.clr_screen();
			ghost.move_xy(x, y);
		}
	} // while




} //main

