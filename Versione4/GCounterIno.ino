#include <LiquidCrystal_I2C.h>
#include "note.h"
//#include "music.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int APin = 8;  //Rot Encoder
const int BPin = 9; //Rot Encoder
const int SPin = 11; //Switch Encoder
// variabili per Encoder
int val = 0;
int oldVal = 0;
// variabili per il bottone dell Encoder
int state = 0;
int oldState = 0;

//variabili per i player
int NumPlayer ; // indica quanti player
int MaxPlayer = 4; // imposta il numero massimo di player
int Player[ 4 ]; // crea un array di lunghezza pari al palter
int PlayerSel; // indica il plater selezionato
int col = 4; // variabile per la visualizzazione dello stato SelectPlayerStato
int classifica[4]; //array per la classifica

//variabili per il visualizzare selettore
int oldi = 0;
int oldj = 0;

int valueState = 0;  // Per lo stato [Welcome, Setting, Select, Action, Add, Mult, Back, Win]
bool setk = false; //flag per la costante


char *Action[] = {"Add ","Com","Back"}; //Menu per azioni

int clickCount =0; //conta i click per exit

unsigned long t0;
unsigned long dt;

//BuzzerMusic
int buzz = 2;

  int endMusic[]={C4, P, C5, P, E4, P, C5, P , G4, P, C4, P, E7,P};
  int durataEnd[] = {16,16,16,16,16,16,16,16,32,32,32,32,8,16};
  int nEndNote = 14;

  int startMusic[]={C7, P, C7, P};
  int durataStart[] = {8,8,8,8};
  int nStartNote = 4;

 //Led
  int lv = 6;
  int lb = 5;
  int lr = 4;
  int Max = 50;



void setup(){
  pinMode(APin, INPUT);
  pinMode(BPin, INPUT);
  pinMode(SPin, INPUT_PULLUP);

  pinMode(buzz, OUTPUT);

  pinMode(lv, OUTPUT);
  pinMode(lb, OUTPUT);
  pinMode(lr, OUTPUT);

  lcd.begin(); //Init lcd ARDUINO
  lcd.backlight(); // retroilluminazione
}

void loop(){
  if(click()){} // check function

  switch(valueState){
    case 0:
     WelcomeStato();
    break;
    case 1:
      SetUpGameStato();
    break;
    case 2:
     SelectPlayerStato();
    break;
    case 3:
    ActionPlayerStato();
    break;
    case 4:
     AddPointStato();
    break;
    case 5:
     MulAddStato();
    break;
    case 6:
     BackStato();
    break;
    case 7:
    WinStato();
    break;
  }

}


      //Stato
/* Stato SetUp
*/
void SetUpGameStato(){
  lcd.setCursor(0,0);
  lcd.print("Giocatori: ");
  val = val +  getRotaryEncoder();
  if(val < 0){ val = 0;}
  if(val > MaxPlayer){ val = MaxPlayer;}
  if(val != oldVal){
   clearLinePoint(11,0);
   lcd.setCursor(11,0);
   lcd.print(val);
   oldVal= val;
  }
  if(click()){
    cISetSel();
  }
}

/* Stato Select
*/
void SelectPlayerStato(){
  val = val + getRotaryEncoder();
  if(val < 0){val = 0;}
  if(val > NumPlayer + 2){val = NumPlayer + 2;}
  VSelector(val, col);
  if(click()){
    if(val == NumPlayer){
      viewScore();
    }else if( val ==( NumPlayer + 1) ){
       cISelBack();
    }else if(val ==( NumPlayer + 2)){
       cISelWin();
    }else{
      cISelAct();
    }
  }
}

/* Stato Action
*/
void ActionPlayerStato(){

  val = val + getRotaryEncoder();
  if(val < 0){val = 0;}
  if(val > 2){val = 2;}
  VSelector(val, 4); // 4 perche col = 4 (Back)
  //cambio stato
  if(click()){
    if(val == 0){
      cIActAdd();
    }
    if(val == 1){
      cIActMult();
    }
    if(val == 2){
      cIActSel();
    }
  }
}

/* Stato Add
*/
void AddPointStato(){
  val = val +  getRotaryEncoder();
  if(val != oldVal){
    viewPoint();
    oldVal= val;
  }

  if(click()){
    if(val == 0){
      cIAddAct();
    }else{
      Player[PlayerSel] =  Player[PlayerSel] + val;
      viewPoint();
      viewTot();
      blinkPoint(lb,Player[PlayerSel]);
      cIAddSel();
    }
  }

}

/* Questo stato permetti di selezionare un moltiplicatore per il valore dei punti
 * Post:
 */
void MulAddStato(){
 float k;
  while(!setk){
    val = val +  getRotaryEncoder();
    if(val != oldVal){
      viewMult(val, 0,setk);
      oldVal= val;
    }
    if(click()){
     k = val;
     val = 0 ;
     setk = true;
    }
  }

  val = val +  getRotaryEncoder();
    if(val != oldVal){
     viewMult(k, val,setk);
     oldVal= val;
    }

  if(click()){
    if(k == 0){
      cIMultAct();
    }else if(val ==0){
      setk = false;
    }else{
      Player[PlayerSel] =  Player[PlayerSel] + (k*val);
      viewMult(k, val,setk);
      viewTot();
      blinkPoint(lb,Player[PlayerSel]);
      val = 0;
      k = 0;
      cIMulSel();
    }
  }
}

/* Stato Win
*/
void WinStato(){
  lcd.setCursor(0,0);
  lcd.print(ST());
  lcd.setCursor(0,1);
  lcd.print(SB());
  int k =0;
  makeMusic(endMusic, durataEnd, nEndNote);
  while(k < 30){
    lcd.scrollDisplayLeft();
  delay(700);
  k++;
  }
    cIWinWel();
}

/* Stato Back
*/
void BackStato(){
  val = val + getRotaryEncoder();
  if(val < 0){val = 0;}
  if(val > 1){val = 0;}
  VSelector(val, col);
  if(click()){
    if(val == 0){
      cIBackSel();
    }else{
      cIBackSet();
    }
  }
}

/* Stato Welcome
*/
void WelcomeStato(){
  WelcomeMessage();
  cIWelSet();
  makeMusic(startMusic, durataStart, nStartNote);
}

      //Change State methods
/* from SetUp to Select
*/
void cISetSel(){
  if(val != 0){
    lcd.clear();
    NumPlayer = val;
    Player[NumPlayer];
    initPlayer();
    startTimer();

    valueState = 2;
    val = 0;

    blinkSimple(lv);
    viewPlayer();
  }else{
    lcd.setCursor(0,0);
    lcd.print("Numero non valido");
    delay(1000);
    lcd.clear();
  }

}

/* from Select to Action
*/
void cISelAct(){
  lcd.clear();
  valueState = 3;
  viewAction();
  PlayerSel = val;
  val = 0;
  blinkSimple(lv);
}

/* from Action to Select
*/
void cIActSel(){
  lcd.clear();
  valueState = 2;
  viewPlayer();
  val = 0;
  blinkSimple(lr);
}

/* from Action to Add
*/
void cIActAdd(){
  lcd.clear();
  valueState = 4;
  viewPoint();
  val = 0;
  blinkSimple(lv);
}

/* from Action to Mult
*/
void cIActMult(){
  lcd.clear();
  valueState = 5;
  viewMult(0,0,setk);
  val = 0;
  blinkSimple(lv);
}

/* from Mult to Act
*/
void cIMultAct(){
  lcd.clear();
  valueState = 3;
  viewAction();
  val = 0;
  setk = false;
  blinkSimple(lr);
}

/* from Add to Act
*/
void cIAddAct(){
  lcd.clear();
  valueState = 3;
  viewAction();
  val = 0;
  blinkSimple(lr);
}

/* from Add to Select
*/
void cIAddSel(){
  lcd.clear();
  valueState = 2;
  viewPlayer();
  val = 0;
  blinkSimple(lv);
}

/* from Mul to Sel
*/
void cIMulSel(){
  lcd.clear();
  valueState = 2;
  viewPlayer();
  val = 0;
  blinkSimple(lv);
}

/* from Select to Back
*/
void cISelBack(){
  lcd.clear();
  valueState = 6;
  val = 0;
  blinkSimple(lr);
  //messaggio di conferma
  lcd.setCursor(0,0);
  lcd.print("Confermare");
  delay(1500);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("No ");
  lcd.setCursor(1,1);
  lcd.print("Yes ");
}

/* from Back to Select
*/
void cIBackSel(){
  lcd.clear();
  valueState = 2;
  viewPlayer();
  val = 0;
  blinkSimple(lr);
  blinkSimple(lr);
}

/* from Back to SetUp
*/
void cIBackSet(){
  lcd.clear();
  valueState = 1;
  val = 0;
  blinkSimple(lv);
}

/* from Select to Win
*/
void cISelWin(){
  lcd.clear();
  valueState = 7;
  val = 0;
  setClassifica();
  sort();
  stopTimer();
  blinkWin();
}

/* from Welcome to SetUp
*/
void cIWelSet(){
  lcd.clear();
  valueState = 1;
}

/* from Win to Welcome
*/
void cIWinWel(){
  lcd.clear();
  valueState = 0;
  viewTimer();
  blinkOff();
  GoodByeMessage();
}


    //view methods
/* Post: stampa sull Lcd l'elenco dei Player [PN <- N-esimo giocatore]
*/
void viewPlayer(){
  int x,y;
  for(int i= 0; i< NumPlayer; i++){
    y = i % 2;
    x = (col * (i / 2)) + 1;
    lcd.setCursor(x,y);
    lcd.print("P");
    lcd.print(i+1);
  }
  y = NumPlayer % 2;
  x = (col * (NumPlayer / 2)) + 1;
  lcd.setCursor(x,y);
  lcd.print("Scr"); // score
  y = (NumPlayer + 1) % 2;
  x = (col * ( (NumPlayer + 1) / 2)) + 1;
  lcd.setCursor(x,y);
  lcd.print("Back");
  y = (NumPlayer + 2) % 2;
  x = (col * ( (NumPlayer + 2) / 2)) + 1;
  lcd.setCursor(x,y);
  lcd.print("Win");
}

/* Post: stampa sull Lcd l'elenco delle azioni possibili
*/
void viewAction(){
  for(int i = 0; i < sizeof(Action);i++){
    int y = i % 2;
    int x = (col * (i / 2)) + 1; //4 -> la dimensione della colonna 4
    lcd.setCursor(x,y);
    lcd.print(Action[i]);
  }
}

/* Stampa un informativa per lo stato di Mult
*/
void viewMult(int c, int v, bool s){
  if(!s){
    lcd.setCursor(0,0);
    lcd.print("Const: ");
    lcd.print(c);
  }else{
    lcd.setCursor(0,0);
    lcd.print("Val: (");
    lcd.print(c);
    lcd.print(")x");
    lcd.print(v);
  }
  lcd.setCursor(0, 1);
  lcd.print("Tot: ");
  lcd.print(Player[PlayerSel]);
}

/* Post: stampa sull Lcd messaggi di benvenuto
*/
void WelcomeMessage(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GameCounterIno");
  delay(1000);
  int count =0;
  while(count < 17){
    lcd.scrollDisplayRight();
    count++;
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(9, 1);
  lcd.print("By Bobo");
  delay(2000);
}

/* Post: stampa sull Lcd messaggi di uscita
*/
void GoodByeMessage(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("GameCounterIno");
  delay(2000);
  lcd.setCursor(6, 1);
  lcd.print("End");
  delay(7000);
}

/* Post: stampa a schermo un informativa sui punteggi attuali della partita
*/
void viewScore(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(STScore());
  lcd.setCursor(0,1);
  lcd.print(SB());
  delay(2000);
  int c = 0;
  //5  -> sync con il display
  while( c < 5 * NumPlayer){
    c++;
    lcd.scrollDisplayLeft();
    delay(500);
  }
  lcd.clear();
  viewPlayer();
  val = 0;
}

/*Post: stampa a schermo il punteggio del giocatore
 */
void viewPoint(){
  lcd.setCursor(0, 0);
  lcd.print("Val: ");
  clearLinePoint(5,0);
  lcd.setCursor(5, 0);
  lcd.print(val);
  lcd.setCursor(0, 1);
  lcd.print("Tot: ");
  lcd.print(Player[PlayerSel]);
}

/*Post:
 */
void viewTot(){
  clearLinePoint(0,1);
  lcd.setCursor(0, 1);
  lcd.print("Tot: ");
  lcd.print(Player[PlayerSel]);
  delay(1500);
}

/* Effect: pulisce Lcd dalla posizione [x;y] fino a [x+16;y]
 */
void clearLinePoint(int x,int y){
  lcd.setCursor(x,y);
  for(x; x < 16; x++){
    lcd.print(" ");
  }
}

/*Effect: permette la visualizzazione di una '>' sull lcd
 */
void VSelector(int i, int j){
  delVSelector(oldi, oldj);
  int x = col * ( i / 2 );
  int y = i % 2;
  lcd.setCursor(x,y);
  lcd.print(">");
  oldi = i;
  oldj = j;
}

/*Effect: permette la scomparsa di '>' sull lcd
 */
void delVSelector(int i, int j){
  int x = col * ( i / 2 );
  int y = i % 2;
  lcd.setCursor(x,y);
  lcd.print(" ");
}

/* Effect: Visualizza la durata della partita
 */
void viewTimer(){
  int sec = dt / 1000; // tolgo i millisecondi
  int m = sec /60;
  sec = sec % 60;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Durata: ");
  lcd.setCursor(0,1);
  lcd.print(m);
  lcd.print(":");
  lcd.print(sec);
  delay(3000);
 }

    //Methods

/*Post: restituisce la lettura dell encoder
*/
int getRotaryEncoder(){
  static int oldA = HIGH;
  static int oldB = HIGH;
  int result = 0;
  int newA = digitalRead(APin); //read the value of APin to newA
  int newB = digitalRead(BPin); //read the value of BPin to newB
  if (newA != oldA || newB != oldB){
    if (oldA == HIGH && newA == LOW){
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  delay(10);
  return result;
}

/* Post: restituisce se il pulsante è stato premuto.
*  true solo quando cambia lo stato da premuto a non premuto, false altrimenti
*/
bool click(){
  state = digitalRead(SPin);
  if(state != oldState){
    oldState = state;
    if(state == HIGH){
      return true;
    }
  }
  return false;
}

/* Effect: inizializza Player[]
*/
void initPlayer(){
  Player[NumPlayer];
  for(int i =0; i< NumPlayer; i++){
    Player[i] = 0;
  }
}

/* Effect: ordina in modo crescente (Bubble Sort) Player[] e sposta gli elementi in classifica in corrispondenza
*/
void sort() {
  int t;
  int tc;
  for( int i =0; i<(NumPlayer - 1); i++) {
    for(int k=0; k<(NumPlayer-(i+1)); k++) {
      if(Player[k] < Player[k+1]) {
        t = Player[k];
        Player[k] = Player[k+1];
        Player[k+1] = t;

        tc = classifica[k];
        classifica[k] = classifica[k + 1];
        classifica[k + 1] = tc;
      }
    }
  }
}

/* Effect: riempie l'array classifica
*/
void setClassifica(){
  for(int i = 0; i < NumPlayer; i++ ){
    classifica[i] = i+1;
  }
}

/* Post: restituisce una stringa che rappresenta i punteggi dei giocatori
 */
String STScore(){
  String s="";
  for(int i = NumPlayer-1 ; i >= 0; i--){
    s += "  P";
    s += (String) (i+1);
    s += " ";
  }
  return s;
}

/* Post: restituisce una stringa che rappresenta la classifica ( posizione ^PN )
*/
String ST(){
  String s="";
  for(int i =0 ; i < NumPlayer; i++){
    s += (String) ( ( NumPlayer) - i) ;
    s += "^P";
    s += (String)classifica[i];
    s += " ";
  }
  return s;
}

/* Post: restituisce una stringa che rappresenta i punteggi ordinati
*/
String SB(){
  String s="";
  for(int i = NumPlayer-1 ; i >= 0; i--){
    s += headSpace(Player[i]) ;
    s += (String)Player[i];
    s += " ";
  }
  return s;
}

/* Post: restituisce una stringa che serve per allineare SB con ST
*/
String headSpace(int n){
  String s="";
  if(n < 10){
    s+=" ";
  }
  if(n < 100){
    s+=" ";
  }
  if(n < 1000){
    s+=" ";
  }
  return s;
}

/*Effect: imposta l'inizio del timer.
 */
void startTimer(){
  t0 = millis();
}

/*Post: restituisce quanti millisecondi sono passati dall' inizio della partia
 */
void stopTimer(){
  dt = millis() - t0;
}


// New
void makeMusic(int note[], int durata[], int nNote){
  for(int i =0; i< nNote; i++){
    int dur = 1500 / durata[i];
    tone(buzz, note[i], dur);
    delay(dur * 1.3);
  }
}


void blinkSimple(int led){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

void blinkPoint(int led, int point){

  int k =Max;
  while(k <= point){
  blinkSimple(led);
    k += Max;
  }
  //delay(1000);
}

void blinkWin(){
  int k = 200;
 digitalWrite(lv, HIGH);
  digitalWrite(lb, HIGH);
  digitalWrite(lr, HIGH);
  delay(k);
  digitalWrite(lv, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(lr, LOW);
  delay(k);
  digitalWrite(lv, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(lr, LOW);
  delay(k);
  digitalWrite(lv, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(lr, LOW);
  delay(k);
  digitalWrite(lv, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(lr, HIGH);
  delay(k);
  digitalWrite(lv, LOW);
  digitalWrite(lb, HIGH);
  digitalWrite(lr, LOW);
  delay(k);
  digitalWrite(lv, HIGH);
  digitalWrite(lb, LOW);
  digitalWrite(lr, LOW);
  delay(k);
  digitalWrite(lv, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(lr, LOW);
  delay(2*k);
  digitalWrite(lv, HIGH);
  digitalWrite(lb, HIGH);
  digitalWrite(lr, HIGH);
  delay(k);
}

void blinkOff(){
  int k = 200;
  digitalWrite(lv, LOW);
  digitalWrite(lb, LOW);
  digitalWrite(lr, LOW);
  delay(2*k);
}
