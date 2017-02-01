int cur_dist;
void setup_distance()
{
  cur_dist = 0; // distance inconnue
  pinMode(dist_trigger,OUTPUT);pinMode(dist_reader,INPUT);
}

int read_dist()
{
  /*
  ** lecture periodique de la distance
  */
  digitalWrite(dist_trigger, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(dist_trigger, LOW); 
  // retourne la distance convertie en cm
  cur_dist= pulseIn(dist_reader, HIGH)/58;
  //Serial.println(cur_dist);
  return (cur_dist);
}

int get_dist()
{
  return cur_dist;
}

void reset_dist()
{
  cur_dist = 0;
}

void disp_dist()
{
  Serial.print("Distance = ");
  Serial.println(cur_dist);
}
