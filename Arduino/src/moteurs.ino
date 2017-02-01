#include "const.h"
int step_a;    // position moteur A
int step_b;    // position moteur B
int m_etat;    // etat des moteurs (HALT,HALTING,ROTATE,...)
int m_sens;    // AVANT ARRIERE
int m_sens_a;  // sens moteur A
int m_sens_b;  // sens moteur B
int m_allure;  // allure des moteur (FULL,HALF,QUARTER)
int m_scaler;  // scaling moteur (FULL=1, HALF=2,...)
int m_tick  ;  // ticker moteur
int h_step  ;  // compteur utilise pour les arret
int r_step  ;  // compteur utiliser pour les rotation

void setup_moteurs()
{
  m_tick = 0;
  m_etat = HALT;
  step_a = 0; step_b = 0;
  // assignation des pins de controle moteur
  pinMode(pinA_0,OUTPUT);pinMode(pinA_1,OUTPUT);
  pinMode(pinA_2,OUTPUT);pinMode(pinA_3,OUTPUT);
  pinMode(pinB_0,OUTPUT);pinMode(pinB_1,OUTPUT);
  pinMode(pinB_2,OUTPUT);pinMode(pinB_3,OUTPUT);
  shut_down();
} 

void shut_down()
{
  digitalWrite(pinA_0,0);digitalWrite(pinA_1,0);
  digitalWrite(pinA_2,0);digitalWrite(pinA_3,0);
  digitalWrite(pinB_0,0);digitalWrite(pinB_1,0);
  digitalWrite(pinB_2,0);digitalWrite(pinB_3,0);
} 
  
int get_mstate()
{
  return m_etat;
}

void set_speed(int sens,int allure)
{
  //Serial.print(sens); Serial.print( " ");Serial.println(allure);

  switch (allure)
  {
    case FULL:    m_scaler=1; break;
    case HALF:    m_scaler=2; break;
    case QUARTER: m_scaler=4; break;
  }
  Serial.println(m_scaler);
  m_allure = allure;
  if (sens != m_sens)
  {
    h_step = 10;
    m_etat = HALT_INV;
  } 
  else m_etat = DRIVE;
  m_sens = sens;
  m_sens_a = sens; m_sens_b = sens;
  
}  

void set_rotate(int dir,int pas)
{
  r_step = pas;
  m_scaler = 4;
  if (dir=LEFT)
  {
    m_sens_a = FORWARD; m_sens_b = BACKWARD;
  }
  else
  {
    m_sens_b = FORWARD; m_sens_a = BACKWARD;
  }
  m_etat = ROTATE;
}  
void controle_moteur()
{
  m_tick++;
  switch (m_etat)
  {
    case HALT: 
      // les moteurs sont a l'arret ne rien faire
      shut_down();
      break;
    case HALTING:
      // tempo imposée avant de passer dans l'etat HALT
      h_step--;
      // si la tempo est echue on passe en etat HALT
      if (h_step <= 0) m_etat=HALT;
      break;
    case ROTATE:
      // rotation de n step en cours
      // actionne les moteur en fonction de l'allure
      if (m_tick>=m_scaler)
      {
        mota_step(); motb_step();
        m_tick = 0;
        // decrement le compteur de steps
        r_step--;
        if (r_step <= 0) 
        {
          h_step = 10;
          m_etat=HALTING;
        }
      }
      break;
    case DRIVE:
      // fonctionnement normal
      if (m_tick>=m_scaler)
      {
        mota_step(); motb_step();
        m_tick = 0;
      }
      break;
    case HALT_INV:
      // tempo imposée avant changement de sens
      h_step--;
      // si la tempo est echue on passe en etat DRIVE
      if (h_step <= 0)
      {
        shut_down();
        m_etat=DRIVE;
      }
      break;
  }
}

void mota_step()
{
  Serial.println(step_a);
  switch (step_a)
  {
    case 0:
      if (m_sens_a == FORWARD)
      {
        digitalWrite(pinA_3,0);
        digitalWrite(pinA_0,1);
        step_a = 1;
      }
      else
      {
        digitalWrite(pinA_1,0);
        digitalWrite(pinA_0,1);
        step_a = 3;
      }
      break;
    case 1:
      if (m_sens_a == FORWARD)
      {
        digitalWrite(pinA_0,0);
        digitalWrite(pinA_1,1);
        step_a = 2;
      }
      else
      {
        digitalWrite(pinA_2,0);
        digitalWrite(pinA_1,1);
        step_a = 0;
      }
      break;
    case 2:
      if (m_sens_a == FORWARD)
      {
        digitalWrite(pinA_1,0);
        digitalWrite(pinA_2,1);
        step_a = 3;
      }
      else
      {
        digitalWrite(pinA_3,0);
        digitalWrite(pinA_2,1);
        step_a = 1;
      }
      break;
    case 3:
      if (m_sens_a == FORWARD)
      {
        digitalWrite(pinA_2,0);
        digitalWrite(pinA_3,1);
        step_a = 0;
      }
      else
      {
        digitalWrite(pinA_0,0);
        digitalWrite(pinA_3,1);
        step_a = 2;
      }
      break;
  }   
}

void motb_step()
{
  switch (step_b)
  {
    case 0:
      if (m_sens_b == FORWARD)
      {
        digitalWrite(pinB_3,0);
        digitalWrite(pinB_0,1);
        step_b = 1;
      }
      else
      {
        digitalWrite(pinB_1,0);
        digitalWrite(pinB_0,1);
        step_b = 3;
      }
      break;
    case 1:
      if (m_sens_b == FORWARD)
      {
        digitalWrite(pinB_0,0);
        digitalWrite(pinB_1,1);
        step_b = 2;
      }
      else
      {
        digitalWrite(pinB_2,0);
        digitalWrite(pinB_1,1);
        step_b = 0;
      }
      break;
    case 2:
      if (m_sens_b == FORWARD)
      {
        digitalWrite(pinB_1,0);
        digitalWrite(pinB_2,1);
        step_b = 3;
      }
      else
      {
        digitalWrite(pinB_3,0);
        digitalWrite(pinB_2,1);
        step_b = 1;
      }
      break;
    case 3:
      if (m_sens_b == FORWARD)
      {
        digitalWrite(pinB_2,0);
        digitalWrite(pinB_3,1);
        step_b = 0;
      }
      else
      {
        digitalWrite(pinB_0,0);
        digitalWrite(pinB_3,1);
        step_b = 2;
      }
      break;
  }   
}

void report_moteur()
{
String res;
  res = "Etat courant :";
  switch (m_etat)
  {
    case HALT:  res +="HALT "; break;
    case HALTING:  res +="HALTING "; break;
    case ROTATE:  res +="ROTATING "; break;
    case DRIVE:  res +="DRIVE "; break;
    case HALT_INV: res+="INVERTING "; break;
  }
  Serial.println(res);  
  if (m_etat == DRIVE)
  {
    switch (m_allure)
    {
      case FULL:     res= "FULL "; break;
      case HALF:     res= "FULL "; break;
      case QUARTER:  res= "QUARTER "; break;
    }
    if (m_sens==FORWARD)
      res += "FORWARD ";
    else
      res += "BACKWARD ";
    Serial.println(res);
  }
}
void set_moteur(char *sens, char *allure)
{
int vsens;
int vallure;
  //Serial.print(sens); Serial.print( " ");Serial.println(allure);
  if (strcmp(sens,"HALT")==0)
  {
    h_step = 10;
    m_etat = HALTING;
    return;
  }
  vsens=-1; vallure=-1;
  if (strcmp(sens,"FORWARD")==0)    vsens=FORWARD;
  if (strcmp(sens,"F")==0)          vsens=FORWARD;
  if (strcmp(sens,"BACKWARD")==0)   vsens=BACKWARD;
  if (strcmp(sens,"B")==0)          vsens=BACKWARD;
  if (strcmp(allure,"FULL")==0)     vallure=FULL;
  if (strcmp(allure,"F")==0)        vallure=FULL;
  if (strcmp(allure,"HALF")==0)     vallure=HALF;
  if (strcmp(allure,"H")==0)        vallure=HALF;
  if (strcmp(allure,"QUARTER")==0)  vallure=QUARTER;
  if (strcmp(allure,"Q")==0)        vallure=QUARTER;
  //Serial.print(vsens); Serial.print( " ");Serial.println(vallure);

  if ((vsens != -1) && (vallure !=-1)) set_speed(vsens,vallure);
}
void rot_moteur(char *sens, char *pas)
{
int vpas;

  vpas = 0;
  vpas = atoi(pas);
  // test des erreurs de conversion
  if (vpas == 0) return;
  if ((strcmp(sens,"D")==0) || (strcmp(sens,"R")==0))
  {
    r_step = vpas;
    m_scaler = 4;
    m_sens_a = FORWARD; m_sens_b = BACKWARD;
    m_etat = ROTATE;
  }
  if ((strcmp(sens,"G")==0) || (strcmp(sens,"L")==0))
  {
    r_step = vpas;
    m_scaler = 4;
    m_sens_b = FORWARD; m_sens_a = BACKWARD;
    m_etat = ROTATE;
  }
}
