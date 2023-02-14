// 1- Yuanxu Zhang - yzhan319 and Jack Li - wli200
// 2- Project - Ikun Joystick game
// 3- Description - Two player use joystick to play a game and answer a series of question to seehow much they know about each other.
// 4- Assumptions - Each play have one joystick to control the game.After the game they will have a score of how much they know about each other.


#include <LiquidCrystal.h>
#include <ArduinoJson.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int lcd1_rs = 12, lcd1_en = 11, lcd1_d4 = 5, lcd1_d5 = 4, lcd1_d6 = 3, lcd1_d7 = 2; //init for lcd screen 1
const int lcd2_rs = 10, lcd2_en = 9, lcd2_d4 = 13, lcd2_d5 = 6, lcd2_d6 = 7, lcd2_d7 = 8; //init for lcd screen 2 

LiquidCrystal lcd1(lcd1_rs, lcd1_en, lcd1_d4, lcd1_d5, lcd1_d6, lcd1_d7); //init for lcd screen 1
LiquidCrystal lcd2(lcd2_rs, lcd2_en, lcd2_d4, lcd2_d5, lcd2_d6, lcd2_d7); //init for lcd screen 2


int total_question = 6; //count of the total question that we have.
char serialState; // this is for the serialstate
char p1Choice = 'x'; //init for player choices
char p2Choice = 'x'; // init for player choices
char input;  // init to store input
int match = 0; // count of th ematch.
StaticJsonDocument<400> doc; //space for json format
int question_pos = 0; //the current question postiton
char* question; //to store question
char* answers; // to store ansers

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd1.begin(16, 2);
  lcd2.begin(16, 2);
  // where the question is stored at.
  char questions[] ="{\"data\":[{\"question\":\"What is Player1 When to complete assignments?\",\"answers\":\"A.Last minute B.Days before\"},{\"question\":\"What is Player2 Favorite Drinks?\",\"answers\":\"A.Juice    B.Water\"},{\"question\":\"What Player1 Always likes to\",\"answers\":\"A.Question    B.Answer\"},{\"question\":\"What does Player2 Perfers?\",\"answers\":\"A.Youtube B.TikTok\"},{\"question\":\"What does Player1 Perfers\",\"answers\":\"A.Apple Music B.Spotify\"},{\"question\":\"What does Player2 Perfers to Play? \",\"answers\":\"A.PS5 Game  B.Xbox Game\"}]}";
  DeserializationError error = deserializeJson(doc, questions);
  // Print a message to the LCD.
   if (error) { //catch error if the json is not read correctly.
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
  }
//  Serial.println("Before");
  initLCD(); //init lcd before players start playing
//  Serial.println("After");

}



void initLCD() {
  lcd1.setCursor(0,0);
  lcd1.print("Welcome! Press");
  lcd1.setCursor(0,1);
  lcd1.print("button1 to play");

  lcd2.setCursor(0,0);
  lcd2.print("Welcome! Press");
  lcd2.setCursor(0,1);
  lcd2.print("button1 to play");

  while (serialState != 's') { // if play is ready press s to start playing.
    if (Serial.available() > 0){
      serialState = Serial.read();
    }
  }
  lcd1.clear(); //clear out lcd and start playing.
  lcd2.clear();
}


void(* resetFunc)(void) = 0; // reset function

void check_input(){ // function to check into 
  if (Serial.available() > 0) { 
    input = Serial.read();
    if (input == 'l') { // if input is l player1 have choose a
      p1Choice = 'a';
    } else if (input == 'r') {// if input is l player1 have choose b
      p1Choice = 'b';
    } else if (input == 'L') {// if input is L player2 have choose a
      p2Choice = 'a';
    } else if (input == 'R') {// if input is L player2 have choose b
      p2Choice = 'b';
    }else if(input == 'a'){// if input is a that means players wants to reet the game.
      resetFunc(); /
    }
  }

  if (p1Choice != 'x' && p2Choice != 'x') {    //if p1 and p2 have choose a option that means we have to compare.
    question_pos++;
    if (p1Choice == p2Choice) { // if both player have to same choice add point to the player and send a serial to make a sound.
      Serial.print("Before p1Choice-");
      Serial.println(p1Choice); //
      Serial.print("Before p2Choice-");
      Serial.println(p2Choice);
      
      Serial.write('n');
      Serial.println("Match");
      match++;
    } else { // if not match goes to next question
      Serial.print("Before p1Choice-");
      Serial.println(p1Choice);
      Serial.print("Before p2Choice-");
      Serial.println(p2Choice);
      
      Serial.write('m');
      Serial.println("Not Match");      
    }
    // set choice to null and goes to next question.
    p1Choice = 'x';
    p2Choice = 'x';
    question = doc["data"][question_pos]["question"];
    answers = doc["data"][question_pos]["answers"];
    // clear out lcd screen.
    lcd1.clear();
    lcd2.clear();
  
    lcd1.setCursor(0, 0);
    lcd1.print(question); //display question
    lcd2.setCursor(0, 0);
    lcd2.print(question);
  
    lcd1.setCursor(0, 1);
    lcd1.print(answers);// dipslay ansers.
    lcd2.setCursor(0, 1);
    lcd2.print(answers);
  }
  if(total_question == question_pos){ // if questions have reach to the end ask the player if they want to reset.
        while(true){ // wait till players gives a command.
          lcd1.clear();
          lcd2.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("Total Score:");
          lcd1.print(match);
          lcd1.setCursor(0, 1);
          lcd1.print("Press To Reset");
          lcd2.setCursor(0, 0);
          lcd2.print("Total Score:");
          lcd2.print(match);
          lcd2.setCursor(0, 1);
          lcd2.print("Press To Reset");
          delay(450);
          if (Serial.available() > 0) {
            input = Serial.read();
            if(input == 'a'){ // if palyer want to rest call the reset function.
              resetFunc();
            }
          }
        }
      }
}

void loop() {
  question = doc["data"][question_pos]["question"]; //init questions and answers
  answers = doc["data"][question_pos]["answers"];
  
  lcd1.clear();
  lcd2.clear(); // clears out lcd and display questions and answers.

  lcd1.setCursor(0, 0);
  lcd1.print(question);
  lcd2.setCursor(0, 0);
  lcd2.print(question);

  lcd1.setCursor(0, 1);
  lcd1.print(answers);
  
  lcd2.setCursor(0, 1);
  lcd2.print(answers);

  for (int i = 0; i <= strlen(question) - 16; i++) // print 0 to 16 letter
  {
    lcd1.setCursor(0, 0);
    lcd2.setCursor(0, 0);

    for (int j = i; j <= i + 15; j++)  // print only 16 characters
    {
      lcd1.print(question[j]); //output the letter.
      lcd2.print(question[j]); //output the letter.
      check_input();
    }
    check_input();
    delay(450);
  }

 for (int i = 0; i <= strlen(answers) - 16; i++) // print 0 to 16 letter
  {
    lcd1.setCursor(0, 1);
    lcd2.setCursor(0, 1);

    for (int j = i; j <= i + 15; j++)  // print only 16 characters
    {
      lcd1.print(answers[j]); //output the letter.
      lcd2.print(answers[j]); //output the letter.
      check_input();
    }
    check_input();

    delay(450);
  }

  
  
  delay(500);

}
