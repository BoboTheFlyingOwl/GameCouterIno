#include <LiquidCrystal_I2C.h>

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

//variabili per il visualizzare selettore
int oldi = 0;
int oldj = 0;

//FlagStato
bool WelcomeStatFlag = true;
bool SetUpPlayerFlag = false;
bool SelectPlayerFlag = false;
bool ActionPlayerFlag = false;
bool AddPointFlag = false;
bool MultAddFlag = false;
bool BackStatFlag = false;
bool WinStatsFlag = false;

char *Action[] = {"Add ","Com","Back"}; //Menu per azioni
int classifica[4]; //array per la classifica
int clickCount =0; //conta i click per exit

bool setk = false; //flag per la costante

void setup(){
  pinMode(APin, INPUT);
  pinMode(BPin, INPUT);
  pinMode(SPin, INPUT_PULLUP);

  lcd.begin(); //Init lcd ARDUINO
  lcd.backlight(); // retroilluminazione
}

void loop(){
  if(click()){} // check function
  if(WelcomeStatFlag){
    WelcomeStato();
  }
  if(SetUpPlayerFlag){
    SetUpGameStato();
  }
  if(SelectPlayerFlag){
    SelectPlayerStato();
  }
  if(ActionPlayerFlag){
    ActionPlayerStato();
  }
  if(AddPointFlag){
    AddPointStato();
  }
  if(MultAddFlag){
    MulAddStato();
  }
  if(BackStatFlag){
    BackStato();
  }
  if(WinStatsFlag){
    WinStato();
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
  //variabile globale maxplayer
  if(val > MaxPlayer){ val = MaxPlayer;}
  if(val != oldVal){
   clearLinePoint(11,0);
    lcd.setCursor(11,0);
    lcd.print(val);
    delay(50);
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
  delay(10);
  if(val < 0){val = 0;}
  if(val > NumPlayer + 2){val = NumPlayer+2 ;}
  VSelector(val, col);
  if(click()){
    if(val == NumPlayer){
      viewScore();
    }
    else if( val ==( NumPlayer + 1) ){
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
  delay(10);
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
    delay(50);
    oldVal= val;
  }

  if(click()){
    if(val == 0){
      cIAddAct();
    }else{
      Player[PlayerSel] =  Player[PlayerSel] + val;
      val = 0;
      viewPoint();
      delay(1000);
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
      viewMult(val, 0);
      delay(50);
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
     viewMult(k, val);
     delay(50);
     oldVal= val;
    }

  if(click()){
    if(k == 0){
      cIMultAct();
    }else if(val ==0){
      setk = false;
    }else{
      Player[PlayerSel] =  Player[PlayerSel] + (k*val);
      viewMult(k, val);
      val = 0;
      k = 0;
      delay(1000);
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

  lcd.scrollDisplayLeft();
  delay(700);
  if(click()){
    clickCount ++;
    if(clickCount == 3){
      cIWinWel();
    }
  }
}

/* Stato Back
*/
void BackStato(){
  val = val + getRotaryEncoder();
  delay(10);
  if(val < 0){val = 0;}
  if(val > 1){val = 0;}
  VSelector(val, col);
  if(click()){
    if(val == 0){
      //No
      cIBackSel();
    }else{
      //Yes
      cIBackSet();
    }
  }
}

/* Stato Welcome
*/
void WelcomeStato(){
  WelcomeMessage();
  cIWelSet();
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

    SetUpPlayerFlag = false;
    SelectPlayerFlag = true;
    val = 0;
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
  SelectPlayerFlag = false;
  ActionPlayerFlag = true;
  viewAction();
  PlayerSel = val;
  val = 0;
}

/* from Action to Select
*/
void cIActSel(){
  lcd.clear();
  SelectPlayerFlag = true;
  ActionPlayerFlag = false;
  viewPlayer();
  val = 0;
}

/* from Action to Add
*/
void cIActAdd(){
    lcd.clear();
    ActionPlayerFlag = false;
    AddPointFlag = true;
    viewPoint();
    val = 0;
}

/* from Action to Mult
*/
void cIActMult(){
    lcd.clear();
    ActionPlayerFlag = false;
    MultAddFlag = true;
    viewPoint();
    val = 0;
}

/* from Mult to Act
*/
void cIMultAct(){
    lcd.clear();
    ActionPlayerFlag = true;
    MultAddFlag = false;
    viewPoint();
    val = 0;
}

/* from Add to Act
*/
void cIAddAct(){
    lcd.clear();
    viewAction();
    AddPointFlag = false;
    ActionPlayerFlag = true;
    val = 0;
}

/* from Add to Select
*/
void cIAddSel(){
    lcd.clear();
    AddPointFlag = false;
    SelectPlayerFlag = true;
    viewPlayer();
    val = 0;
}

/* from Mul to Sel
*/
void cIMulSel(){
    lcd.clear();
     MultAddFlag= false;
    SelectPlayerFlag = true;
    viewPlayer();
    val = 0;
}

/* from Select to Back
*/
void cISelBack(){
  lcd.clear();
  SelectPlayerFlag = false;
  BackStatFlag = true;
  val = 0;
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
    BackStatFlag = false;
    SelectPlayerFlag = true;
    viewPlayer();
    val = 0;
}

/* from Back to SetUp
*/
void cIBackSet(){
  lcd.clear();
    BackStatFlag = false;
    SetUpPlayerFlag = true;
    val = 0;
}

/* from Select to Win
*/
void cISelWin(){
  lcd.clear();
  SelectPlayerFlag = false;
  WinStatsFlag = true;
  val = 0;
  setClassifica();
  sort();
}

/* from Welcome to SetUp
*/
void cIWelSet(){
  lcd.clear();
   WelcomeStatFlag = false;
   SetUpPlayerFlag = true;
}

/* from Win to Welcome
*/
void cIWinWel(){
  lcd.clear();
  WinStatsFlag = false;
  WelcomeStatFlag = true;
  GoodByeMessage();
}

    //view methods
/* Post: stampa sull Lcd l'elenco dei Player [PN <- N-esimo giocatore]
*/
void viewPlayer(){
  for(int i= 0; i< NumPlayer; i++){
    int y = i % 2;
    int x = (col * (i / 2)) + 1;
    lcd.setCursor(x,y);
    lcd.print("P");
    lcd.print(i+1);
  }
  int y = NumPlayer % 2;
  int x = (col * (NumPlayer / 2)) + 1;
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
  lcd.clear();
  for(int i = 0; i < sizeof(Action);i++){
    int y = i % 2;
    int x = (4 * (i / 2)) + 1; //4 -> la dimensione della colonna 4
    lcd.setCursor(x,y);
    lcd.print(Action[i]);
  }
}

/* Stampa un informativa per lo stato di Mult
*/
void viewMult(int cons, int v){
  lcd.clear();
  lcd.setCursor(0,0);
  if(v == 0){
    lcd.print("Const: ");
    lcd.print(cons);
  }else{
  lcd.print("Val: (");
  lcd.print(cons);
  lcd.print(")X");
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
  delay(3000);
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
  delay(2000);
  delay(10000);
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
  lcd.print(val);
  lcd.setCursor(0, 1);
  lcd.print("Tot: ");
  lcd.print(Player[PlayerSel]);
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
  return result;
}

/* Post: restituisce se il pulsante ?? stato premuto.
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
  //inizializza Array classifica
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


String STScore(){
  String s="";
  for(int i =0 ; i < NumPlayer; i++){
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
  if(n < 100){
    s+=" ";
  }
  return s;
}
