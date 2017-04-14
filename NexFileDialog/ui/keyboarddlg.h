#ifndef KEYBOARDDLG_H
#define KEYBOARDDLG_H

#include <QDialog>

namespace Ui {
	class KeyboardDlg;
}

class KeyboardDlg : public QDialog
{
	Q_OBJECT

public:
	explicit KeyboardDlg(QWidget *parent = 0);
	~KeyboardDlg();

	void setText(const QString &text);

	QString getText();

	void setMaxLength(int maxLength);

	void setRequestMode(bool reqMode = true);

	QString getReqText();

protected:

public slots:
	void slotBarcode(QString barcode);

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
	void on_pushButton_a_clicked();
	void on_pushButton_b_clicked();
	void on_pushButton_c_clicked();
	void on_pushButton_d_clicked();
	void on_pushButton_e_clicked();
	void on_pushButton_f_clicked();
	void on_pushButton_g_clicked();
	void on_pushButton_h_clicked();
	void on_pushButton_i_clicked();
	void on_pushButton_j_clicked();
	void on_pushButton_k_clicked();
	void on_pushButton_l_clicked();
	void on_pushButton_m_clicked();
	void on_pushButton_n_clicked();
	void on_pushButton_o_clicked();
	void on_pushButton_p_clicked();
	void on_pushButton_q_clicked();
	void on_pushButton_r_clicked();
	void on_pushButton_s_clicked();
	void on_pushButton_t_clicked();
	void on_pushButton_u_clicked();
	void on_pushButton_v_clicked();
	void on_pushButton_w_clicked();
	void on_pushButton_x_clicked();
	void on_pushButton_y_clicked();
	void on_pushButton_z_clicked();

	void on_pushButton_point_clicked();
	void on_pushButton_underline_clicked();
	void on_pushButton_backspace_clicked();
    //void on_pushButton_delete_clicked();
	void on_pushButton_keyLeft_clicked();
	void on_pushButton_keyRight_clicked();
	void on_pushButton_reset_clicked();
	void on_pushButton_enter_clicked();
	void on_pushButton_enter_2_clicked();

	void slotAccepted();

	void slotRejected();

	void slotCheckInput();

	void on_pushButton_enter_pressed();

	void on_pushButton_enter_released();

	void on_pushButton_enter_2_pressed();

	void on_pushButton_enter_2_released();

//	void on_pushButton_req_clicked(); old request code

private:
	Ui::KeyboardDlg *ui;

	QString _text;

	bool m_reqMode;

	QString m_reqText;
};

#endif // KEYBOARDDLG_H
