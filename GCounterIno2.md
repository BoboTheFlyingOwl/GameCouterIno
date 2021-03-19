# GameCounterIno
## Un contatore di punti per giochi da tavolo.
Il Sw parte dallo stato Welcome. In questo stato visualizza la scritta "GameCounter" come schermata di avvio. Automaticamente si passa allo stato successivo SetUp. In questo stato è possibile impostare il numero di giocatori (Per la versione V1 il numero di giocatori al massimo puo essere 4).
Una volta passata la fase di Setup si passa nello stato Select in cui si puó:
* PN = Sceglie il giocatore N per aggiungere/togliere dei punti
* Scr = Vedere i punteggi dei vari giocatori. 
* Win = Terminare la partita.
* Back = Tornare a reimpostare il numero di giocatori(Perdendo i punti gia assegnati).
 
 ## Action State
Dopo aver scelto un Player è possibile scegliere due modi per aggiungere punti:
* Simple (Add) -> in questo modo si sceglie il valore da aggiungere ai punti totali. Player[Numero] <- Player[Numero] + valore.
* Complex (Com) -> in questo modo si sceglie una costante moltiplicativa e poi il valore. Player[Numero] <- Player[Numero] + (Costante * valore).
una volta aggiuniti i punti si ritorna allo stato di Select.

## Score
Dallo stato Select è possibile vedere i punti dei vari player selezionando l'opzione Scr.

## Terminare la partita
Selezionando l'opzione Win viene visualizzato la classifica della partita gia ordinata.

##Opzione Back
Alcuni stati hanno l'opzione Back con la quale è possibile tornare allo stato precedente. Nello stato Select questa opzione dará la possibilita di ripetere l azione di selezionare quanti giocatori. Riselezionando azzererà i punteggi attuali comuque questa azione è seguita da una conferma.

## Versione 1
### Stati:
* SetUp: In questo stato si può selezionare il numero dei player. Quando si prende una decisione si passa allo stato successivo. Il numero dei giocatori puo essere tra 1 e 4, selezionando il valore 0 la scelta viene segnalata non valida.
* SelectPlayer: In questo stato viene creata un interfaccia dove e possibile scegliere un Player o altre opzioni.
* ActionPLayer: In questo stato, dopo aver scelto il giocatore, si puo decidere di aumentare i punteggio.
* AddPoint e Multpoint: In questi stati è possibile aggiungere i punteggi.
* Back: per tornare allo stato iniziale.
* Win: schermata che comunica i risultati della partita.

Ogni cambimento avviene attraverso funzioni cI[from][to] (change Interface) nelle quali viene modificato il flag per variare lo stato e, mediante funzioni di view preparano lcd o comunicano qualcosa.
* cISetSel: Imposta quanti player, inizializza l'array PLayer e prepara a visualizzare la schermata di SelectPlayer.
* cISelAct: Memorizza l'indice del Player selezionato.
* cISelBack: Imposta la schermta per selezionare se si vuole uscire o no.
* cIBackSet: Reimposta lo stato di SetUp.
* cIBackSel: Ritorno allo stato di SelectPlayer.
* cISelWin: La partita termina. Viene fermato il timer, viene visualizzata la classifica ordinata.
* cISelAct: Permette di tornare indietro nello stato Action.
* cIActAdd: Viene visualizzato il punteggio totale del giocatore e il punteggio da aggiungere.
* cIActSel: Torno allo stato precedente.
* cIActMult: Viene visualizzato il punteggio totale del giocatore e il punteggio da aggiungere.

I view:
* viewPlayer: visualizza i Player.
* viewAction: visualizza il menu delle azioni.
* viewMult: visualizza il formato per aggiungere i punti.
* viewPoint: visualizza il formato per aggiunger i punti.
* Vselector: permette la visualizzazione di un selettore per le varie opioni ">";
* delVSelectior: pulisce lo schermo dal selettore precedente.

I metodi:
* getRotaryEncoder: Serve per leggere la rotazione dell encoder
* click: permette di leggere il pulsante. Viene rilevato in singolo cambio di stato.
* initPlayer: inizializza l'array Player.
* sort: ordina l'Array Player (BSort).
* setClassifica: riempie l' array classifica.
* STScore: restituisce una string che rappresenta lo stato della partita
* ST: restituisce la classifica sotto forma di stringa.
* SB: restituisce la corrispondenza con ST.
* headSpace: aggiunge degli spazi per formattare le stringhe informative dello stato Win.


## Aggiornamento v1
Introdotta la schermata di benvenuto e migliorata alcune visualizzazione sullo schermo.
I view:
* viewScore: visualizza lo stato dei punteggi dei giocatori.
* WelcomeMessage: visualizza un messaggio di benvenuto.
* GoodByMessage: visualizza la schermata terminale.


##  Aggiornamento v2
Impostato la possibilita di avere un timer che segna la durata della partita.
I view:
* viewTimer: visualizza la durata della partita.

I metodi:
* startTime: imposta l'inzio del timer.
* stopTimer: ferma il timer.
