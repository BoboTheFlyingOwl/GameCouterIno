

int endMusic[]={C4, P, C5, P, E4, P, C5, P , G4, P, C4, P, E7,P};
int durataEnd[] = {16,16,16,16,16,16,16,16,32,32,32,32,8,16};
int nEndNote = 14;

int startMusic[]={C7, P, C7, P};
int durataStart[] = {8,8,8,8};
int nStartNote = 4;

#define end1 {C4, P, C5, P, E4, P, C5, P , G4, P, C4, P, E7,P};
#define endDur {16,16,16,16,16,16,16,16,32,32,32,32,8,16};

#define start1 {C7, P, C7, P};
#define startDur {8,8,8,8};
  // posizione del buzzer
void selectMusic(int posizioneBuzzer, char m, int num){
  int note[];
  int durata[];
  int nNote;

  if(m = 'W'){
    note = start1;
    durata = startDur;
    numeroDiNote = sizeof( start1 );
  }else if(m = 'G'){
    note = end1;
    durata = endDur;
    numeroDiNote = sizeof(end1);
  }
  makeMusic(posizioneBuzzer, note, durata, numeroDiNote);
}

void makeMusic(int buzz, int note[], int durata[], int nNote){
  for(int i =0; i< nNote; i++){
    int dur = 1500 / durata[i];
    tone(buzz, note[i], dur);
    delay(dur * 1.3);
  }
}
