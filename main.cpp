#include "bits/stdc++.h"
#define clr() system("clear")
#define draw !(board.find('.') != string::npos)
using namespace std;

string board = ".........";
int level;

void gui()
{
    for (int i=0; i<9; ++i) {
        cout << board[i];
        cout << ((i+1)%3==0?"\n":" ");
    } cout << "\n";
    
}

bool valid(int pos) 
{
    return board[pos]=='.';
}

int state()
{
    string w = "012345678036147258048246";

    for (int i=0; i<w.size(); i+=3) {
        set<char> s = { board[w[i]-'0'], board[w[i+1]-'0'], board[w[i+2]-'0'] };
        if (s.size() == 1 && *s.begin() != '.') { 
            return *s.begin() == 'X' ? -1 : 1;
        }
    }

    return 0;
}

bool check()
{
    if (draw||state()!=0) {
        clr();
        gui();
        cout << (state()==0?"Draw":state()==1?"AI Win":"You Win") << "\n";
        return true;
    }
    return false;
}

int minimax(bool is_ai)
{
    if (draw) return 0;
    if (state()!=0) return state();
    int best_score = is_ai ? -2 : 2;
    for (int i=0; i<9; ++i) {
        if (board[i]=='.') {
            board[i] = is_ai ? 'O' : 'X';
            int score = minimax(!is_ai);
            board[i] = '.';
            best_score = is_ai ? max(best_score, score) : min(best_score, score);
        }
    }
    return best_score;
}

void ai()
{
    switch (level) {
    case 0:
        {
            vector<int> vm;
            for (int i=0; i<9; ++i) {
                if (board[i]=='.') {
                    vm.push_back(i);
                }
            }
            board[vm[rand()%vm.size()]] = 'O';
            break;
        }
    
    case 1:
        {   
            int best_score = -2;
            int best_move = -1;
            for (int i=0; i<9; ++i) {
                if (board[i]=='.') {
                    board[i] = 'O';
                    int score = minimax(false);
                    board[i] = '.';
                    if (score > best_score) {
                        best_score = score;
                        best_move = i;
                    }
                }
            }
            board[best_move] = 'O';
            break;
        }

    default:
        break;
    }
}

int main()
{
    srand(time(0));
    cout << "Enter AI level[0:1]\n";
    cin >> level;
    while(1) {
        clr();
        gui();
        int p;
        cout << "Enter position[0:9[: ";
        cin >> p;
        if(!valid(p)) {
            clr();
            continue;
        }
        board[p] = 'X';
        if (check()) break;
        ai();
        if (check()) break;
    }
    return 0;
}
