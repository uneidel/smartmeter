void doubleClick() {
   
    Serial.println("Main Button SinglePress");
    if (currentdisplay < 3)
    {
      currentdisplay++;  
    }
    else{ currentdisplay = 0;};
    dirty = true;
} 

void longClick(){

  SetupOTA();
}
void singleClick(){
  digitalWrite(D3, 1);
}

