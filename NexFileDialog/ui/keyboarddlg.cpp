#include "keyboarddlg.h"
#include "ui_keyboarddlg.h"

#include "hw/beep.h"

KeyboardDlg::KeyboardDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KeyboardDlg)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

	connect(this, SIGNAL(accepted()), this, SLOT(slotAccepted()));
	connect(this, SIGNAL(rejected()), this, SLOT(slotRejected()));
	connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckInput()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_pushButton_enter_clicked()));
	m_reqMode = false;

	//ui->pushButton_req->hide(); old jobb request code

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
	ui->pushButton_0->setText("0");
	ui->pushButton_1->setText("1");
	ui->pushButton_2->setText("2");
	ui->pushButton_3->setText("3");
	ui->pushButton_4->setText("4");
	ui->pushButton_5->setText("5");
	ui->pushButton_6->setText("6");
	ui->pushButton_7->setText("7");
	ui->pushButton_8->setText("8");
	ui->pushButton_9->setText("9");
	ui->pushButton_a->setText("A");
	ui->pushButton_b->setText("B");
	ui->pushButton_c->setText("C");
	ui->pushButton_d->setText("D");
	ui->pushButton_e->setText("E");
	ui->pushButton_f->setText("F");
	ui->pushButton_g->setText("G");
	ui->pushButton_h->setText("H");
	ui->pushButton_i->setText("I");
	ui->pushButton_j->setText("J");
	ui->pushButton_k->setText("K");
	ui->pushButton_l->setText("L");
	ui->pushButton_m->setText("M");
	ui->pushButton_n->setText("N");
	ui->pushButton_o->setText("O");
	ui->pushButton_p->setText("P");
	ui->pushButton_q->setText("Q");
	ui->pushButton_r->setText("R");
	ui->pushButton_s->setText("S");
	ui->pushButton_t->setText("T");
	ui->pushButton_u->setText("U");
	ui->pushButton_v->setText("V");
	ui->pushButton_w->setText("W");
	ui->pushButton_x->setText("X");
	ui->pushButton_y->setText("Y");
	ui->pushButton_z->setText("Z");
	ui->pushButton_backspace->setText("Back");
	ui->pushButton_enter->setText("Enter");
	ui->pushButton_keyLeft->setText("<");
	ui->pushButton_keyRight->setText(">");
	ui->pushButton_point->setText(".");
	ui->pushButton_reset->setText("X");
	ui->pushButton_underline->setText("_");
#endif
}

KeyboardDlg::~KeyboardDlg()
{
	delete ui;
}

void KeyboardDlg::setText(const QString &text)
{
	ui->lineEdit->setText(text);
	ui->lineEdit->selectAll();
	_text = text;
}

QString KeyboardDlg::getText()
{
	return _text;
}

void KeyboardDlg::setMaxLength(int maxLength)
{
	ui->lineEdit->setMaxLength(maxLength);
}

void KeyboardDlg::setRequestMode(bool reqMode)
{
	m_reqMode = reqMode;
	m_reqText.clear();

	// old request code
#if 0
	if (reqMode)
		ui->pushButton_req->show();
	else
		ui->pushButton_req->hide();
#endif
}

QString KeyboardDlg::getReqText()
{
	return m_reqText;
}

void KeyboardDlg::on_pushButton_0_clicked() {Beep::beepClick(); ui->lineEdit->insert("0");}
void KeyboardDlg::on_pushButton_1_clicked() {Beep::beepClick(); ui->lineEdit->insert("1");}
void KeyboardDlg::on_pushButton_2_clicked() {Beep::beepClick(); ui->lineEdit->insert("2");}
void KeyboardDlg::on_pushButton_3_clicked() {Beep::beepClick(); ui->lineEdit->insert("3");}
void KeyboardDlg::on_pushButton_4_clicked() {Beep::beepClick(); ui->lineEdit->insert("4");}
void KeyboardDlg::on_pushButton_5_clicked() {Beep::beepClick(); ui->lineEdit->insert("5");}
void KeyboardDlg::on_pushButton_6_clicked() {Beep::beepClick(); ui->lineEdit->insert("6");}
void KeyboardDlg::on_pushButton_7_clicked() {Beep::beepClick(); ui->lineEdit->insert("7");}
void KeyboardDlg::on_pushButton_8_clicked() {Beep::beepClick(); ui->lineEdit->insert("8");}
void KeyboardDlg::on_pushButton_9_clicked() {Beep::beepClick(); ui->lineEdit->insert("9");}
void KeyboardDlg::on_pushButton_a_clicked() {Beep::beepClick(); ui->lineEdit->insert("A");}
void KeyboardDlg::on_pushButton_b_clicked() {Beep::beepClick(); ui->lineEdit->insert("B");}
void KeyboardDlg::on_pushButton_c_clicked() {Beep::beepClick(); ui->lineEdit->insert("C");}
void KeyboardDlg::on_pushButton_d_clicked() {Beep::beepClick(); ui->lineEdit->insert("D");}
void KeyboardDlg::on_pushButton_e_clicked() {Beep::beepClick(); ui->lineEdit->insert("E");}
void KeyboardDlg::on_pushButton_f_clicked() {Beep::beepClick(); ui->lineEdit->insert("F");}
void KeyboardDlg::on_pushButton_g_clicked() {Beep::beepClick(); ui->lineEdit->insert("G");}
void KeyboardDlg::on_pushButton_h_clicked() {Beep::beepClick(); ui->lineEdit->insert("H");}
void KeyboardDlg::on_pushButton_i_clicked() {Beep::beepClick(); ui->lineEdit->insert("I");}
void KeyboardDlg::on_pushButton_j_clicked() {Beep::beepClick(); ui->lineEdit->insert("J");}
void KeyboardDlg::on_pushButton_k_clicked() {Beep::beepClick(); ui->lineEdit->insert("K");}
void KeyboardDlg::on_pushButton_l_clicked() {Beep::beepClick(); ui->lineEdit->insert("L");}
void KeyboardDlg::on_pushButton_m_clicked() {Beep::beepClick(); ui->lineEdit->insert("M");}
void KeyboardDlg::on_pushButton_n_clicked() {Beep::beepClick(); ui->lineEdit->insert("N");}
void KeyboardDlg::on_pushButton_o_clicked() {Beep::beepClick(); ui->lineEdit->insert("O");}
void KeyboardDlg::on_pushButton_p_clicked() {Beep::beepClick(); ui->lineEdit->insert("P");}
void KeyboardDlg::on_pushButton_q_clicked() {Beep::beepClick(); ui->lineEdit->insert("Q");}
void KeyboardDlg::on_pushButton_r_clicked() {Beep::beepClick(); ui->lineEdit->insert("R");}
void KeyboardDlg::on_pushButton_s_clicked() {Beep::beepClick(); ui->lineEdit->insert("S");}
void KeyboardDlg::on_pushButton_t_clicked() {Beep::beepClick(); ui->lineEdit->insert("T");}
void KeyboardDlg::on_pushButton_u_clicked() {Beep::beepClick(); ui->lineEdit->insert("U");}
void KeyboardDlg::on_pushButton_v_clicked() {Beep::beepClick(); ui->lineEdit->insert("V");}
void KeyboardDlg::on_pushButton_w_clicked() {Beep::beepClick(); ui->lineEdit->insert("W");}
void KeyboardDlg::on_pushButton_x_clicked() {Beep::beepClick(); ui->lineEdit->insert("X");}
void KeyboardDlg::on_pushButton_y_clicked() {Beep::beepClick(); ui->lineEdit->insert("Y");}
void KeyboardDlg::on_pushButton_z_clicked() {Beep::beepClick(); ui->lineEdit->insert("Z");}

void KeyboardDlg::on_pushButton_point_clicked()
{
	Beep::beepClick(); ui->lineEdit->insert(".");
}

void KeyboardDlg::on_pushButton_underline_clicked()
{
	Beep::beepClick(); ui->lineEdit->insert("_");
}

void KeyboardDlg::on_pushButton_backspace_clicked()
{
	Beep::beepClick(); ui->lineEdit->backspace();
}

#if 0
void KeyboardDlg::on_pushButton_delete_clicked()
{
	Beep::beepClick(); ui->lineEdit->del();
}
#endif

void KeyboardDlg::on_pushButton_keyLeft_clicked()
{
	Beep::beepClick(); ui->lineEdit->cursorBackward(false);
}

void KeyboardDlg::on_pushButton_keyRight_clicked()
{
	Beep::beepClick(); ui->lineEdit->cursorForward(false);
}

void KeyboardDlg::on_pushButton_reset_clicked()
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

void KeyboardDlg::on_pushButton_enter_clicked()
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

void KeyboardDlg::on_pushButton_enter_2_clicked()
{
	on_pushButton_enter_clicked();
}

void KeyboardDlg::slotAccepted()
{
	qDebug("[KeyboardDlg::slotAccepted] [%s]", ui->lineEdit->text().toLocal8Bit().constData());

	Beep::beepClick();
}

void KeyboardDlg::slotRejected()
{
	qDebug("[KeyboardDlg::slotRejected]");

	Beep::beepClick();
}

void KeyboardDlg::slotCheckInput()
{
	//qDebug("[KeyboardDlg::slotCheckInput] [%s]", ui->lineEdit->text().toLocal8Bit().constData());
}

void KeyboardDlg::slotBarcode(QString barcode)
{
	this->setText(barcode);
}

void KeyboardDlg::on_pushButton_enter_pressed()
{
	ui->pushButton_enter_2->setDown(true);
}

void KeyboardDlg::on_pushButton_enter_released()
{
	ui->pushButton_enter_2->setDown(false);
}

void KeyboardDlg::on_pushButton_enter_2_pressed()
{
	ui->pushButton_enter->setDown(true);
}

void KeyboardDlg::on_pushButton_enter_2_released()
{
	ui->pushButton_enter->setDown(false);
}

// old request code
#if 0
void KeyboardDlg::on_pushButton_req_clicked()
{
	const QString &text = ui->lineEdit->text().trimmed();

	if (text.isEmpty())
	{
		Beep::beepClick();
		return;
	}
	else
	{
		m_reqText = text;
		this->reject();
	}
}
#endif
