#include <iostream>

using namespace std;

int field[100][100];
// -1 - mine
// 0-8 - cell

int field_p[100][100];
// -2 - closed
// -1 - flag
// 0-8 - open

const int N = 11, M = 11, K = 12;

void field_fill() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            field[i][j] = 0;
            field_p[i][j] = -2;
        }
    }
}

void mine_generation() {
    for (int i = 0; i < K; i++) {
        while (true) {
            int x = rand() % N;
            int y = rand() % M;
            if (field[x][y] != -1) {
                //cout << x << ' ' << y << endl;
                field[x][y] = -1;
                break;
            }
        }
    }
}

void field_calc() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (field[i][j] != -1) {
                field[i][j] = 0;
                for (int i2 = i - 1; i2 <= i + 1; i2++) {
                    for (int j2 = j - 1; j2 <= j + 1; j2++) {
                        if (i2 >= 0 && i2 < N && j2 >= 0 && j2 < M && field[i2][j2] == -1) field[i][j]++;
                    }
                }
            }
        }
    }
}

void print_field() {
    system("cls");
    cout << "   ";
    for (int i = 0; i < M; i++) {
        cout << i + 1 << " ";
        if (i + 1 < 10) cout << " ";
    }
    cout << "\n";
    for (int i = 0; i < N; i++) {
        
        cout << i + 1  << " ";
        if (i + 1 < 10) cout << " ";
        for (int j = 0; j < M; j++) {
            if (field_p[i][j] == -2) cout << "_  ";
            else if (field_p[i][j] == -1) cout << "f  ";
            else if (field_p[i][j] == -3) cout << "*  ";
            else cout << field_p[i][j] << "  ";
        }
        cout << endl;
    }
}

void dfs(int x, int y) {
    if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] > -2) return;
    field_p[x][y] = field[x][y];
    if (field[x][y] > 0) return;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i != x || j != y) dfs(i, j);
        }
    }
}

// true - ne vzorvalis
bool open_cell(int x, int y) {
    if (field[x][y] == -1) return false;
    if (field[x][y] > 0) {
        field_p[x][y] = field[x][y];
        return true;
    }
    dfs(x, y);
}

bool is_win() {
    int opened = 0;
    for(int i = 0; i < N;i++) {
        for (int j = 0; j < M; j++) {
            if (field_p[i][j] >= 0) opened++;
        }
    }
    return (N * M - K == opened);
}

void end_game(bool is_win = false) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (field_p[i][j] == -1) field_p[i][j] = -2;
            if (field[i][j] == -1) field_p[i][j] = -3;
        }
    }
    print_field();
    cout << "Vi " << (is_win ? "viigrali" : "proigrali") << ",\nchtoby nachat novuyu igru vvedite lubuyu stroky!\n";
    string s;
    cin >> s;
}

int main()
{
    while (true) {
        srand(time(0));

        field_fill();
        mine_generation();
        field_calc();



        while (true) {
            print_field();
            cout << "Enter comand:";
            /*
            \o x y
            \f x y
            \n
            \q
            */
            string comand;
            cin >> comand;

            if (comand == "\\o") {
                // vvod koordinat
                int x, y;
                cin >> x >> y;
                x--;y--;
                // otkroem kletki
                if (x < 0 || x >= N || y< 0 || y>= M || field_p[x][y] >= 0) continue;
                if (!open_cell(x, y)) {
                    end_game();
                    break;
                }
                if (is_win()) {
                    end_game(true);
                    break;
                }
                
            }
            else if (comand == "\\f") {
                // vvod koordinat
                int x, y;
                cin >> x >> y;
                x--;y--;
                // postavim flag
                if (x < 0 || x >= N || y< 0 || y>= M || field_p[x][y] >= 0) continue;
                
                if (field_p[x][y] == -1) field_p[x][y] = -2;
                else field_p[x][y] = -1;
            }
            else if (comand == "\\n") {
                break;
            }
            else if (comand == "\\q") {
                return 0;
            }
        }
    }
}
