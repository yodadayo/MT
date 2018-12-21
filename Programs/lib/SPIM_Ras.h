#ifndef SPIM_Ras_h
#define SPIM_Ras_h

class SPIM_Ras
{
public:
	// SPI Configuration methods
	SPIM_Ras(void);
	short initialization(int speed,int speed2 = 2000000);
	short begin(char pin);
	uint16_t transfer(char pin, uint16_t data);
	uint16_t transferEncoder1(char pin, uint16_t data);
	uint64_t transferEncoder3(char pin, uint64_t data);

	uint16_t transferArm(char pin, uint16_t data);

private:
};

extern SPIM_Ras SPIR;
#endif
