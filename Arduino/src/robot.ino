#include "const.h"
#define T_PRESCALE_MOTEUR 500
long time_prescaler_moteur;
long time_autonome;
/*
** partie interpretation de commandes
*/
char inData[80];  // buffer de saisie de message
byte index = 0;

void setup()
{
  Serial.begin(9600);  // port serie a vitesse 9600
  //
  // attachement d'interruption
  time_prescaler_moteur = 0;
  time_autonome = 0;
  //
  // setup moteur
  //
  setup_moteurs();
  //
  // setup distance
  //
  setup_distance();
  //
  // setup mode autonome
  //
  setup_autonome();
  //
  // programmation du compteur timer0
  //
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A); 
}

/*
** routine d'interruption toutes les millisecondes
*/
SIGNAL(TIMER0_COMPA_vect) 
{
  // mode autonome
  time_autonome++;
  if (time_autonome==100)
  {
    // le controle du mode autonome a lieu toutes
    // les 100 milliseconde et precede le
    // controle des moteurs afin que si une
    // décision est prise par le mode autonome elle
    // soit immediatement prise en compte
    autonome();
    time_autonome=0;
  }
  // controle des moteur
  time_prescaler_moteur++;
  if (time_prescaler_moteur==T_PRESCALE_MOTEUR)
  {
    controle_moteur(); // controle moteur pas a pas
    time_prescaler_moteur = 0; // RAZ prescaler moteur
  }
}
/*
** boucle principale
*/
void loop()
{
  /*
  ** lecture periodique de la distance
  */
  read_dist();
  /*
  ** traitement des entrees clavier
  */
  while(Serial.available() > 0)
  {   
    char aChar = Serial.read();
    if(aChar == '\n')
    {
      inData[index] = '\0'; // Keep the string NULL terminated
      // End of record detected. Time to parse
      parse_command(inData);
      index = 0;
      inData[index] = NULL;
    }
    else
    {
      inData[index] = aChar;
      index++;
    }
  }   
}


void parse_command(char *inData)
{
// analyse la chaine de caracteres de la forme commande(par,par,par)
char *comm;
char *arg;
char *sarg[10];
char *saveptr;
char *saveptr2;
char *str = inData;
int  index = 0;

  // extraction de la commande
  comm = strtok_r(str,"()",&saveptr);
  arg = strtok_r(NULL,"()",&saveptr);
  for (str=arg;;str=NULL)
  {
    sarg[index] = strtok_r(str,",",&saveptr2);
    if (sarg[index]==NULL) break;
    index++;
  }
  // analyse des commandes
  if ((strcmp(comm,"moteur?")==0) || (strcmp(comm,"m?")==0))
  {
    // controle moteurs
    report_moteur();
  }
  if ((strcmp(comm,"moteur")==0)| (strcmp(comm,"m")==0))
  {
    // controle moteurs
    set_moteur(sarg[0],sarg[1]);
  }

  if ((strcmp(comm,"tourne")==0) | (strcmp(comm,"t")==0))
  {
    // controle moteurs
    rot_moteur(sarg[0],sarg[1]);
  }
  if ((strcmp(comm,"dist?")==0) | (strcmp(comm,"d?")==0))
  {
    // controle moteurs
    disp_dist();
  }
  if ((strcmp(comm,"auto")==0) | (strcmp(comm,"a")==0))
  {
    // controle moteurs
    set_auto(sarg[0]);
  }
}
