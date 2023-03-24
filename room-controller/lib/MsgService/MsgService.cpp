#include "Arduino.h"
#include "MsgService.h"

String content;

MsgServiceClass MsgService;

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  // Serial.begin(9600);
  content.reserve(256);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n') {
      if (MsgService.currentMsg != NULL) {
        delete MsgService.currentMsg;
      }

      if (content[content.length() - 1] == '$') {
        content[content.length() - 1] = ':';
      } else {
        content += ':';
      }
      MsgService.currentMsg = new Msg(content);
      MsgService.msgAvailable = true;
      // content = ""; // setting content to "" here would be correct but it causes some weird bugs.
      // this translates to the fact that every Msg contains all the previous content and the last message should be extracted.
      // separating messages has been accomplished using this message separator => :
    } else {
      content += ch;  
    }
  }
}
