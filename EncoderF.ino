#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int APin = 8; //Set the digital 2 to A pin
const int BPin = 9; //Set the digital 3 to B pin
const int SPin = 11; //Set the digital 4 to S pin
// variabili per Encoder
int val = 0;
int oldVal = 0;
// variabili per il bottone dell Encoder
int state = 0;
int oldState = 0;

//variabili per i player
int NumPlayer ;
int Player[0];
//int tot = 0; // mantiene il totale dei punti
int PlayerSel = 0;

int oldn=0; // server per il selettore

bool AddPointFlag = false;
bool ActionPlayerFlag = false;
bool SelectPlayerFlag = false;
bool SetUpPlayerFlag = true;


void setup(){
  pinMode(APin, INPUT);//initialize the A pin as input
  pinMode(BPin, INPUT);//initialize the B pin as input
  pinMode(SPin, INPUT_PULLUP);//initialize the S pin as input  

  lcd.begin(); //Init with pin default ESP8266 or ARDUINO
  lcd.backlight(); //accende la retroilluminazione
}

void loop(){
  if(click()){} // check function
  if(SetUpPlayerFlag){
    SetUpGameInterface();
  }
  if(SelectPlayerFlag){
    SelectPlayerInterface();
  }
  if(ActionPlayerFlag){
    ActionPlayerInterface();
  }
  if(AddPointFlag){
    AddPointInterfaceLcd();
  }
}

      //Methods
//Legge la rotazione del Encoder 
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

//Legge un click singolo
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

//Pulisce Lcd dall [4;x] fino alla [maxLine:x] 
void clearLinePoint(int x,int y){
  lcd.setCursor(x,y);
  lcd.print("            ");
}

//Vselector
void VSelector(int n){
  
  delVSelector(oldn);
  int x = 3 * ( n / 2 );
  int y = n % 2;
  lcd.setCursor(x,y);
  lcd.print(">");
  oldn = n;
}

void delVSelector(int n){
  int x = 3 * ( n / 2 );
  int y = n % 2;
  lcd.setCursor(x,y);
  lcd.print(" ");
}

      //Interface
//Interfaccia Setup
void SetUpGameInterface(){
  lcd.setCursor(0,0);
  lcd.print("Giocatori: ");
  val = val +  getRotaryEncoder();  
  if(val < 0){ val = 0;}
  //variabile globale maxplayer
  if(val != oldVal){    
   clearLinePoint(11,0);
    lcd.setCursor(11,0);
    lcd.print(val);
    delay(50);
    oldVal= val;
  } 
  if(click()){
    NumPlayer = val;
    val=0;
    lcd.setCursor(0,1);
    lcd.print(NumPlayer);
    lcd.print("< nuovo num");
    cISetSel();
  }
}

//Interfaccia SelectPlayer
void SelectPlayerInterface(){
  val = val + getRotaryEncoder();
  delay(10);
  if(val < 0){val = 0;}
  if(val > 9){val = 9;}
  VSelector(val);
  if(click()){
    cISelAct();
  }
}

//Interfaccia Action Player
void ActionPlayerInterface(){
  lcd.setCursor(0,0);
  lcd.print("Action");
  val = val + getRotaryEncoder();
  if(val < 0){val = 0;}
  if(val > 1){val = 1;}
  delay(10);
  //cambio stato
  if(click()){
  switch (val) {
  case 0: //add 
    cIActAdd();
    break;
  case 1: //Back
   cIActSel();
    break;
  }
}

  delay(1000);
  cIActAdd();
}

// Interfacci Addpoint
void AddPointInterfaceLcd(){
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
    Player[PlayerSel] =  Player[PlayerSel] + val;
    val = 0;
    clearLinePoint(4,1);
    lcd.setCursor(0, 1);
    lcd.print("Tot: ");
    lcd.print( Player[PlayerSel]);
    delay(500);
    
    cIAddSel();
  }
 
}


      //Change Interface
//change Interface from: Set to: Sel
void cISetSel(){
  lcd.clear();
  Player[NumPlayer];
  SetUpPlayerFlag = false;
  SelectPlayerFlag = true;
  val = 0;
  viewPlayer();
}

// change interface from: Sel to: Act
void cISelAct(){
  lcd.clear();
  SelectPlayerFlag = false;
  ActionPlayerFlag = true;
  PlayerSel = val;
  val = 0;
}

// change interface from: Act to: Sel
void cIActSel(){
  lcd.clear();
  SelectPlayerFlag = true;
  ActionPlayerFlag = false;
  val = 0;
  viewPlayer();
}

//change Interface from:Act to:Add
void cIActAdd(){
    lcd.clear();
    ActionPlayerFlag = false;
    AddPointFlag = true;
    val = 0;
    //Prepare interface
    lcd.setCursor(0, 0);
    lcd.print("Val: ");
    lcd.print(val);
    lcd.setCursor(0, 1);
    lcd.print("Tot: ");
    lcd.print( Player[PlayerSel]);
    
}

//change Interface from:Add to:Act
void cIAddAct(){
    lcd.clear();
    AddPointFlag = false;
    ActionPlayerFlag = true;
    val = 0;
}

//change Interface from:Add to:Sel
void cIAddSel(){
    lcd.clear();
    AddPointFlag = false;
    SelectPlayerFlag = true;
    val = 0;
    viewPlayer();
}

void viewPlayer(){
  for(int i= 0; i< NumPlayer; i++){
    int y = i % 2;
    int x = (3 * (i / 2)) + 1; 
    lcd.setCursor(x,y);
    lcd.print("P");
    lcd.print(i);
  }
}
