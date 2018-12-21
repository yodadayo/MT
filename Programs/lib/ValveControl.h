#ifndef ValveControl_h
#define ValveControl_h
#include "arduino.h"

class ValveControl
{
  public:
	// SPI Configuration methods
	ValveControl(void);
	void valve_init(void);
	void initialization_slave_board(void);
	void pluse_control(uint16_t pin,uint16_t rate,uint16_t vnum);// Using for pluse control in valve
	uint32_t pressure_adjust(uint32_t vnum,  int32_t targetVol ,int32_t range);//Retuern the analog value
	uint32_t pressure_adjust_using_pluse(uint32_t vnum,  int32_t targetVol ,int32_t range);
	void pressure_adjust_all(void);
	void pressure_adjust_using_pluse_all(void);

	void all_exhaust(void);
	void all_supply(void);
	void all_close(void);

	void printall_get_data(void);
	void printall_analog(void);

  	private:

};

extern ValveControl VC;
#endif
