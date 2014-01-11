/*
BOMBARDINO
*/

#include <TM1638.h>
	TM1638 module(12, 13, 11);
int check;
String descr = "val";
int flag =0;
int flag_ready =0;
int status = 1;// stato dello schermo LCD
int V_ore = 0;
int V_minuti = 90;
int V_secondi = 0;
int V_delay = 15;
int V_apertura = 0;
int V_start = 0 ;
int uscita = 1;
int V_fili[5] = {1, 2, 3, 4, 5};
int V_Efili[5] = {0, 0, 0, 0, 0};
long V_tempo , V_tempoLED , V_tempolampeggio;
int V_velocita =1000;
int V_fade=0;
int V_fadevel=3;
int V_accensione =0;
int temp =0;
char str[53]="-.-.-.-BombardIno.-.-.-.-InSErIrE MaStEr key.-.-.-.-";

void setup() {
	randomSeed(analogRead(1));
	Serial.begin(9600);
	pinMode(2, INPUT);//filo1
	pinMode(3, INPUT);//filo2
	pinMode(4, INPUT);//filo3
	pinMode(5, INPUT);//filo4
	pinMode(6, INPUT);//filo5
	pinMode(7, INPUT);//apertura valietta
	pinMode(8, OUTPUT);//relè
	pinMode(9, OUTPUT);//led 1
	pinMode(10, OUTPUT);//buzzer
	V_tempo=millis();
	V_tempoLED = millis();
	V_tempolampeggio = millis();
	f_mescola(V_fili,5,3);


}

void f_accensione(){
int i=0;
while(!(analogRead(A4) > 845 && analogRead(A4) < 865)){
	module.clearDisplay();
	module.setDisplayToString(String(str[i+7]),0,7);
	module.setDisplayToString(String(str[i+6]),0,6);
	module.setDisplayToString(String(str[i+5]),0,5);
	module.setDisplayToString(String(str[i+4]),0,4);
	module.setDisplayToString(String(str[i+3]),0,3);
	module.setDisplayToString(String(str[i+2]),0,2);
	module.setDisplayToString(String(str[i+1]),0,1);
	module.setDisplayToString(String(str[i]),0,0);
	delay(potenziometro(A0,0,1000));
	i++;
	if(i>44)i=0;
	}
	V_accensione=1;
}

int f_pulsante(){
	delay(10);
	int valore = analogRead(A5);
	delay(10);
//Serial.println(valore);
	if(valore > 825 && valore < 845 && flag != 1){
//Serial.println(" enter ");
//Serial.println(valore);
		delay(10);
		valore = analogRead(A5);
		delay(10);
		while(valore > 825 && valore < 845){
			delay(10);
			valore = analogRead(A5);
			delay(10);
			flag = 1;
		}
//Serial.println("W l'ENTER");
		flag = 0;
//Serial.println("1");
		return (1);
	}

	else  if(valore > 450 && valore < 475 && flag != 1){
//Serial.println(" SET ");
//Serial.println(valore);
		delay(10);
		valore = analogRead(A5);
		delay(10);
		while(valore > 450 && valore < 475){
			delay(10);
			valore = analogRead(A5);
			delay(10);
			flag = 1;
		}
//Serial.println("W la il SET");
		flag = 0;
//Serial.println("2");
		return(2);
	}
}
void loop() {
	if(V_accensione == 0)f_accensione();
	f_menu();
	if(V_delay != 0)f_delay_partita();
	if(V_apertura != 0) f_apertura_partita();
	f_principale();
	//int prima =analogRead(A4);
	//if(prima > (temp +10) || prima < (temp -10) ){
	//temp =prima;
	//Serial.println(prima);
	//}
}


int potenziometro( char pin, int min, int max){  
//con questa procedura leggo 10 risultati a distanza di 10 millisec e faccio una media
	int risultato = 0;
	int pot=0;
	for(int i=0;i<11;i++){
	delay(10);
	pot=constrain(analogRead(pin),0,1000);
	//Serial.println(pot);
	risultato = risultato + map(pot,0,1000,min,max);
	delay(10);
	}
	risultato=risultato/10;
	
	return constrain(risultato,min,max);
}

void lcd2(String scritta,int numero){
	module.setDisplayToString(scritta, 0);
	if(numero > 9) module.setDisplayToString(String(numero), 0, 6);
	else {
		module.clearDisplayDigit(6,false);
		module.setDisplayToString(String(numero), 0, 7);  
	}
}

int f_delay(){

	int pot =potenziometro(A0,0,60);
		lcd2("",pot);
	return pot;
}

int f_Asino(){

	int pot =potenziometro(A0,0,1);
		if(pot != 0){
			module.setDisplayToString("ApEr  Si");
		}
		else module.setDisplayToString("ApEr  NO");
	return pot;

}

int f_oms (int valore){ //funzione OreMinutiSecondi
	//per le ore valore 2 per i minuti valore 1 per i secondi valore 0
	int pot = 0;
	switch (valore){
	
	case 2://le ore massimo consentito 36 ore di gioco 
	pot = potenziometro(A0,0,36);
		//module.setDisplayToString(String((pot)),0,0);
		if(pot > 9) module.setDisplayToString(String(pot), 0, 0);
		else {
		module.clearDisplayDigit(0,false);
		module.setDisplayToString(String(pot), 0, 1);  
		}
		return pot;
	break;	
	
	case 1://per i minuti
	if(V_ore != 0){//se sono state impostate le ore non sarà disponobile selezioare più di 59 minuti
		pot = potenziometro(A0,0,59);
		if(pot > 9) module.setDisplayToString(String(pot), 0, 3);
		else {
		module.clearDisplayDigit(3,false);
		module.setDisplayToString(String(pot), 0, 4);  
		}
		return pot;
		break;
			}
	
		else{ //in caso contrio i minuti posso arrivare fino a 90
		pot = potenziometro(A0,0,90);
		if(pot > 9) module.setDisplayToString(String(pot), 0, 3);
		else {
		module.clearDisplayDigit(3,false);
		module.setDisplayToString(String(pot), 0, 4);  
		}
		return pot;
				
		}
		
	case 0://per i secondi fino a un massimo d 59
		pot = potenziometro(A0,0,59);
		if(pot > 9) module.setDisplayToString(String(pot), 0, 6);
		else {
		module.clearDisplayDigit(6,false);
		module.setDisplayToString(String(pot), 0, 7);  
		}
		return pot;
	break;
		}
}
	

int f_Ssino(){

	int pot =potenziometro(A0,0,1);
		if(pot != 0){
			module.setDisplayToString("Start Si");
		}
		else module.setDisplayToString("Start NO");
	return pot;//ritorna una divisione intera quindi o l valore 0 o 1

}

void f_scambio(int *vettore, int pc, int pd)//passa un vettore come argomento con indice pc posiione corrente e destinato a indice pd posizione desiderata
{
int temp = vettore[pd];
vettore[pd] = vettore [pc];
vettore [pc] = temp;
}

int f_menu(){
while(uscita != 7){
	int pulsante = 0;
	switch (uscita){
	
	case 1://settaggio dei fili di bombardino, viene generata una funzione stadar, in caso contrario, si può modificarli manualmente
	module.clearDisplay();//pulisci il menù
	lcd2("s.Filo",V_fili[1]);//viene visualizzato il filo Boooom
	//prima voce del menù
	while((pulsante != 1) && (pulsante != 2)){
			pulsante = f_pulsante();
		}
	if(pulsante == 2){//ramo 1a
			Serial.println("ramo 1a modifica dei fili");
			module.clearDisplay();
			pulsante = 0;
			int temp;
			int old_temp=0;
			while(pulsante != 1){
				pulsante = f_pulsante();
				temp = potenziometro(A0,0,4);
				lcd2("BUOnO",V_fili[temp]);
				f_lampeggio(821,7,7);
				
				}
				f_scambio(V_fili,temp,4);
				module.clearDisplay();
				pulsante = 0;
				old_temp=5;
				while(pulsante != 1){
				pulsante = f_pulsante();
				temp =potenziometro(A0,0,3);
				lcd2("1TeMPO",V_fili[temp]);
				f_lampeggio(821,7,7);
				}
				f_scambio(V_fili,temp,3);
				module.clearDisplay();
				pulsante = 0;
				old_temp=5;
				while(pulsante != 1){
				pulsante = f_pulsante();
				temp =potenziometro(A0,0,2);
				lcd2("rApIdO",V_fili[temp]);
				f_lampeggio(821,7,7);	
				}
				f_scambio(V_fili,temp,2);
				module.clearDisplay();
				pulsante = 0;
				old_temp=5;
				while(pulsante != 1){
				pulsante = f_pulsante();
				temp =potenziometro(A0,0,1);
				lcd2("botto",V_fili[temp]);
				f_lampeggio(821,7,7);
				}
				f_scambio(V_fili,temp,1);
				module.clearDisplay();
				pulsante = 0;
				for(int a=0;a<5;a++){
				Serial.print(V_fili[a]);
				}
			
		}
		else if(pulsante == 1){
		uscita++;
		break;
		}
		
	case 2://viene settato se l'apertura innesca la partenza del tempo, se viene inserita l'apertura a innesco non è possibile inserre il delay
	//ramo 2
	pulsante = 0;
	Serial.println("Ramo 2");
	module.clearDisplay();//pulisci il menù
	if(V_apertura != 0) module.setDisplayToString("ApEr  Si");
		else module.setDisplayToString("ApEr  NO");
	
		
	while((pulsante != 1) && (pulsante != 2)){
		pulsante = f_pulsante();
		}
	if(pulsante == 2){
	//ramo 2a il potenziometro modifica il parametro apertura
			Serial.println("ramo 2a");
			pulsante = 0;
			while(pulsante != 1){
				pulsante = f_pulsante();
				V_apertura = f_Asino();
				f_lampeggio(821,6,7);
				
				}
			Serial.println(V_apertura);
		}
	else if(pulsante == 1){
		uscita++;
		break;
		}
		
	case 3://dove viene settato il tempo
	//ramo3
	if (V_apertura == 1) {
	uscita = 4;
	V_delay= 0;
	}
	pulsante = 0;
	Serial.println("Ramo 3");
	module.clearDisplay();//pulisci il menù
	lcd2("tEMpO",V_minuti);//stampo schermata tempo 90 miunti
	while((pulsante != 1) && (pulsante != 2)){
	pulsante = f_pulsante();
	}
	if(pulsante == 2){
		//ramo 3a modifica del TEMPO
		Serial.println("ramo 3a");
		module.setDisplayToString("HH.mm.ss");
		pulsante = 0;
			while(pulsante != 1){
				//ore
				pulsante = f_pulsante();
				f_lampeggio(821,0,1);
				V_ore = f_oms(2);
				
				}
				
				pulsante = 0;
				while(pulsante != 1){
				//minuti
				pulsante = f_pulsante();
				f_lampeggio(821,3,4);
				V_minuti = f_oms(1);
				
				}
				pulsante = 0;
				while(pulsante != 1){
				//ore
				pulsante = f_pulsante();
				f_lampeggio(821,6,7);
				V_secondi = f_oms(0);
				
				}
				Serial.println(V_ore);
				Serial.println(V_minuti);
				Serial.println(V_secondi);
				}
		else if(pulsante == 1){
	
		uscita++;
		Serial.println(uscita);
		break;
		}
		
	case 4:
	//ramo4
	pulsante = 0;
	Serial.println("ramo 4");
	module.clearDisplay();//pulisci il menù
	lcd2("dElay",V_delay);//stampo delay 15 minuti
	while((pulsante != 1) && (pulsante != 2)){
	pulsante = f_pulsante();
	}
	if(pulsante == 2){
	//ramo 4a potenzinziometro modifica il tempo di delay
		Serial.println("ramo 4a");
		pulsante = 0;
		while(pulsante != 1){
		pulsante = f_pulsante();
		V_delay = f_delay();
		f_lampeggio(821,6,7);
		
		//stampo, non risco capire com mai viene aggiorata i tempo reale al cambio della varibile... funziona lo lascio così(scemo guarda il codice -____-)
						}
		Serial.println(V_delay);  
					}
	else if(pulsante == 1){
		uscita++;
		break;
		}

	case 5:
	//ramo 5
	pulsante = 0;
	Serial.println("ramo 5");
	module.clearDisplay();//pulisci il menù
	if(V_start != 0){
		module.setDisplayToString("Start Si");
		}
		else module.setDisplayToString("Start NO");
			while((pulsante != 1) && (pulsante != 2)){
				pulsante = f_pulsante();
				}
			if(pulsante == 2){
			//ramo 5a
				Serial.println("ramo 5a");
				pulsante = 0;
				while(pulsante != 1){
					pulsante = f_pulsante();
					V_start = f_Ssino(); 
					f_lampeggio(821,6,7);
					
							}
							Serial.println(V_start);
						}	
			else if(pulsante == 1){
			uscita++;
			if (V_start == 0) uscita = 1 ;
			break;
			}
				
			case 6:
			//ready
			int display =1;
			int stato = 0;
			V_tempolampeggio = millis();
			stato = analogRead(A4);
			while (stato >835 && stato < 875){
			stato = analogRead(A4);
				if((millis()-V_tempolampeggio)>800){
				V_tempolampeggio = millis();
				if(display == 0){
					module.clearDisplay();
					display++;
					}
					else{
						module.setDisplayToString("--Ready-");
						display--;
						}	
			
					}
			}
			uscita =7;
			break;
			
		}//dello switch
	}//del while
	Serial.println(uscita);
	/*if(uscita==6){
	Serial.println("-------al naturale-------");
	for(int a = 0; a < 5;a++){Serial.print(V_fili[a]);}
	Serial.println("-------------");
	f_mescola(V_fili,5,3);
	for(int a = 0; a < 5;a++){Serial.print(V_fili[a]);}
	Serial.println("------prima rimescolata---------");

	uscita++;
	}*/
}//della fx



void f_mescola(int *vettore, int elem, int ripetizione)
{
for (ripetizione;ripetizione > 0; ripetizione --){
for(int ripeti=random(10); ripeti>0;ripeti--){

  for (int a=elem-1; a>0; a--)
  {
    int r = random(0,5);
    
      int temp = vettore[a];
      vettore[a] = vettore[r];
      vettore[r] = temp;
  }
 }
}
}

void f_principale(){
for(int i=0; i<5;i++){
if(digitalRead((V_fili[i])+1) == 0){
if(V_Efili[i]==0){
V_Efili[i]=1;
f_evento(i);
}
}
}
if((millis()- V_tempo)>V_velocita){
	V_tempo = millis();
	V_secondi--;
	Serial.println(V_secondi);
	if(V_secondi <= 0){
		if(V_minuti==0){
			if(V_ore==0)f_evento(1);
			else{
			V_ore--;
			V_minuti =59;
			V_secondi=59;
			}
		}
		else{
		V_minuti--;
		V_secondi =59;
		}
	}
	
	f_lcdtempo(V_ore,V_minuti,V_secondi);
	if(V_secondi%2 ==0)digitalWrite(10,HIGH);
	else digitalWrite(10,LOW);
	}

if((millis() - V_tempoLED)>25){
	V_tempoLED = millis();
	analogWrite(9,V_fade);
	//analogWrite(10,V_fade);
	
	V_fade = V_fade + V_fadevel;
	if(V_fade <= 0 || V_fade >= 255) V_fadevel = -V_fadevel;
	}
	
int cheat = analogRead(A4);
//if(!(cheat > 301 && cheat < 325)) f_evento(1);//guarda se è presente la resistenza da 220K
if(cheat == 0)f_evento(1);
}

void f_lcdtempo(int O,int M, int S ){
module.setDisplayToString("-", 0, 2); 
if( O > 9) module.setDisplayToString(String(O), 0, 0);
		else {
		module.clearDisplayDigit(0,false);
		module.setDisplayToString(String(0), 0, 0); 
		module.setDisplayToString(String(O), 0, 1);  
		}
module.setDisplayToString("-", 0, 5); 
if( M > 9) module.setDisplayToString(String(M), 0, 3);
		else {
		module.clearDisplayDigit(3,false);
		module.setDisplayToString(String(0), 0, 3); 
		module.setDisplayToString(String(M), 0, 4);  
		}
if( S > 9) module.setDisplayToString(String(S), 0, 6);
else {
module.clearDisplayDigit(6,false);
module.setDisplayToString(String(0), 0, 6); 
module.setDisplayToString(String(S), 0, 7);  
		}
}

void f_evento(int evento){/*
Serial.println("-------------------");
Serial.println(digitalRead((V_fili[i])+1));
Serial.print("filo staccato");
Serial.println(i);
Serial.print("filo assegnato");
Serial.println(V_fili[i]);
Serial.println("-------------------");
*/
int display =1;
switch (evento){
	case 0://il filo buffo, il monitor si spegne per qualche secondo dando l'impressione della vincita poi il tempo viene dimezzato.
	while( (millis() - V_tempo )< 8000){
	if(digitalRead((V_fili[1])+1) == 0){//interrompo in caso venga tagliato il filo boooom
	module.setupDisplay(true, 7);
	break;}
	if(digitalRead((V_fili[4])+1) == 0){//interrmpo in caso aver tagliato il filo vittoria
	module.setupDisplay(true, 7);
	break;}
	module.setupDisplay(false, 0);
	analogWrite(9,0);
	digitalWrite(10,LOW);
	}
	module.setupDisplay(true, 7);
	V_ore=V_ore/2;
	V_minuti=V_minuti/2;
	V_secondi=V_secondi/2;
	break;

	case 1://esplosione della bomba
	V_ore=0;
	V_minuti=0;
	V_secondi=0;
	//mando il segnale alto sul pin8?
	digitalWrite(8,HIGH);
	digitalWrite(10,HIGH);
	module.setDisplayToString("b000000m");
	while (true){
		if((millis()- V_tempo)>30000){
		//mando il segnale basso sul pin8?
		digitalWrite(8,LOW);
		digitalWrite(10,LOW);
		}

		if((millis()-V_tempolampeggio)>300){
		V_tempolampeggio = millis();
		if(display == 0){
			module.clearDisplay();
			display++;
			}
			else{
			module.setDisplayToString("b000000m");
			display--;
			}
		}
		analogWrite(9,255);
	}
	break;

	case 2://dimezza il tempo
	V_ore=V_ore/2;
	V_minuti=V_minuti/2;
	V_secondi=V_secondi/2;
	break;

	case 3:
	V_velocita = 17;
	break;

	case 4:
	module.setDisplayToString("vittoriA");
	while (true){
		if((millis()-V_tempolampeggio)>1000){
		V_tempolampeggio = millis();
		if(display == 0){
			module.clearDisplay();
			display++;
			}
			else{
			module.setDisplayToString("vittoriA");
			display--;
			}
		}
		analogWrite(9,0);
		digitalWrite(10,LOW);
	}
	break;
	}
}



void f_lampeggio(int lampeggio, int caratteriI, int caratteriF){
	if((millis() - V_tempolampeggio)> lampeggio){
	V_tempolampeggio = millis();
	for(;caratteriI<=caratteriF;caratteriI++){
	module.clearDisplayDigit(caratteriI,false);
	}
	
	} 	
}

void f_delay_partita(){
int secondi=0;
int flag =1;
while(flag != 0){
if((millis()- V_tempo)>V_velocita){
	V_tempo = millis();
	secondi--;
	if(secondi <= 0){
		if(V_delay==0)flag = 0;
		else{
		V_delay--;
		secondi =59;
		}
	}
	
	if((secondi%2) == 0)f_lcdtempo(00,V_delay,secondi);
	else module.setDisplayToString("--dELAy-");
	
	}
	}

}

void f_apertura_partita(){
while(digitalRead(7) ==0 ){
module.setDisplayToString("ChIudErE");
}
module.clearDisplay();
while(digitalRead(7) ==1 ){
module.setDisplayToString("ArmAtA");
}
module.clearDisplay();
V_apertura = 0;
}


