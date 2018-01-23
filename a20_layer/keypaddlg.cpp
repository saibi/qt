#include "keypaddlg.h"
#include "ui_keypaddlg.h"

#include "hw/beep.h"

#include <QGraphicsColorizeEffect>
#include <QKeyEvent>

KeypadDlg::KeypadDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KeypadDlg)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

	//
	QGraphicsColorizeEffect* pointColorizeEffect = new QGraphicsColorizeEffect(this);
	Q_CHECK_PTR(pointColorizeEffect);
	pointColorizeEffect->setColor(Qt::darkGray);
	pointColorizeEffect->setEnabled(false);

	ui->pushButton_point->setGraphicsEffect(pointColorizeEffect);

	//
	QGraphicsColorizeEffect* plusMinusColorizeEffect = new QGraphicsColorizeEffect(this);
	Q_CHECK_PTR(plusMinusColorizeEffect);
	plusMinusColorizeEffect->setColor(Qt::darkGray);
	plusMinusColorizeEffect->setEnabled(false);

	ui->pushButton_plusMinus->setGraphicsEffect(plusMinusColorizeEffect);

	//
	QGraphicsColorizeEffect* enterColorizeEffect = new QGraphicsColorizeEffect(this);
	Q_CHECK_PTR(enterColorizeEffect);
	enterColorizeEffect->setColor(Qt::darkGray);
	enterColorizeEffect->setEnabled(false);

	ui->pushButton_enter->setGraphicsEffect(enterColorizeEffect);

	//
	_mode = KM_INT;

	_precision = 1;

	_minIntValue = -1000;
	_maxIntValue = 1000;

	_minFloatValue = -1000.0;
	_maxFloatValue = 1000.0;

	_intStep = 1;
	_floatStep = 0.5;

	_intValue = 0;
	_floatValue = 0.0;

	connect(this, SIGNAL(accepted()), this, SLOT(slotAccepted()));
	connect(this, SIGNAL(rejected()), this, SLOT(slotRejected()));
	connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckNumber()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_pushButton_enter_clicked()));
}

KeypadDlg::~KeypadDlg()
{
	delete ui;
}

void KeypadDlg::readyForInput()
{
	switch (_mode)
	{
	case KM_INT:
	{
#if 0
		if (_intValue < _minIntValue)
			_intValue = _minIntValue;
		else if (_intValue > _maxIntValue)
			_intValue = _maxIntValue;
#endif
		_text = QString::number(_intValue);
		ui->lineEdit->setText(_text);

		ui->pushButton_point->setEnabled(false);
		ui->pushButton_point->graphicsEffect()->setEnabled(!ui->pushButton_point->isEnabled());

		if (_minIntValue < 0)
			ui->pushButton_plusMinus->setEnabled(true);
		else
			ui->pushButton_plusMinus->setEnabled(false);
		ui->pushButton_plusMinus->graphicsEffect()->setEnabled(!ui->pushButton_plusMinus->isEnabled());

		//
		ui->label_minMax->setText(QString("(%1 ~ %2)").arg(_minIntValue).arg(_maxIntValue));
	}
		break;

	case KM_FLOAT:
	{
#if 0
		if (_floatValue < _minFloatValue)
			_floatValue = _minFloatValue;
		else if (_floatValue > _maxFloatValue)
			_floatValue = _maxFloatValue;
#endif
		_text = QString::number(_floatValue, 'f', _precision);
		ui->lineEdit->setText(_text);

		ui->pushButton_point->setEnabled(true);
		ui->pushButton_point->graphicsEffect()->setEnabled(!ui->pushButton_point->isEnabled());

		if (_minFloatValue < 0)
			ui->pushButton_plusMinus->setEnabled(true);
		else
			ui->pushButton_plusMinus->setEnabled(false);
		ui->pushButton_plusMinus->graphicsEffect()->setEnabled(!ui->pushButton_plusMinus->isEnabled());

		//
		ui->label_minMax->setText(QString("(%1 ~ %2)").arg(QString::number(_minFloatValue, 'f', _precision)).arg(QString::number(_maxFloatValue, 'f', _precision)));
	}
		break;

	default:
		;
	}

	ui->lineEdit->selectAll();
}

void KeypadDlg::on_pushButton_0_clicked()
{
	Beep::beepClick();

	QString text = ui->lineEdit->text().trimmed();

	if (text == "0")
	{
		return;
	}

	ui->lineEdit->insert("0");
}

void KeypadDlg::on_pushButton_1_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("1");
}

void KeypadDlg::on_pushButton_2_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("2");
}

void KeypadDlg::on_pushButton_3_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("3");
}

void KeypadDlg::on_pushButton_4_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("4");
}

void KeypadDlg::on_pushButton_5_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("5");
}

void KeypadDlg::on_pushButton_6_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("6");
}

void KeypadDlg::on_pushButton_7_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("7");
}

void KeypadDlg::on_pushButton_8_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("8");
}

void KeypadDlg::on_pushButton_9_clicked()
{
	Beep::beepClick();

	if(isNumberAddableByPrecision())
		ui->lineEdit->insert("9");
}

void KeypadDlg::on_pushButton_plusMinus_clicked()
{
	Beep::beepClick();

	QString text = ui->lineEdit->text().trimmed();

	if (text.left(1) == "-")
	{
		// - -> +
		if (text.length() <= 1)
			text.clear();
		else
			text = text.mid(1);

		ui->lineEdit->setText(text);
		ui->lineEdit->selectAll();
	}
	else
	{
		// + -> -
		text = text.insert(0, QChar('-'));

		ui->lineEdit->setText(text);
		if (text.size() > 1)
		{
			ui->lineEdit->setSelection(1, text.size() - 1);
		}
	}
}

void KeypadDlg::on_pushButton_point_clicked()
{
	Beep::beepClick();

	QString text = ui->lineEdit->text().trimmed();

	if (!text.contains(QChar('.')))
	{
		ui->lineEdit->insert(".");
	}
}

void KeypadDlg::on_pushButton_back_clicked()
{
	Beep::beepClick();

	ui->lineEdit->backspace();
}

void KeypadDlg::on_pushButton_reset_clicked()
{
#if 0
	if (ui->lineEdit->text().trimmed().isEmpty())
	{
		this->reject();
	}
	else
	{
		Beep::beepClick();
		ui->lineEdit->clear();
	}
#else
	if (ui->lineEdit->text() == _text)
	{
		this->reject();
	}
	else
	{
		Beep::beepClick();
		ui->lineEdit->setText(_text);
		ui->lineEdit->selectAll();
	}
#endif
}

void KeypadDlg::on_pushButton_enter_clicked()
{
	if (ui->pushButton_enter->isEnabled())
	{
		const QString &text = ui->lineEdit->text().trimmed();

		if (text.isEmpty())
		{
			Beep::beepClick();
			return;
		}
		else
		{
			_text = text;
			this->accept();
		}
	}
}

void KeypadDlg::on_pushButton_increase_clicked()
{
	Beep::beepClick();

	QString text = ui->lineEdit->text().trimmed();

	switch (_mode)
	{
	case KM_INT:
	{
		int value = text.toInt();

		value += _intStep;

		if (value < _minIntValue)
			value = _minIntValue;
		if (value > _maxIntValue)
			value = _maxIntValue;

		ui->lineEdit->setText(QString::number(value));
	}
		break;

	case KM_FLOAT:
	{
		float value = text.toFloat();

		value += _floatStep;

		if (value < _minFloatValue)
			value = _minFloatValue;
		if (value > _maxFloatValue)
			value = _maxFloatValue;

		ui->lineEdit->setText(QString::number(value, 'f', _precision));
	}
		break;

	default:
		;
	}

	ui->lineEdit->selectAll();
}

void KeypadDlg::on_pushButton_decrease_clicked()
{
	Beep::beepClick();

	QString text = ui->lineEdit->text().trimmed();

	switch (_mode)
	{
	case KM_INT:
	{
		int value = text.toInt();

		value -= _intStep;

		if (value < _minIntValue)
			value = _minIntValue;
		if (value > _maxIntValue)
			value = _maxIntValue;

		ui->lineEdit->setText(QString::number(value));
	}
		break;

	case KM_FLOAT:
	{
		float value = text.toFloat();

		value -= _floatStep;

		if (value < _minFloatValue)
			value = _minFloatValue;
		if (value > _maxFloatValue)
			value = _maxFloatValue;

		ui->lineEdit->setText(QString::number(value, 'f', _precision));
	}
		break;

	default:
		;
	}

	ui->lineEdit->selectAll();
}

void KeypadDlg::slotAccepted()
{
	qDebug("[KeypadDlg::slotAccepted] [%s] <-- [%s]", this->windowTitle().toUtf8().constData(), ui->lineEdit->text().toLocal8Bit().constData());

	Beep::beepClick();
}

void KeypadDlg::slotRejected()
{
	qDebug("[KeypadDlg::slotRejected]");

	Beep::beepClick();
}

void KeypadDlg::slotCheckNumber()
{
	switch (_mode)
	{
	case KM_INT:
	{
		int value = ui->lineEdit->text().toInt();

		QPalette palette = ui->lineEdit->palette();

		if (value < _minIntValue)
		{
			ui->pushButton_enter->setEnabled(false);
			palette.setColor(QPalette::Text, Qt::red);
		}
		else if (value > _maxIntValue)
		{
			ui->pushButton_enter->setEnabled(false);
			palette.setColor(QPalette::Text, Qt::red);
		}
		else
		{
			ui->pushButton_enter->setEnabled(true);
			palette.setColor(QPalette::Text, Qt::black);
		}
		ui->pushButton_enter->graphicsEffect()->setEnabled(!ui->pushButton_enter->isEnabled());

		ui->lineEdit->setPalette(palette);
	}
		break;

	case KM_FLOAT:
	{
		float value = ui->lineEdit->text().toFloat();

		QPalette palette = ui->lineEdit->palette();

		if (value < _minFloatValue)
		{
			ui->pushButton_enter->setEnabled(false);
			palette.setColor(QPalette::Text, Qt::red);
		}
		else if (value > _maxFloatValue)
		{
			ui->pushButton_enter->setEnabled(false);
			palette.setColor(QPalette::Text, Qt::red);
		}
		else
		{
			ui->pushButton_enter->setEnabled(true);
			palette.setColor(QPalette::Text, Qt::black);
		}
		ui->pushButton_enter->graphicsEffect()->setEnabled(!ui->pushButton_enter->isEnabled());

		ui->lineEdit->setPalette(palette);
	}
		break;

	default:
		;
	}
}

bool KeypadDlg::isNumberAddableByPrecision()
{
	if(_mode == KM_FLOAT)
	{
		QString text = ui->lineEdit->text().trimmed();

		if(text.contains(QChar('.')))
		{
			if(!ui->lineEdit->hasSelectedText())
			{
				int point_pos = text.indexOf(QChar('.'));
				int cursor_pos =  ui->lineEdit->cursorPosition();

				// 소수점 앞자리 위치의 숫자 입력 허용
				if(cursor_pos <= point_pos)
					return true;

				QString decimals = text.section('.', 1, 1);

				// 소수점 이하 숫자 개수 확인
				if(decimals.size() >= _precision)
					return false;
			}
		}
	}

	return true;
}
