#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

bool seta    = false;
bool setaBC  = false;
bool setaRO  = false;
bool setaRC  = false;
bool setaLO  = false;
bool setaLC  = false;

bool alerta  = false;
bool alertaO = false;
bool alertaC = false;

bool farol   = false;
bool farolO  = false;
bool farolC  = false;

bool blink   = false;
int  counter = 0;

int  buttonR = 3;
int  buttonL = 4;

int  barF    = 0;
bool selectF = false;

int  valueF  = 7;
bool eventF  = true;

bool loading = true;

void setup() {
	pinMode(buttonR, INPUT_PULLUP);
	pinMode(buttonL, INPUT_PULLUP);

	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10,OUTPUT);
	pinMode(11,OUTPUT);
}

void loop() {
	if (loading) {
		start();
	} else {
		menu();
	}
}

void menu() {

	u8g.firstPage();

	do {

		u8g.setFont(u8g_font_courB10);

		u8g.drawStr  ( 44, 20, "SETA"  );
		u8g.drawStr  ( 36, 39, "ALERTA");
		u8g.drawStr  ( 40, 58, "FAROL" );

		u8g.drawFrame(  0,  0,128,  6);

		u8g.drawFrame(  0,  7, 18, 18);
		u8g.drawFrame(110,  7, 18, 18);

		u8g.drawFrame(  0, 26, 18, 18);
		u8g.drawFrame(110, 26, 18, 18);

		u8g.drawFrame(  0, 45, 18, 18);
		u8g.drawFrame(110, 45, 18, 18);

		u8g.drawFrame( 19, valueF, 90, 18);

		if (digitalRead(buttonR) == LOW) {
			eventF = true;
		}

		if (digitalRead(buttonR) == HIGH && eventF && !selectF) {
			if (valueF < 45) {
				valueF += 19;
			}
			else {
				valueF = 7;
			}

			eventF = false;
		}

		if (digitalRead(buttonL) == LOW) {
			barF = -50;
		}

		if (digitalRead(buttonL) == HIGH) {
			barF++;
		}

		if (barF > 0 && barF < 123 && !selectF) {
			u8g.drawBox( 2, 2, barF, 2);
		}

		if (barF > 0 && barF < 123 && selectF) {
			u8g.drawBox( 2, 2, 123 - barF, 2);
		}

		if (selectF && barF < 0) {
			u8g.drawBox( 2, 2, 124, 2);
		}

		if (barF > 123) {
			if (selectF) {
				u8g.drawBox( 2, 2, 124, 2);
			}
		}

		if (barF == 123) {
			if (selectF) {
				selectF = false;
			}
			else {
				selectF = true;
			}
		}

		if (selectF && valueF == 7) {
			if (digitalRead(buttonL) == LOW) {
				setaLO = true;
			}

			if (digitalRead(buttonL) == HIGH && setaLO == true) {
				setaLO = false;
				if (setaLC) {
					setaLC  = false;
					setaBC  = false;
				}
				else {
					setaLC  = true;
					setaRC  = false;
					alertaC = false;
				}
			}

			if (digitalRead(buttonR) == LOW) {
				setaRO = true;
			}

			if (digitalRead(buttonR) == HIGH && setaRO == true) {
				setaRO = false;
				if (setaRC) {
					setaRC  = false;
					setaBC  = false;
				}
				else {
					setaRC  = true;
					setaBC  = true;
					setaLC  = false;
					alertaC = false;
				}
			}

			if (barF == -10) {
				if (setaLC) {
					setaLC = false;
					if (setaBC) {
						setaRC = true;
					}
				}
				else {
					setaLC = true;
					setaRC = false;
				}
			}
		}

		if (setaLC && (barF == -50 || barF > -10)) {
			if (counter >= 0 && counter < 5) {
				u8g.drawBox  (  2,  9, 14, 14);
			}

			if (blink) {
				analogWrite(5, 255);
				analogWrite(6, 255);
				analogWrite(7, 0);
			}
		}

		if (setaRC && (barF == -50 || barF > -10)) {
			if (counter >= 0 && counter < 5) {
				u8g.drawBox  (112,  9, 14, 14);
			}

			if (blink) {
				analogWrite(8, 255);
				analogWrite(9, 255);
				analogWrite(10,0);
			}
		}

		if (selectF && valueF == 26) {
			if (digitalRead(buttonR) == LOW) {
				alertaO = true;
			}

			if (digitalRead(buttonR) == HIGH && alertaO == true) {
				alertaO = false;
				if (alertaC) {
					alertaC = false;
				}
				else {
					alertaC = true;
					setaRC  = false;
					setaLC  = false;
				}
			}
		}

		if (alertaC) {
			if (counter >= 0 && counter < 5) {
				u8g.drawBox  (  2, 28, 14, 14);
				u8g.drawBox  (112, 28, 14, 14);
			}

			if (blink) {
				analogWrite(5, 0);
				analogWrite(6, 255);
				analogWrite(7, 255);
				analogWrite(8, 0);
				analogWrite(9, 255);
				analogWrite(10,255);
			}
		}

		if (selectF && valueF == 45) {
			if (digitalRead(buttonR) == LOW) {
				farolO = true;
			}

			if (digitalRead(buttonR) == HIGH && farolO == true) {
				farolO = false;
				if (farolC) {
					farolC = false;
				}
				else {
					farolC = true;
				}
			}
		}

		if (farolC) {
			u8g.drawBox  (  2, 47, 14, 14);
			u8g.drawBox  (112, 47, 14, 14);

			digitalWrite(11, HIGH);
		}
		else {
			digitalWrite(11, LOW);
		}

		counter++;
		if (counter == 9) {
			counter = 0;

			if (blink) {
				blink = false;
			}
				else {
				blink = true;
			}
		}

		if (!blink) {
			analogWrite(5, 0);
			analogWrite(6, 0);
			analogWrite(7, 0);
			analogWrite(8, 0);
			analogWrite(9, 0);
			analogWrite(10,0);
		}

	} while(u8g.nextPage());

}

void start() {

	float c, cx, cy, cz, cxx, cyy, czz, cxxx, cyyy, czzz;

	int x = 64;
	int y = 32;

	int frame[12][2];

	int cube[8][3] = {
		{-20,-20, 20},
		{ 20,-20, 20},
		{ 20, 20, 20},
		{-20, 20, 20},
		{-20,-20,-20},
		{ 20,-20,-20},
		{ 20, 20,-20},
		{-20, 20,-20}
	};

	char ae[2] = "";
		 ae[0] = 198;

	for (int angle = 0; angle <= 90; angle = angle + 2) {

		u8g.firstPage();

		do {

			u8g.setFont(u8g_font_courB14);

			u8g.drawStr( 12, 35, "sinaliz-");
			u8g.drawStr(100, 35, ae);

			for (int i = 0; i < 8; i++) {
				c = angle * 0.0174532;

				cz = cube[i][2] * cos(c) - cube[i][0] * sin(c);
				cx = cube[i][2] * sin(c) + cube[i][0] * cos(c);
				cy = cube[i][1];

				cyy = cy * cos(c) - cz * sin(c);
				czz = cy * sin(c) + cz * cos(c);
				cxx = cx;

				cxxx = cxx * cos(c) - cyy * sin(c);
				cyyy = cxx * sin(c) + cyy * cos(c);
				czzz = czz;

				cxxx = cxxx + x;
				cyyy = cyyy + y;

				frame[i][0] = cxxx;
				frame[i][1] = cyyy;
				frame[i][2] = czzz;

				u8g.drawPixel (cxxx, cyyy);
			}

			u8g.drawLine(frame[0][0], frame[0][1], frame[1][0], frame[1][1]);
			u8g.drawLine(frame[1][0], frame[1][1], frame[2][0], frame[2][1]);
			u8g.drawLine(frame[2][0], frame[2][1], frame[3][0], frame[3][1]);
			u8g.drawLine(frame[3][0], frame[3][1], frame[0][0], frame[0][1]);

			u8g.drawLine(frame[4][0], frame[4][1], frame[5][0], frame[5][1]);
			u8g.drawLine(frame[5][0], frame[5][1], frame[6][0], frame[6][1]);
			u8g.drawLine(frame[6][0], frame[6][1], frame[7][0], frame[7][1]);
			u8g.drawLine(frame[7][0], frame[7][1], frame[4][0], frame[4][1]);

			u8g.drawLine(frame[0][0], frame[0][1], frame[4][0], frame[4][1]);
			u8g.drawLine(frame[1][0], frame[1][1], frame[5][0], frame[5][1]);
			u8g.drawLine(frame[2][0], frame[2][1], frame[6][0], frame[6][1]);
			u8g.drawLine(frame[3][0], frame[3][1], frame[7][0], frame[7][1]);

			if (angle == 90) {
				loading = false;
			}

		} while (u8g.nextPage());

	}

}
