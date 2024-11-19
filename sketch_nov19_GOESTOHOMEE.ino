#include <Arduino.h>

// GLOBAL VARIABLES
int placeholder_1;
int placeholder_2;
int placeholder_3;
int placeholder_4;
int placeholder_5;
#include <Servo.h>
Servo myservo;
int pos = 35;
const int stepPin1 = 2;  // Step pin for Motor 1 (left = HIGH, right = LOW)
const int dirPin1 = 3;   // Direction pin for Motor 1
const int stepPin2 = 6;  // Step pin for Motor 2 (up = LOW, down = HIGH)
const int dirPin2 = 7;   // Direction pin for Motor 2
bool grab = true;
// FUNCTION PROTOTYPES
void print_blocks(int matrix[3][3]);
bool same_colour_column(int matrix[3][3], int col);
int check_for_success(int matrix[3][3]);
int column_count(int matrix[3][3], int col);
int find_most_empty_column(int matrix[3][3]);
int find_secound_most_empty_column(int matrix[3][3]);
void place_in_lowest_empty(int matrix[3][3], int block, int col);
int find_in_lowest_empty(int matrix[3][3], int col);
int count_top_row_blocks(int matrix[3][3]);
int find_top_left(int matrix[3][3]);
void move_top_left(int matrix[3][3]);
int check_row2(int matrix[3][3]);
int find_first_row2(int matrix[3][3], int colour);
int find_secound_row2(int matrix[3][3], int colour);
void move_first_row2(int matrix[3][3], int colour);
void move_empty_column(int matrix[3][3]);
void find_stacked_matching_blocks_col(int matrix[3][3], int col_first, int col_second);
int find_most_full_column(int matrix[3][3]);
int find_last_dupe(int matrix[3][3], int colour);
int blocks_above_dupe(int matrix[3][3], int col_first, int col_second, int row_value);
int find_last_block(int matrix[3][3]);
void moveBlock(int row, int col);

void setup() {

  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  myservo.attach(10);
  // Move to home position slowly at startup
  goHome();

    Serial.begin(9600); // Initialize serial communication

    // PRINT ORIGINAL SETUP
    int blocks[3][3] = {
        {0, 1, 0},
        {2, 1, 0},
        {2, 2, 1}
    };

    int success = 0;

    print_blocks(blocks);
    Serial.println("-------------------------");
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }

    // PART 1 (CLEARING TOP ROW)
    bool statement = true;
    placeholder_3 = find_most_empty_column(blocks);
    
    while (statement) {
        int num_blocks_top_row = count_top_row_blocks(blocks);

        switch (num_blocks_top_row) {
            case 0:
                statement = false;
                break;
            case 1:
                placeholder_2 = find_top_left(blocks);
                placeholder_4 = find_in_lowest_empty(blocks, placeholder_3);
                move_top_left(blocks);
                moveBlock(0, placeholder_2); // pick up 
                moveBlock(placeholder_4, placeholder_3); // place
                print_blocks(blocks);
                Serial.println("-------------------------");
                break;
            case 2:
                placeholder_1 = find_top_left(blocks);
                placeholder_4 = find_in_lowest_empty(blocks, placeholder_3);
                move_top_left(blocks);
                moveBlock(0, placeholder_1); // pick up
                moveBlock(placeholder_4, placeholder_3); // place
                print_blocks(blocks);
                Serial.println("-------------------------");
                break;
        }
    }
    
    // PART 2 (CHECKING ROW 2)
    int dup_block_colour = check_row2(blocks);
    int new_empty_col = find_first_row2(blocks, dup_block_colour);
    int stacked_col = find_secound_row2(blocks, dup_block_colour);
    
    moveBlock(1, new_empty_col); // pick up
    moveBlock(0, stacked_col); // place
    move_first_row2(blocks, dup_block_colour);
    print_blocks(blocks);
    Serial.println("-------------------------");

    // PART 3 (EMPTY LEAST FULL COLUMN)
    moveBlock(2, new_empty_col); // pick up
    moveBlock(0, placeholder_3); // place
    move_empty_column(blocks); 
    print_blocks(blocks);
    Serial.println("-------------------------");
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }

    // PART 4 (FILL EMPTY ROW WITH MATCHING TWO BLOCKS)
    moveBlock(0, stacked_col); // pick up
    moveBlock(2, new_empty_col); // place
    moveBlock(1, stacked_col); // pick up
    moveBlock(1, new_empty_col); // place
    find_stacked_matching_blocks_col(blocks, new_empty_col , stacked_col);
    print_blocks(blocks);
    Serial.println("-------------------------");

    // PART 5
    placeholder_5 = find_most_full_column(blocks);
    int row_value = find_last_dupe(blocks, dup_block_colour);

    if (row_value == 0) {
        moveBlock(0, placeholder_5); // pick up (GOOD)
        moveBlock(0, new_empty_col); // place (GOOD)
    } else if (row_value == 1) {
        moveBlock(0, placeholder_5); // pick up (BAD)
        moveBlock(1, stacked_col); // place (BAD)
        moveBlock(1, placeholder_5); // pick up (GOOD)
        moveBlock(0, new_empty_col); // place (BAD)
    } else if (row_value == 2) {
        moveBlock(0, placeholder_5); // pick up (BAD)
        moveBlock(1, stacked_col); // place (BAD)
        moveBlock(1, placeholder_5); // pick up (BAD)
        moveBlock(0, stacked_col); // place (BAD)
        moveBlock(2, placeholder_5); // pick up (GOOD)
        moveBlock(0, new_empty_col); // place (GOOD)
    }
    
    blocks_above_dupe(blocks, new_empty_col, stacked_col, row_value);
    print_blocks(blocks);
    Serial.println("-------------------------");
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }

    // PART 6
    find_last_block(blocks);
}

void loop() {
      delay(5000000);
      hometo_00();
      hometo_01();
      hometo_02();
      hometo_10();
      hometo_11();
      hometo_12();
      hometo_20();
      hometo_21();
      hometo_22();
      clawopen();
      clawclose();
}

void print_blocks(int matrix[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (matrix[i][j] == 1) {
                Serial.print("🔴 ");  // Red block
            } else if (matrix[i][j] == 2) {
                Serial.print("🔵 ");  // Blue block
            } else {
                Serial.print("⚪ ");  // Empty space
            }
        }
        Serial.println();
    }
}

bool same_colour_column(int matrix[3][3], int col) {
    int color = matrix[0][col];
    if (color == 0) {
        return false;
    }
    for (int row = 0; row < 3; ++row) {
        if (matrix[row][col] != color) {
            return false;
        }
    }
    return true;
}

int check_for_success(int matrix[3][3]) {
    bool column0 = same_colour_column(matrix, 0);
    bool column1 = same_colour_column(matrix, 1);
    bool column2 = same_colour_column(matrix, 2);
    int count = 0;

    if (column0) count++;
    if (column1) count++;
    if (column2) count++;

    if (count > 1) {
        Serial.println("SUCCESSFUL!!!!!!");
        return 1;
    }
    return 0;
}

int column_count(int matrix[3][3], int col) {
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        if (matrix[i][col] != 0) {
            count++;
        }
    }
    return count;
}

int find_most_empty_column(int matrix[3][3]) {
    int max_count = 4;
    int column_index = 0;
    for (int col = 0; col < 3; ++col) {
        int count = column_count(matrix, col);
        if (count < max_count) {
            max_count = count;
            column_index = col;
        }
    }
    return column_index;
}

int find_secound_most_empty_column(int matrix[3][3]) {
    int secound_least_column = 0;
    for (int col = 0; col < 3; ++col) {
        int count = column_count(matrix, col);
        if (count == 2) {
            secound_least_column = col;
        }
    }
    return secound_least_column;
}

void place_in_lowest_empty(int matrix[3][3], int block, int col) {
    for (int row = 2; row >= 0; --row) {
        if (matrix[row][col] == 0) {
            matrix[row][col] = block;
            break;
        }
    }
}

int find_in_lowest_empty(int matrix[3][3], int col) 
{
    int row1;
    for (int row = 2; row >= 0; --row) {
        if (matrix[row][col] == 0) {
            row1 = row;
            break;
        }
    }
    return row1;
}

int count_top_row_blocks(int matrix[3][3]) {
    int count = 0;
    for (int col = 0; col < 3; ++col) {
        if (matrix[0][col] != 0) {
            count++;
        }
    }
    return count;
}

int find_top_left(int matrix[3][3]) {
    for (int col = 0; col < 3; ++col) {
        if (matrix[0][col] != 0) {
            return col;
        }
    }
    return -1;
}

void move_top_left(int matrix[3][3]) {
    int column_with_top_left = find_top_left(matrix);
    int most_empty_column = find_most_empty_column(matrix);
    int block = matrix[0][column_with_top_left];
    matrix[0][column_with_top_left] = 0;
    place_in_lowest_empty(matrix, block, most_empty_column);
}

int check_row2(int matrix[3][3]) {
    int blue_count = 0;
    int red_count = 0;
    for (int col = 0; col < 3; col++) {
        if (matrix[1][col] == 1) {
            red_count++;
        } else if (matrix[1][col] == 2) {
            blue_count++;
        }
    }
    if (red_count > blue_count) {
        return 1;
    } else if (blue_count > red_count) {
        return 2;
    }
    return 0; // In case of a tie or no blocks
}

int find_first_row2(int matrix[3][3], int colour) {
    for (int col = 0; col < 3; ++col) {
        if (matrix[1][col] == colour) {
            return col;
        }
    }
    return -1;
}

int find_secound_row2(int matrix[3][3], int colour) {
    int count = 0;
    for (int col = 0; col < 3; ++col) {
        if (matrix[1][col] == colour) {
            count++;
            if (count == 2) {
                return col;
            }
        }
    }
    return -1; // Return -1 if no second duplicate is found
}

void move_first_row2(int matrix[3][3], int colour) {
    int first_duplicate_block_col = find_first_row2(matrix, colour);
    int secound_duplicate_block_col = find_secound_row2(matrix, colour);
    int block = matrix[1][first_duplicate_block_col];
    matrix[1][first_duplicate_block_col] = 0;
    place_in_lowest_empty(matrix, block, secound_duplicate_block_col);
}

void move_empty_column(int matrix[3][3]) {
    int first_most_empty = find_most_empty_column(matrix);
    int secound_most_empty = find_secound_most_empty_column(matrix);
    int block = matrix[2][first_most_empty];
    matrix[2][first_most_empty] = 0;
    place_in_lowest_empty(matrix, block, secound_most_empty);
}

void find_stacked_matching_blocks_col(int matrix[3][3], int col_first, int col_second) {
    int dup_block_1 = matrix[0][col_second];
    int dup_block_2 = matrix[1][col_second];
    matrix[0][col_second] = 0;
    place_in_lowest_empty(matrix, dup_block_1, col_first);
    print_blocks(matrix);
    Serial.println("-------------------------");
    matrix[1][col_second] = 0;
    place_in_lowest_empty(matrix, dup_block_2, col_first);
}

int find_most_full_column(int matrix[3][3]) {
    int max_count = 0;
    int column_index = 0;
    for (int col = 0; col < 3; ++col) {
        int count = column_count(matrix, col);
        if (count > max_count) {
            max_count = count;
            column_index = col;
        }
    }
    return column_index;
}

int find_last_dupe(int matrix[3][3], int colour) {
    int bigcolumn = find_most_full_column(matrix);
    for (int i = 0; i < 3; ++i) {
        if (matrix[i][bigcolumn] == colour) {
            return i; // Return the row index of the last duplicate
        }
    }
    return -1; // Return -1 if no duplicate is found
}

int blocks_above_dupe(int matrix[3][3], int col_first, int col_second, int row_value) {
    int currentcolumn = find_most_full_column(matrix);
    int goodblock, badblock1, badblock2;

    if (row_value == 0) {
        goodblock = matrix[0][currentcolumn];
        matrix[0][currentcolumn] = 0;
        place_in_lowest_empty(matrix, goodblock, col_first);
    } else if (row_value == 1) {
        badblock1 = matrix[0][currentcolumn];
        matrix[0][currentcolumn] = 0;
        place_in_lowest_empty(matrix, badblock1, col_second);
        print_blocks(matrix);
        Serial.println("-------------------------");
        goodblock = matrix[1][currentcolumn];
        matrix[1][currentcolumn] = 0;
        place_in_lowest_empty(matrix, goodblock, col_first);
    } else if (row_value == 2) {
        badblock1 = matrix[0][currentcolumn];
        matrix[0][currentcolumn] = 0;
        place_in_lowest_empty(matrix, badblock1, col_second);
        print_blocks(matrix);
        Serial.println("-------------------------");
        badblock2 = matrix[1][currentcolumn];
        matrix[1][currentcolumn] = 0;
        place_in_lowest_empty(matrix, badblock2, col_second);
        print_blocks(matrix);
        Serial.println("-------------------------");
        goodblock = matrix[2][currentcolumn];
        matrix[2][currentcolumn] = 0;
        place_in_lowest_empty(matrix, goodblock, col_first);
    }
}

int find_last_block(int matrix[3][3]) {
    int col_to_move_last_block_from = find_most_empty_column(matrix);
    int col_to_move_last_block_to = find_secound_most_empty_column(matrix);
    int last_block = matrix[2][col_to_move_last_block_from];
    matrix[2][col_to_move_last_block_from] = 0;
    place_in_lowest_empty(matrix, last_block, col_to_move_last_block_to);
    print_blocks(matrix);
    Serial.println("-------------------------");
}

void moveBlock(int row, int col) {
    Serial.print("Move to [");
    Serial.print(row);
    Serial.print("][");
    Serial.print(col);
    Serial.println("]");
}


void goHome() {
  // Set the direction towards home position for both motors
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, LOW);  // Adjust direction if needed
  myservo.write(100);

  // Move both motors 1250 steps to reach the home position
  for (int x = 0; x < 1250; x++) {
    //begins 1.8cm from corner, finishes 16.5 cm 16.5-1.8 =~ 15cm
     //Assume 1 step == 0.17cm
     
    // Step Motor 1
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    
   
    // Step Motor 2
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    
    delayMicroseconds(1000);  // Small delay between each full step cycle
  }

delay(1000);
  // Motors are now at the home position
}

void hometo_00(){
   // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it left ~4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it down ~4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

//Grab functions
  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }
  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //up first
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }
  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_01(){
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it down 5.68cm
  for(int x = 0; x < 275; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }
//Grab functions
if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }
  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  ///This for loop moves it up ~5.68cm
  for(int x = 0; x < 275; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }


  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  
delay(1000);
}

void hometo_02(){
  
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it down 53.748mm
  for(int x = 0; x < 250; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it up ~5.38cm
  for(int x = 0; x < 250; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}

void hometo_10(){
   // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it down 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  
  delay(1000);
  
}

void hometo_11(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  
  delay(1000);
}

void hometo_12(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  
  delay(1000);
}

void hometo_20(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it down 5.68cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home
  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_21(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_22(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
}

void clawopen(){
  myservo.write(100);
  delay(1000);
}


void clawclose(){
  myservo.write(35);
  delay(1000);
}