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
int NumPlayer ;
int MaxPlayer = 4;
int Player[ 4 ];
int PlayerSel;

//variabili per il visualizzare selettore
int oldi = 0;
int oldj = 0;

//FlagStato
bool AddPointFlag = false;
bool ActionPlayerFlag = false;
bool SelectPlayerFlag = false;
bool SetUpPlayerFlag = false;
bool BackStatFlag = false;
bool WinStatsFlag = false;
bool WelcomeStatFlag = true;

char *Action[] = {"Add ","Back"}; //Menu per azioni
int classifica[4]; //array per la classifica
int clickCount =0; //conta i click per exit


int col = 4; // variabile per la visualizzazione dello stato SelectPlayerStato
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
  if(val > 1){val = 1;}
  delay(10);
  VSelector(val, 4); // 4 perche col = 4 (Back)
  //cambio stato
  if(click()){
    if(val == 0){
      cIActAdd();
    }
    if(val == 1){
      cIActSel();
    }
  }

}

/* Stato Add
*/
void AddPointStato(){
  val = val +  getRotaryEncoder();
  if(val != oldVal){
    clearLinePoint(4, 0);
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(val);

    delay(50);
    oldVal= val;
  }

  if(click()){
    if(val == 0){
      cIAddAct();
    }else{
      Player[PlayerSel] =  Player[PlayerSel] + val;
      val = 0;
      clearLinePoint(4,1);
      lcd.setCursor(0, 1);
      lcd.print("Tot: ");
      lcd.print( Player[PlayerSel] );
      delay(1000);
      cIAddSel();
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
    if(clickCount == 5){
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
      viewPoint():
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
    int x = (3 * (i / 2)) + 1;
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

// altre funzioni di view() -> prepare
