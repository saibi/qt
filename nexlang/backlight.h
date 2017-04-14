#ifndef BACKLIGHT_H
#define BACKLIGHT_H

class Backlight
{
public:
	Backlight();

	static int init();
	static int close();

	static int turnOn() {return control("1");}

	static int turnOff() {return control("0");}

	static int _fdBacklight;

protected:
	static int init_i();

	static int control(const char* controlString);
};

#endif // BACKLIGHT_H
