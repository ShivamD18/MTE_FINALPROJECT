#include <iostream>
using namespace std;

int DisplayBoxes(int arr[3][3]) {
    int i, j;
    cout << "Setup of Boxes:\n";
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            cout << "\t" << arr[i][j];
        }
        cout << endl;
    }
    return 0;
}

int main() {
    int arr[3][3] = {
        {0, 0, 0},
        {1, 2, 1},
        {2, 1, 2}
    };

    // Display initial setup
    DisplayBoxes(arr);
    
    if (arr[1][0] == arr[1][1]){
        arr[0][1] = arr[1][1];
        arr[1][0] = 0;
    }
    else if (arr[1][0] == arr[1][2]){
        arr[0][2] = arr[1][2];
        arr[1][0] = 0;
    }
    else if (arr[1][1] == arr[1][2]){
        arr[0][2] = arr[1][2];
        arr[1][1] = 0;
    }
    
    DisplayBoxes(arr);
    
    for (int i; i<3;i++){
        if (arr[0][i] == 0 && arr[1][i] == 0){
            for (int j; j<3;j++){
                if (arr[0][j] == 0 && arr[1][j] != 0){
                    if (arr[1][j] == arr[2][i]){
                        arr[1][i] = arr[1][j];
                        arr[1][j] = 0;
                    }
                    else{
                        arr[0][j] = arr[2][i];
                        arr[2][i] = 0;
                    }
                    for (int f; f<3;f++){
                        if (arr[0][f] == arr[1][f] && arr[0][f] != 0){
                            arr[2][i] = arr[0][f];
                            arr[1][i] = arr[0][f];
                        }
                    }
                }
            }
        }
    }

    // Final setup with all 1s, 2s, and 0s sorted in different columns
    DisplayBoxes(arr);

    return 0;
}
