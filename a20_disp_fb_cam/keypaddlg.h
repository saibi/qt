#ifndef KEYPADDLG_H
#define KEYPADDLG_H

#include <QDialog>

namespace Ui {
	class KeypadDlg;
}

class KeypadDlg : public QDialog
{
	Q_OBJECT

public:
	explicit KeypadDlg(QWidget *parent = 0);
	~KeypadDlg();

	// mode
	enum KeypadMode
	{
		KM_INT = 1,
		KM_FLOAT = 2,
	};

	void setKeypadMode(int mode)
	{
		_mode = mode;
	}

	int getKeypadMode()
	{
		return _mode;
	}

	// precision
	void setPrecision(int precision)
	{
		_precision = precision;
	}

	// min, max
	void setIntMinMax(int min, int max)
	{
		if (min < max)
		{
			_minIntValue = min;
			_maxIntValue = max;
		}
		else
		{
			_minIntValue = max;
			_maxIntValue = min;
		}
		readyForInput();
		slotCheckNumber();
	}

	void setFloatMinMax(float min, float max)
	{
		if (min < max)
		{
			_minFloatValue = min;
			_maxFloatValue = max;
		}
		else
		{
			_minFloatValue = max;
			_maxFloatValue = min;
		}
		readyForInput();
		slotCheckNumber();
	}

	// step
	void setIntStep(int step)
	{
		_intStep = step;
	}

	void setFloatStep(float step)
	{
		_floatStep = step;
	}

	// value
	QString getText()
	{
		return _text;
	}

	void setIntValue(int value)
	{
		setKeypadMode(KM_INT);
		_intValue = value;
		readyForInput();
		slotCheckNumber();
	}

	int getIntValue()
	{
		return _text.toInt();
	}

	void setFloatValue(float value)
	{
		setKeypadMode(KM_FLOAT);
		_floatValue = value;
		readyForInput();
		slotCheckNumber();
	}

	float getFloatValue()
	{
		return _text.toFloat();
	}

protected:
	// ready
	void readyForInput();

	bool isNumberAddableByPrecision();

private slots:
	void on_pushButton_0_clicked();

	void on_pushButton_1_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_5_clicked();

	void on_pushButton_6_clicked();

	void on_pushButton_7_clicked();

	void on_pushButton_8_clicked();

	void on_pushButton_9_clicked();

	void on_pushButton_plusMinus_clicked();

	void on_pushButton_point_clicked();

	void on_pushButton_back_clicked();

	void on_pushButton_reset_clicked();

	void on_pushButton_enter_clicked();

	void on_pushButton_increase_clicked();

	void on_pushButton_decrease_clicked();

	void slotAccepted();

	void slotRejected();

	void slotCheckNumber();

private:
	Ui::KeypadDlg *ui;

	// mode
	int _mode;

	// precision
	int _precision;

	// min, max
	int _minIntValue;
	int _maxIntValue;

	float _minFloatValue;
	float _maxFloatValue;

	// step
	int _intStep;

	float _floatStep;

	// value
	QString _text;

	int _intValue;

	float _floatValue;
};

#endif // KEYPADDLG_H
