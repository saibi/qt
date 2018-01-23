#ifndef BEEP_H
#define BEEP_H

class Beep
{
public:
	Beep();
	~Beep();

	static int init();
	static int close();

	static void beep(unsigned int beepMsecs = 5, unsigned int muteMsecs = 0);

	static void beep(unsigned int beep1Msecs, unsigned int mute1Msecs,
					 unsigned int beep2Msecs, unsigned int mute2Msecs = 0);

	static void beep(unsigned int beep1Msecs, unsigned int mute1Msecs,
					 unsigned int beep2Msecs, unsigned int mute2Msecs,
					 unsigned int beep3Msecs, unsigned int mute3Msecs = 0);

	static void beepClick() {beep(15);}

	//
	static void beepInformation() {beep(100, 50, 100, 100);}

	static void beepQuestion() {beepInformation();}

	//
	static void beepNotification() {beep(300, 600, 300, 100);}

	//
	static void beepWarning() {beep(300, 50, 300, 100);}

	static void beepError() {beep(300, 50, 300, 50, 300, 100);}

	static void beepCritical() {beepWarning();beepWarning();beepWarning();}

	static int _fdBeep;

	//
	static void setEnabled(bool flag = true);

	static void setDisabled(bool flag = true) {setEnabled(!flag);}

	static bool _beepFlag;

	static void setMute(bool flag);

protected:
	static int init_i();
};

#endif // BEEP_H
