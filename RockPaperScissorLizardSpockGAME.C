#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_USERS 10
#define MAX_NAME_LENGTH 50
#define HISTORY_SIZE 5
#define FILE_NAME "users.dat"

struct User {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int high_score;
    int historical_data[HISTORY_SIZE];
};

struct User users[MAX_USERS];
int user_count = 0;

void registerUser();
int loginUser();
int getComputerChoice();
int getWinner(int userChoice, int computerChoice);
void displayChoices();
void playGame(int userIndex);
void computerVsComputer();
void displayUserStats(int userIndex);
void updateHistory(int historical_data[], int score);
void saveUsersToFile();
void loadUsersFromFile();
void hidePassword(char *password);

int main() {
    srand(time(NULL));
    int choice, userIndex = -1;

    loadUsersFromFile();

    while (1) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_BLUE);  
        system("cls");  
        printf("\n--- Main Menu ---\n");
        printf("0. Exit\n1. Register\n2. Login\n3. Play Game\n4. Computer vs Computer\n5. View Stats\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Exiting the program. Goodbye!\n");
            saveUsersToFile();
            break;
        } else if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            userIndex = loginUser();
            if (userIndex == -1) {
                printf("Invalid login. Please try again.\n");
                system("pause");
            } else {
                printf("Login successful! Welcome, %s!\n", users[userIndex].username);
                system("pause");
            }
        } else if (choice == 3) {
            if (userIndex != -1) {
                do {
                    playGame(userIndex);
                    printf("Do you want to play again? (1 for yes, 0 for no): ");
                    scanf("%d", &choice);
                } while (choice == 1);
            } else {
                printf("Please login first.\n");
                system("pause");
            }
        } else if (choice == 4) {
            computerVsComputer();
            system("pause");
        } else if (choice == 5) {
            if (userIndex != -1) {
                displayUserStats(userIndex);
                system("pause");
            } else {
                printf("Please login first.\n");
                system("pause");
            }
        } else {
            printf("Invalid choice. Please try again.\n");
            system("pause");
        }
    }

    return 0;
}

void hidePassword(char *password) {
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void registerUser() {
    // Step 1: Check if maximum user limit is reached
    if (user_count >= MAX_USERS) {
        printf("User registration limit reached.\n");
        return;
    }

    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    int i;

    // Step 2: Get username and password
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    hidePassword(password);

    // Step 3: Check if username already exists
    for (i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            system("pause");  // Pause so the user can read the message
            return;  // Exit if username exists
        }
    }

    // Step 4: Register the new user if username is available
    if (user_count < MAX_USERS) {
        strcpy(users[user_count].username, username);
        strcpy(users[user_count].password, password);
        users[user_count].high_score = 0;
        for (i = 0; i < HISTORY_SIZE; i++) {
            users[user_count].historical_data[i] = 0;
        }

        user_count++;  // Increment the user count

        // Print the successful registration message
        printf("Registration successful! Welcome, %s!\n", username);
    } else {
        printf("Error during registration. Please try again later.\n");
    }

    // Pause to ensure the message is visible
    printf("Press any key to continue...\n");
    getchar();  // Wait for the user to press a key
    getchar();  // Consume any extra newline character left by previous inputs
}
int loginUser() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    int i;
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    hidePassword(password);

    for (i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }
    printf("Incorrect username or password.\n");
    return -1;
}

int getComputerChoice() {
    return rand() % 5;
}

int getWinner(int userChoice, int computerChoice) {
    if (userChoice == computerChoice) return 0;
    if ((userChoice == 0 && (computerChoice == 2 || computerChoice == 3)) ||
        (userChoice == 1 && (computerChoice == 0 || computerChoice == 4)) ||
        (userChoice == 2 && (computerChoice == 1 || computerChoice == 3)) ||
        (userChoice == 3 && (computerChoice == 1 || computerChoice == 4)) ||
        (userChoice == 4 && (computerChoice == 0 || computerChoice == 2))) {
        return 1;
    }
    return -1;
}

void displayChoices() {
    printf("0. Rock\n1. Paper\n2. Scissors\n3. Lizard\n4. Spock\n");
}

void updateHistory(int historical_data[], int score) {
    int i;
    for (i = 0; i < HISTORY_SIZE - 1; i++) {
        historical_data[i] = historical_data[i + 1];
    }
    historical_data[HISTORY_SIZE - 1] = score;
}

void playGame(int userIndex) {
    int choice, computerChoice, winner, score = 0;
    printf("Choose your option:\n");
    displayChoices();
    scanf("%d", &choice);

    if (choice < 0 || choice > 4) {
        printf("Invalid choice. Please choose a number between 0 and 4.\n");
        return;
    }

    computerChoice = getComputerChoice();
    printf("Computer chose: %d\n", computerChoice);

    winner = getWinner(choice, computerChoice);
    if (winner == 1) {
        printf("You win!\n");
        score = 1;
    } else if (winner == -1) {
        printf("You lose.\n");
    } else {
        printf("It's a tie.\n");
    }

    updateHistory(users[userIndex].historical_data, score);
    users[userIndex].high_score += score;
}

void computerVsComputer() {
    int choice1 = getComputerChoice(), choice2 = getComputerChoice();
    printf("Computer 1 chose: %d\n", choice1);
    printf("Computer 2 chose: %d\n", choice2);

    int winner = getWinner(choice1, choice2);
    if (winner == 1) {
        printf("Computer 1 wins!\n");
    } else if (winner == -1) {
        printf("Computer 2 wins!\n");
    } else {
        printf("It's a tie!\n");
    }
}

void displayUserStats(int userIndex) {
    int i;
    printf("Username: %s\n", users[userIndex].username);
    printf("High Score: %d\n", users[userIndex].high_score);
    printf("Historical Data: ");
    for (i = 0; i < HISTORY_SIZE; i++) {
        printf("%d ", users[userIndex].historical_data[i]);
    }
    printf("\n");
}

void saveUsersToFile() {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file != NULL) {
        fwrite(&user_count, sizeof(int), 1, file);
        fwrite(users, sizeof(struct User), user_count, file);
        fclose(file);
    } else {
        printf("Error saving users to file.\n");
    }
}

void loadUsersFromFile() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file != NULL) {
        fread(&user_count, sizeof(int), 1, file);
        fread(users, sizeof(struct User), user_count, file);
        fclose(file);
    }
}
