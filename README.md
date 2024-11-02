# TuTu
Пример имитации  контроля  запрета  движения  железнодорожного переезда.

Состав:
ARDUINO NANO
2 Светодиода
2 резистора 120 Ом
Зуммер (Пищалка)
2 HC-SR04 Ultrasonic Distance Sensor

Как работает код:
Датчики HC-SR04 контролируют фиксацию поезда.
Контролируется только сплошной проезд. Так же контролируется, что поезд короткий и в какой то момент времени его не видят 2 датчика.
Когда фиксируется что  поезд проехал сигнализация отключается.
Во время фиксации проезда  звучит зумер и моргают светодиоды.

Функция  для  измерения растояния:
float readDistanceCMONE(){
  digitalWrite(PINTRIGONE, LOW);
  delayMicroseconds(2);
  digitalWrite(PINTRIGONE, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINTRIGONE, LOW);
  int duration = pulseIn(PINECHOONE, HIGH, 25000);
  return duration * 0.034 / 2;
}

Внимание HC-SR04 Ultrasonic Distance Sensor не может  работать с тонкими и пушистыми объектами, информация  будет постоянно искажаться.
Если датчик постоянно греется проверяйте:
1. Контакты
2. Частота опроса
3. Верное указание функций.
(int duration = pulseIn(PINECHOONE, HIGH); - если указать так через 2.5 часа будет перегрев, надо так: int duration = pulseIn(PINECHOONE, HIGH, 25000);)
