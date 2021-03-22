# GameCouterIno
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

## Opzione Back
Alcuni stati hanno l'opzione Back con la quale è possibile tornare allo stato precedente. Nello stato Select questa opzione dará la possibilita di ripetere l azione di selezionare quanti giocatori. Riselezionando azzererà i punteggi attuali comuque questa azione è seguita da una conferma.


![GitHub Logo](/Img/FSM-GameCounter.png)
