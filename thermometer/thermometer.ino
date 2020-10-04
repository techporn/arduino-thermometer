// 5461AS:  4 digit 7 segments LED.
// TMP36GZ: temperature sensor.

const int PIN_TMP36 = 1;  // TMP36のピン(analog input)
const int PIN_SEGMENTS[8] = {2, 3, 4, 5, 6, 7, 8, 9};   // 7セグLED[5461AS]の各セグメントに接続するピン
const int PIN_DIGITS[4] = {10, 11, 12,  13};  // 桁選択用のピン

// 1 = LED on, 0 = LED off　　　　ABCDEFG.
const byte led_patterns[10] = { B11111100,  // 0
                                B01100000,  // 1
                                B11011010,  // 2
                                B11110010,  // 3
                                B01100110,  // 4
                                B10110110,  // 5
                                B10111110,  // 6
                                B11100000,  // 7
                                B11111110,  // 8
                                B11100110   // 9
                             };

void off_digit(int i) {
  digitalWrite(PIN_DIGITS[i], HIGH); 
}

void on_digit(int i) {
  digitalWrite(PIN_DIGITS[i], LOW);
}

void clear_all_digits() {
  // すべての桁をオフにする
  off_digit(0);
  off_digit(1);
  off_digit(2);
  off_digit(3);
}

void pick_digit(int dig) {
  clear_all_digits();  // 全ての桁を消灯する
  on_digit(dig);  // 指定した桁をオンにする
}

void send_serial(byte led) {
  // 各セグメントに、ビットのパターンで表されるHIGH/LOWを1ビットずつ送信する
  for (int i=0; i<8; ++i) {
    byte mask = B10000000 >> i;  // 上位ビットから1ビットずつ取り出すためのマスク
    int flag = (led & mask) != 0;  // 取り出したビットが0でなければflagは1
    digitalWrite(PIN_SEGMENTS[i], flag);
  }
}

void clear_segments() {
  // すべてのセグメントを消灯する。
  send_serial(0);
}

void show_num(int num, int dp) {
  // 指定した数値を表示する
  byte serial = led_patterns[num] | dp;  // LEDの点灯パターンとDPの有無をまとめる
  send_serial(serial);
}

void show_num_4digit(int n_4digit) {
  // 4桁の数字を表示する
  // n_4digitsは４桁の整数
  // 最上位の0は表示されない
  
  int n, dp;
  
  for(int i=3; i>=0; --i) {
    // 3桁目にDPを表示する
    dp = (i == 2); 

    // 数字を一桁ずつnに取り出す
    n = n_4digit % 10;
    n_4digit /= 10;

    // 最上位の桁の値が0なら表示しない
    if (i == 0 && n == 0) {
      continue;
    }

    // nの表示
    clear_segments();
    pick_digit(i);
    show_num(n, dp);
  }

}


void test_all_num() {
  // 全て数字を点灯させるテスト
  
  for(int j=0; j<4; ++j) {
    pick_digit(j);
    for (int i=0; i<10; ++i) {
      clear_segments();
      show_num(i, 1);
      delay(300);
    }
  }
}

double get_temperature() {
  double temp;
  const int V = 4.6  // 理論上は5Vだが、計測結果から4.6Vとする
  temp = (double)analogRead(PIN_TMP36)  / 1024;
  temp = temp * V; 
  temp = temp - 0.5; // TMP36のオフセットを引く
  temp = temp * 100; // degreeに変換
  return temp;
}

void setup() 
{
  for (int i=0; i<8; ++i) {
    pinMode(PIN_SEGMENTS[i], OUTPUT);
  }
  for (int i=0; i<4; ++i) {
    pinMode(PIN_DIGITS[i], OUTPUT);
  }

}

void loop()  
{ 
  double temp;

  // 1000msごとに温度を更新
  if (millis() % 1000 == 0) {
    temp = get_temperature();
  }

//  show_num_4digit(temp * 10);  // 10倍して上４桁を表示する ex.) 123.45 * 10 == 1234
//  test_all_num();
 }
