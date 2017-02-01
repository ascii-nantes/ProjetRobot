int auto_etat;
void setup_autonome()
{
  auto_etat = AUTO_NORM;
}

void set_auto(char *mode)
{
  if ((strcmp(mode,"ON")==0) || (strcmp(mode,"on")==0))
    auto_etat = AUTO_NORM;
  if ((strcmp(mode,"OFF")==0) || (strcmp(mode,"off")==0))
    auto_etat = AUTO_DISABLE;
}    
void autonome()
{
int  dist;
int dir;

  //Serial.print("----------------");
  //Serial.println(auto_etat);
  switch (auto_etat)
  {
    case AUTO_ROTATE:
      if (get_mstate() != HALT) return;
      // rotation terminer, retour etat normal
      auto_etat = AUTO_NORM;
      // attention pas de break, on peut passer 
      // directement a l'etat NORMAL
    case AUTO_NORM:
      dist = get_dist();
      reset_dist();
      //Serial.print("===========");
      //Serial.println(dist);
      if (dist==0) break; // pas d'info de distance dispo
      if (dist<20)
      {
        // distance inférieur a 20 cm
        // rotation aléataoire
        dir=random(-100,100);
        if (dir<0) {set_rotate(RIGHT,-dir);auto_etat=AUTO_ROTATE;}
        if (dir>0) {set_rotate(LEFT,dir);;auto_etat=AUTO_ROTATE;}
        // si dir= 0 , on attend le prochain coup
        break; // sortie du switch
      }
      if (dist<100)
      {
        // a moins de 1 metre on passe a vitesse reduite
        set_speed(FORWARD,HALF);
        break;
      }
      // pas d'obstacle a moins d'un metre, full speed
      set_speed(FORWARD,FULL);
      break;
    case AUTO_DISABLE:
      // ne fait rien :)
      break;
  }
}    
