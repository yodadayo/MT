#ifndef SlaveControlCommand_h
#define SlaveControlCommand_h

class SlaveControlCommand
{
public:
	// SPI Configuration methods
	SlaveControlCommand(void);

	uint16_t transfer_volume(uint8_t SSpin,uint8_t pin ,uint16_t vol );
	void transfer_volume_all(uint8_t SSpin);
	void transfer_volume_all_analog(uint8_t SSpin);
	void getDataFromSSAB(uint8_t SSpin, uint16_t *data);
  	int getDataFromGyro(uint8_t SSpin, uint16_t *data);
  	void getDataFromEncoder1(uint8_t SSpin, uint16_t *data);
  	void getDataFromEncoder3(uint8_t SSpin, uint16_t *data);

private:
};

extern SlaveControlCommand SCC;
#endif
