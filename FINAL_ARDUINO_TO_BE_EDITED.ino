// Define the size of the matrix
#define ROWS 3
#define COLS 3

#include <Servo.h>
Servo myservo;

const int stepPin1 = 2;  // Step pin for Motor 1 (left = HIGH, right = LOW)
const int dirPin1 = 3;   // Direction pin for Motor 1
const int stepPin2 = 6;  // Step pin for Motor 2 (up = LOW, down = HIGH)
const int dirPin2 = 7;   // Direction pin for Motor 2

bool grab = true;

// Function declarations
void print_blocks(int matrix[ROWS][COLS]);
bool same_colour_column(int matrix[ROWS][COLS], int col);
int check_for_success(int matrix[ROWS][COLS]);
int column_count(int matrix[ROWS][COLS], int col);
int find_most_empty_column(int matrix[ROWS][COLS]);
int find_secound_most_empty_column(int matrix[ROWS][COLS]);
void place_in_lowest_empty(int matrix[ROWS][COLS], int block, int col);
int count_top_row_blocks(int matrix[ROWS][COLS]);
int find_top_left(int matrix[ROWS][COLS]);
int move_top_left(int matrix[ROWS][COLS]);
int check_row2(int matrix[ROWS][COLS]);
int find_first_row2(int matrix[ROWS][COLS], int colour);
int find_secound_row2(int matrix[ROWS][COLS], int colour);
int move_first_row2(int matrix[ROWS][COLS], int colour);
int move_empty_column(int matrix[ROWS][COLS]);
int find_stacked_matching_blocks_col(int matrix[ROWS][COLS], int col_first, int col_second);
int find_most_full_column(int matrix[ROWS][COLS]);
int find_last_dupe(int matrix[ROWS][COLS], int colour);
int blocks_above_dupe(int matrix[ROWS][COLS], int col_first, int col_second, int row_value);
int find_last_block(int matrix[ROWS][COLS]);

void setup() {
    pinMode(stepPin1, OUTPUT);
    pinMode(dirPin1, OUTPUT);
    pinMode(stepPin2, OUTPUT);
    pinMode(dirPin2, OUTPUT);
    myservo.attach(10);
    Serial.begin(9600);

    clawopen();
    clawclose();
    goHome();

    // PRINT ORIGINAL SETUP
    int blocks[ROWS][COLS] = {
        {2, 1, 0},
        {2, 2, 0},
        {1, 1, 0}
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

    while (statement) {
        int num_blocks_top_row = count_top_row_blocks(blocks);

        switch (num_blocks_top_row) {
            case 0:
                statement = false;  // No more blocks in the top row
                break;
            case 1:
            case 2:
                move_top_left(blocks);
                print_blocks(blocks);
                Serial.println("-------------------------");
                break;
        }
    }

    // PART 2 (CHECKING ROW 2)
    int dup_block_colour = check_row2(blocks);
    int new_empty_col = find_first_row2(blocks, dup_block_colour);
    int stacked_col = find_secound_row2(blocks, dup_block_colour);
    move_first_row2(blocks, dup_block_colour);

    print_blocks(blocks);
    Serial.println("-------------------------");

    // PART 3 (EMPTY LEAST FULL COLUMN)
    move_empty_column(blocks);
    print_blocks(blocks);
    Serial.println("-------------------------");
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }

    // PART 4 (FILL EMPTY ROW WITH MATCHING TWO BLOCKS)
    find_stacked_matching_blocks_col(blocks, new_empty_col, stacked_col);
    print_blocks(blocks);
    Serial.println("-------------------------");

    // PART 5
    int row_value = find_last_dupe(blocks, dup_block_colour);
    blocks_above_dupe(blocks, new_empty_col, stacked_col, row_value);
    print_blocks(blocks);
    Serial.println("-------------------------");
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }

    // PART 6
    find_last_block(blocks);
    success = check_for_success(blocks);
    if (success != 0) {
        return;
    }
  
    // Move to home position slowly at startup
    
    hometo_00();
    //hometo_01();
    hometo_02();
    //hometo_10();
    //hometo_11();
    //hometo_12();
    //hometo_20();
    //hometo_21();
    //hometo_22();
    //figureoutsteps();
    
}

void loop() {
    // Empty loop as we only need to run setup once
    hometo_00();
    hometo_02();
    delay(300000);
}

// GENERAL FUNCTIONS

void print_blocks(int matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (matrix[i][j] == 1) {
                Serial.print("🔴 ");  // Red block
            } else if (matrix[i][j ] == 2) {
                Serial.print("🔵 ");  // Blue block
            } else {
                Serial.print("⚪ ");  // Empty space
            }
        }
        Serial.println();
    }
}

bool same_colour_column(int matrix[ROWS][COLS], int col) {
    int color = matrix[0][col];
    if (color == 0) {
        return false;
    }
    for (int row = 0; row < ROWS; ++row) {
        if (matrix[row][col] != color) {
            return false;
        }
    }
    return true;
}

int check_for_success(int matrix[ROWS][COLS]) {
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

int column_count(int matrix[ROWS][COLS], int col) {
    int count = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][col] != 0) {
            count++;
        }
    }
    return count;
}

int find_most_empty_column(int matrix[ROWS][COLS]) {
    int max_count = 4; 
    int column_index = 0; 
    for (int col = 0; col < COLS; ++col) {
        int count = column_count(matrix, col);
        if (count < max_count) {
            max_count = count;
            column_index = col; 
        }
    }
    return column_index;
}

int find_secound_most_empty_column(int matrix[ROWS][COLS]) {
    int secound_least_column = 0;
    for (int col = 0; col < COLS; ++col) {
        int count = column_count(matrix, col);
        if (count == 2) {
            secound_least_column = col;
        }
    }
    return secound_least_column;
}

void place_in_lowest_empty(int matrix[ROWS][COLS], int block, int col) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (matrix[row][col] == 0) {
            matrix[row][col] = block;
            break;
        }
    }
}

int count_top_row_blocks(int matrix[ROWS][COLS]) {
    int count = 0;
    for (int col = 0; col < COLS; ++col) {
        if (matrix[0][col] != 0) {
            count++;
        }
    }
    return count;
}

int find_top_left(int matrix[ROWS][COLS]) {
    for (int col = 0; col < COLS; ++col) {
        if (matrix[0][col] != 0) {
            return col;
        }
    }
    return -1;
}

int move_top_left(int matrix[ROWS][COLS]) {
    int colunm_with_top_left = find_top_left(matrix);
    int most_empty_column = find_most_empty_column(matrix);
    int block = matrix[0][colunm_with_top_left];
    matrix[0][colunm_with_top_left] = 0;
    place_in_lowest_empty(matrix, block, most_empty_column);
}

int check_row2(int matrix[ROWS][COLS]) {
    int blue_count = 0;
    int red_count = 0;
    for (int col = 0; col < COLS; col++) {
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
}

int find_first_row2(int matrix[ROWS][COLS], int colour) {
    for (int col = 0; col < COLS; ++col) {
        if (matrix[1][col] == colour) {
            return col;
        }
    }
}

int find_secound_row2(int matrix[ROWS][COLS], int colour) {
    int count = 0;
    for (int col = 0; col < COLS; ++col) {
        if (matrix[1][ col] == colour) {
            count++;
            if (count == 2) {
                return col;
            }
        }
    }
}

int move_first_row2(int matrix[ROWS][COLS], int colour) {
    int first_duplicate_block_col = find_first_row2(matrix, colour);
    int secound_duplicate_block_col = find_secound_row2(matrix, colour);
    int block = matrix[1][first_duplicate_block_col];
    matrix[1][first_duplicate_block_col] = 0;
    place_in_lowest_empty(matrix, block, secound_duplicate_block_col);
}

int move_empty_column(int matrix[ROWS][COLS]) {
    int first_most_empty = find_most_empty_column(matrix);
    int secound_most_empty = find_secound_most_empty_column(matrix);
    int block = matrix[2][first_most_empty];
    matrix[2][first_most_empty] = 0;
    place_in_lowest_empty(matrix, block, secound_most_empty);
}

int find_stacked_matching_blocks_col(int matrix[ROWS][COLS], int col_first, int col_second) {
    int dup_block_1 = matrix[0][col_second];
    int dup_block_2 = matrix[1][col_second];
    matrix[0][col_second] = 0;
    place_in_lowest_empty(matrix, dup_block_1, col_first);
    print_blocks(matrix);
    Serial.println("-------------------------");
    matrix[1][col_second] = 0;
    place_in_lowest_empty(matrix, dup_block_2, col_first);
}

int find_most_full_column(int matrix[ROWS][COLS]) {
    int max_count = 0;
    int column_index = 0;
    for (int col = 0; col < COLS; ++col) {
        int count = column_count(matrix, col);
        if (count > max_count) {
            max_count = count;
            column_index = col; 
        }
    }
    return column_index;
}

int find_last_dupe(int matrix[ROWS][COLS], int colour) {
    int bigcolumn = find_most_full_column(matrix);
    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][bigcolumn] == colour) {
            return i;
        }
    }
}

int blocks_above_dupe(int matrix[ROWS][COLS], int col_first, int col_second, int row_value) {
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
        goodblock = matrix[1][currentcolumn];
        matrix[1][currentcolumn] = 0;
        place_in_lowest_empty(matrix, goodblock, col_first);
    } else if (row_value == 2) {
        badblock1 = matrix[0][currentcolumn];
        matrix[0][currentcolumn] = 0;
        place_in_lowest_empty(matrix, badblock1, col_second);
        badblock2 = matrix[1][currentcolumn];
        matrix[1][currentcolumn] = 0;
        place_in_lowest_empty(matrix, badblock2, col_second);
        goodblock = matrix[2][currentcolumn];
        matrix[2][currentcolumn] = 0;
        place_in_lowest_empty(matrix, goodblock, col_first);
    }
}

int find_last_block(int matrix[ROWS][COLS]) {
    int col_to_move_last_block_from = find_most_empty_column(matrix);
    int col_to_move_last_block_to = find_secound_most_empty_column(matrix);
    int last_block = matrix[2][col_to_move_last_block_from];
    matrix[2][col_to_move_last_block_from] = 0;
    place_in_lowest_empty(matrix, last_block, col_to_move_last_block_to);
    print_blocks(matrix);
    Serial.println("-------------------------");
}


void goHome() {
  // Set the direction towards home position for both motors
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, LOW);  // Adjust direction if needed
  myservo.write(100);

  // Move both motors 200 steps to reach the home position
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


//each movement function will be of type boolean, true == pick up, false == drop
void hometo_00(){
   // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }
  
    //

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
  //This for loop moves it left 4cm
  for(int x = 0; x < 200; x++){
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
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 285; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  ///This for loop moves it down 6cm
  for(int x = 0; x < 285; x++){
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

void hometo_02(){
  
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 40mm
  for(int x = 0; x < 222; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 53.748mm
  for(int x = 0; x < 270; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //
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

  //This for loop moves it down ~5.38cm
  for(int x = 0; x < 270; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 222; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}

void hometo_10(){
  
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

  //Here would be either the pickup or drop function
  //
  //
  //

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

  //Here would be either the pickup or drop function
  //
  //
  //

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

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

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

  //Here would be either the pickup or drop function
  //
  //
  //

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

  //Here would be either the pickup or drop function
  //
  //
  //

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


//math:
/*
whole board is 43 cm, Entire block holder portion is 21 cm, blocks
are entirely held within 15cm, divided evenly. 2.5cm dividers
middle part
43-21 = 22cm to the right to get to first column
To figure out steps:

void figureoutsteps(){
  //35 mm from left post
  //finished 18mm away
  //100 steps = 18mm -> 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, LOW); 
  for(int x = 0; x < 200; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
}

//total height of 3 blocks is 93.8mm
31.26 distance between each one
base is 5mm from table -> total height is 98.8mm
150mm from center of robot to top block
150-98.8 = 51.2mm distance from center
center of robot is 
*/