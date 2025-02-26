#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Question{
    string question, option1, option2, option3, option4, correctAnswer;
};

struct User{
    string username;
    int score;
};

vector<Question> loadQuestions(string category){
    vector<Question> questions;
    ifstream file(category + ".txt");
    Question q;
    while (getline(file, q.question)){
        getline(file, q.option1);
        getline(file, q.option2);
        getline(file, q.option3);
        getline(file, q.option4);
        getline(file, q.correctAnswer);
        questions.push_back(q);
    }
    file.close();
    random_shuffle(questions.begin(), questions.end());
    return questions;
}

void saveLeaderboard(const User &user){
    ofstream file("leaderboard.txt", ios::app);
    file << user.username << " " << user.score << endl;
    file.close();
}

void displayLeaderboard(){
    ifstream file("leaderboard.txt");
    vector<User> users;
    User user;
    while (file >> user.username >> user.score){
        users.push_back(user);
    }
    file.close();
    sort(users.begin(), users.end(), [](User a, User b)
         { return a.score > b.score; });
    cout << "\n--------Leaderboard---------\n";
    int rank =0;
    for (auto &u : users){
        cout <<++rank<<" "<<u.username << " : " << u.score << " points \n";
    }
}

void playQuiz(string username, string category){
    vector<Question> questions = loadQuestions(category);
    int score = 0, streak = 0;
    bool lifelineUsed = false;

    for (size_t i = 0; i < min(questions.size(), size_t(5)); i++){
        cout << "\nQ" << (i+1) << ". " << questions[i].question << endl;
        cout << "1) " << questions[i].option1 << "\n2) " << questions[i].option2 << "\n3) " << questions[i].option3 << "\n4) " << questions[i].option4 << endl;

        char choice;
        cout << "Your Answer (1-4), or 'L' for 50-50 Lifeline: ";
        cin >> choice;

        if (choice == 'L' || choice == 'l') {
           if (!lifelineUsed) {
            lifelineUsed = true;

        int correctOption = questions[i].correctAnswer[0] - '0'; 
        vector<int> options = {1, 2, 3, 4};
        options.erase(remove(options.begin(), options.end(), correctOption), options.end());

        random_shuffle(options.begin(), options.end());
        
        int wrong1 = options[0];
        int wrong2 = options[1];

        cout << "\n50-50 Lifeline Used! Two wrong options removed.\n";
        cout << (wrong1 == 1 || correctOption == 1 ? "1) " + questions[i].option1 + "\n" : "");
        cout << (wrong1 == 2 || correctOption == 2 ? "2) " + questions[i].option2 + "\n" : "");
        cout << (wrong1 == 3 || correctOption == 3 ? "3) " + questions[i].option3 + "\n" : "");
        cout << (wrong1 == 4 || correctOption == 4 ? "4) " + questions[i].option4 + "\n" : "");

        cout << "Choose your answer (1-4): ";
        cin >> choice;
    }
    else {
        cout << "You've already used your lifeline!\nChoose your answer (1-4): ";
        cin >> choice;
    }
}

        if (choice == questions[i].correctAnswer[0]){
            score += 10;
            streak++;
            if (streak % 3 == 0)
            {
                score += 5;
                cout << "Streak Bonus! +5 points!\n";
            }
            cout << "Correct! +10 points\n";
        }
        else{
            streak = 0;
            cout << "Wrong answer!\n";
        }
    }

    cout << "\nQuiz Over! Your Score: " << score << endl;
    saveLeaderboard({username, score});
}

int main(){
    srand(time(0));
    string username, category;
    int choice;

    cout << "Enter your username: ";
    cin >> username;

    while (true){
        cout << "\n1. Play Quiz\n2. View Leaderboard\n3. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1){
            cout << "Choose Category (science / General / tech): ";
            cin >> category;
            for(auto& i : category)
                i= tolower(i);
            
            cout<<category;
            playQuiz(username, category);
        }
        else if (choice == 2){
            displayLeaderboard();
        }
        else{
            break;
        }
    }
    return 0;
}
