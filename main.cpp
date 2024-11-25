#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits>
#include <random>


using namespace std;


const int MIN_LAND_PRICE = 17;
const int MAX_LAND_PRICE = 26;
const int BUSHELS_PER_PERSON = 20;
const double RATS_EAT_PERCENT = 0.07;
const double SEEDS_COST = 0.5;

struct GameState {
    int year;
    int population;
    int bushels;
    int acres;
    int starved;
    int arrived;
    int landPrice;
    int totalStarved;
};

bool loadGame(GameState &state) {
    ifstream file("save_game.txt");
    if (file.is_open()) {
        file >> state.year >> state.population >> state.bushels
             >> state.acres >> state.starved >> state.arrived
             >> state.landPrice >> state.totalStarved;
        file.close();
        return true;
    }
    return false;
}

void saveGame(const GameState &state) {
    ofstream file("save_game.txt");
    if (file.is_open()) {
        file << state.year << " " << state.population << " "
             << state.bushels << " " << state.acres << " "
             << state.starved << " " << state.arrived << " "
             << state.landPrice << " " << state.totalStarved << endl;
        file.close();
    }
}

int randomInRange(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

void printStateGame(GameState &state) {
    cout << "Население города составляет " << state.population << " человек;"<< endl;
    cout << "Имеется " << state.bushels << " бушеля в амбарах;" << endl;
    cout << "Город сейчас занимает " << state.acres << " акров;" << endl;
    cout << "1 акр земли стоит сейчас " << state.landPrice << " бушель." << endl;
}

void initializeGame(GameState &state) {
    state.year = 1;
    state.population = 100;
    state.bushels = 2800;
    state.acres = 1000;
    state.starved = 0;
    state.arrived = 5;
    state.landPrice = randomInRange(MIN_LAND_PRICE, MAX_LAND_PRICE);
    state.totalStarved = 0;

    cout << "Мой повелитель, соизволь поведать тебе\nв начале твоего высочайшего правления\n";
    printStateGame(state);
}

int getValidInput(const string& prompt, int minValue, int maxValue, const GameState& state) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail() || value < minValue || value > maxValue) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "О, повелитель, пощади нас! У нас только "
                 << state.population << " человек, "
                 << state.bushels << " бушелей пшеницы и "
                 << state.acres << " акров земли!\n";
        } else {
            return value;
        }
    }
}

void evaluatePerformance(const GameState &state) {
    double P = static_cast<double>(state.totalStarved) / (state.population + state.totalStarved) * 100 / 10.0;
    int L = state.acres / state.population;

    cout << "\nОценка вашего правления:\n";
    if (P > 33.0 && L < 7) {
        cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.\n";
        cout << "Теперь вы вынуждены влачить жалкое существование в изгнании.\n";
    } else if (P > 10.0 && L < 9) {
        cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному.\n";
        cout << "Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем.\n";
    } else if (P > 3.0 && L < 10) {
        cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова.\n";
    } else {
        cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше.\n";
    }
}

void playGame() {
    GameState state;
    srand(time(0));

    if (loadGame(state)) {
        char choice;
        cout << "Продолжить предыдущую игру? (y/n): ";
        cin >> choice;
        if (choice == 'n') {
            initializeGame(state);
        }
        else
        {
            cout << "Мой повелитель, соизволь поведать тебе\nв году " << state.year - 1 << " твоего высочайшего правления\n";
            printStateGame(state);
        }
    } else {
        initializeGame(state);
    }

    for (; state.year <= 10; state.year++) {
        int acresToBuy, acresToSell, bushelsToEat, acresToPlant;

        acresToBuy = getValidInput(
                "Сколько акров земли хотите купить? ",
                0,
                state.bushels / state.landPrice,
                state
        );
        state.acres += acresToBuy;
        state.bushels -= acresToBuy * state.landPrice;

        if (acresToBuy == 0) {
            acresToSell = getValidInput(
                    "Сколько акров земли хотите продать? ",
                    0,
                    state.acres,
                    state
            );
            state.acres -= acresToSell;
            state.bushels += acresToSell * state.landPrice;
        }

        bushelsToEat = getValidInput(
                "Сколько бушелей пшеницы использовать для еды? ",
                0,
                state.bushels,
                state
        );
        state.bushels -= bushelsToEat;

        acresToPlant = getValidInput(
                "Сколько акров земли засеять? ",
                0,
                min(min(state.acres, int(state.bushels / SEEDS_COST)), state.population * 10),
                state
        );
        state.bushels -= acresToPlant * SEEDS_COST;

        int bushelsByAcr = randomInRange(1, 6);
        int harvest = acresToPlant * bushelsByAcr;
        state.bushels += harvest;
        int ratsEat = randomInRange(0, RATS_EAT_PERCENT * state.bushels);
        state.bushels -= ratsEat;
        int peopleFed = bushelsToEat / BUSHELS_PER_PERSON;
        state.starved = max(0, state.population - peopleFed);
        state.totalStarved += state.starved;
        state.population -= state.starved;
        state.arrived = max(0, min(state.starved / 2 + (5 - bushelsByAcr) * state.bushels / 600 + 1, 50));
        state.population += state.arrived;


        cout << "Мой повелитель, соизволь поведать тебе\nв году " << state.year << " твоего высочайшего правления\n";
        if (state.starved || state.arrived) {
            cout << (state.starved ? to_string(state.starved) + " человек умерли с голоду" : "")
                 << (state.starved && state.arrived ? ", и " : "")
                 << (state.arrived ? to_string(state.arrived) + " человек прибыли в наш великий город" : "")
                 << ";" << endl;
        }

        if (randomInRange(1, 100) <= 15) {
            state.population /= 2;
            cout << "Чума уменьшила население наполовину!\n";
        }

        cout << "Население города сейчас составляет " << state.population << " человек;"<< endl;
        cout << "Мы собрали " << harvest << " бушелей пшеницы, по " << bushelsByAcr << " бушеля с акра;" << endl;
        cout << (ratsEat ? "Крысы истребили " + to_string(ratsEat) + " бушелей пшеницы, оставив " : "Осталось ")
             << state.bushels << " бушеля в амбарах;" << endl;
        cout << "Город сейчас занимает " << state.acres << " акров;" << endl;
        state.landPrice = randomInRange(MIN_LAND_PRICE, MAX_LAND_PRICE);
        cout << "1 акр земли стоит сейчас " << state.landPrice << " бушель." << endl;

        saveGame(state);

        char exitChoice;
        cout << "Хотите продолжить? (y/n): ";
        cin >> exitChoice;
        if (exitChoice == 'n') break;
    }

    evaluatePerformance(state);
    cout << "\nИгра окончена! Спасибо за игру, Правитель Египта!\n";
}

int main() {
    system("chcp 65001");
    cout << "Добро пожаловать в игру 'Правитель Египта'!\n";
    playGame();
    return 0;
}

