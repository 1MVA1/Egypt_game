#include <iostream>
#include <fstream>

#include <cstdlib>  // для rand() и srand()
#include <ctime>    // для time()
#include <limits>  // для numeric_limits

using namespace std;

struct Game
{
    int round = 1;
    int population = 100;
    double baskets_of_grain = 2800.0;
    //Фаддан — египетская мера измерения площади
    int faddans_of_lands = 1000;

    // Переменная, чтобы при загрузке сохранения не были вычислены снова данные
    bool was_calculated = false;
    int baskets_of_grain_to_feed_population = 0;
    int seeded_fadans = 0;
    int faddans_to_buy = 0;
    int baskets_of_grain_from_faddan = 0;
    bool is_plague = false;
    int dead_this_round = 0;
    int price_of_faddan = 0;
    int baskets_of_grain_eaten_by_mouse = 0;
    int new_people = 0;
    int new_baskets_of_grain = 0;

    // Сумма процентов мертвых слуг
    double sum_percentages_of_dead = 0.0f;
};

void clearConsole() 
{
#ifdef _WIN32
    system("cls");  // Для Windows
#else
    system("clear"); // Для Linux и Mac
#endif
}

bool yes_or_no() 
{
    bool ans;
    cout << "Введите 1 (да) или 0 (нет): ";

    while (!(cin >> ans)) 
    {
        cin.clear(); // Очищаем флаг ошибки
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода

        cout << "Неправильный ввод. Пожалуйста, введите 1 (да) или 0 (нет): ";
    }

    return ans;
}

static void saveToFile(const Game& game)
{
    //Автоматически обрезает файл до нуля, если он существует. Это удаляет все его содержимое
    ofstream file("save.txt", ios::trunc);

    if (file.is_open())
    {
        file << game.round << "\n"
            << game.population << "\n"
            << game.baskets_of_grain << "\n"
            << game.faddans_of_lands << "\n"
            << game.baskets_of_grain_to_feed_population << "\n"
            << game.seeded_fadans << "\n"
            << game.faddans_to_buy << "\n"
            << game.baskets_of_grain_from_faddan << "\n"
            << game.sum_percentages_of_dead << "\n"
            << game.was_calculated << "\n"
            << game.is_plague << "\n"
            << game.dead_this_round << "\n"
            << game.price_of_faddan << "\n"
            << game.baskets_of_grain_eaten_by_mouse << "\n"
            << game.new_people << "\n"
            << game.new_baskets_of_grain << "\n";

        file.close();
    }
    else
    {
        cerr << "Не удалось открыть файл для сохранения.\n";
    }
}

static Game readFromFile()
{
    Game game;

    // ios::ate перемещает указатель файла в конец при открытии
    ifstream file("save.txt", ios::ate);

    if (file.is_open())
    {
        // Проверить, что позиция в конце файла равна 0
        bool isEmpty = (file.tellg() == 0);

        if (isEmpty)
        {
            cout << "Сохранение ненайдено.\nЗапуск новой игры.\n\n";
        }
        else
        {
            cout << "Сохранение найдено.\nЖелаете его продолжить?\n";

            if (yes_or_no()) {
                // Возврат указателя на начало
                file.seekg(0, ios::beg);

                file >> game.round;
                file >> game.population;
                file >> game.baskets_of_grain;
                file >> game.faddans_of_lands;
                file >> game.baskets_of_grain_to_feed_population;
                file >> game.seeded_fadans;
                file >> game.faddans_to_buy;
                file >> game.baskets_of_grain_from_faddan;
                file >> game.sum_percentages_of_dead;
                file >> game.was_calculated;
                file >> game.is_plague;
                file >> game.dead_this_round;
                file >> game.price_of_faddan;
                file >> game.baskets_of_grain_eaten_by_mouse;
                file >> game.new_people;
                file >> game.new_baskets_of_grain;

                cout << "Сохранение успешно прочитано.\n";
            }
        }

        file.close();
    }
    else
    {
        cerr << "Не удалось открыть файл для чтения.\n";
    }

    return game;
}

static void deleteSave()
{
    ofstream file("save.txt", ios::trunc);

    if (!file.is_open())
    {
        cerr << "Не удалось открыть файл для удаления сохранения.\n";
    }
}

int main()
{
    double coeff_mouse;
    int helper;

    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    cout << "Добро пожаловать в игру Фараон: великий правитель Египта!\n\n";

    Game game = readFromFile();

    while (game.round < 11)
    {
        cout << "О великий фараон, соизвольте поведать вам!\n\n";

        if (game.round == 1)
        {
            cout << "В начале вашего мудрого правления\n\tВ городе проживает " << game.population << " верных слуг;\n\tВ хранилищах хранится "
                << game.baskets_of_grain << " корзин с зерном;\n\tА город занимает " << game.faddans_of_lands << " фаддонов земли;\n";

            if (!game.was_calculated) {
                game.price_of_faddan = rand() % 10 + 17;
                game.was_calculated = true;

                saveToFile(game);
            }
        }
        else
        {
            if (!game.was_calculated)
            {
                helper = game.baskets_of_grain_to_feed_population / 20;

                // Если выделено слишком много еды, то она возвращается
                if (helper > game.population) {
                    game.baskets_of_grain += game.baskets_of_grain_to_feed_population - (game.population * 20);
                }
                else
                {
                    game.dead_this_round = game.population - (game.baskets_of_grain_to_feed_population / 20);

                    if (game.population * 0.45 <= game.dead_this_round)
                    {
                        cout << "Город голодает. Много ваших слуг погибло. Бунтовщики требует вашей смерти.\nБегите сейчас или примите гнев вашего народа!\n";

                        deleteSave();

                        return 0;
                    }
                }

                game.sum_percentages_of_dead += static_cast<double>(game.dead_this_round) * 100 / game.population;
                game.population -= game.dead_this_round;

                game.baskets_of_grain_from_faddan = rand() % 6 + 1;
                game.new_baskets_of_grain = game.baskets_of_grain_from_faddan * game.seeded_fadans;
                game.baskets_of_grain += game.new_baskets_of_grain;

                coeff_mouse = static_cast<double>(rand()) / RAND_MAX * 0.07;
                game.baskets_of_grain_eaten_by_mouse = static_cast<int>(game.baskets_of_grain * coeff_mouse);
                game.baskets_of_grain -= game.baskets_of_grain_eaten_by_mouse;

                game.new_people = static_cast<int>(static_cast<double>(game.dead_this_round) / 2 + (5 - game.baskets_of_grain_from_faddan) * game.baskets_of_grain / 600 + 1);

                if (game.new_people > 50) {
                    game.new_people = 50;
                }
                else if (game.new_people < 0) {
                    game.new_people = 0;
                }

                game.population += game.new_people;

                if (static_cast<double>(rand()) / RAND_MAX < 0.15f)
                {
                    game.is_plague = true;
                    game.population /= 2;
                }
                else {
                    game.is_plague = false;
                }

                game.price_of_faddan = rand() % 10 + 17;

                game.was_calculated = true;

                saveToFile(game);
            }

            cout << "К началу " << game.round << " года вашего мудрого правления:\n";

            if (game.dead_this_round != 0) {
                cout << "\t" << game.dead_this_round << " верных слуг погибло от голода;\n";
            }

            if (game.new_people != 0) {
                cout << "\t" << game.new_people << " новых слуг прибыло в город;\n";
            }

            if (game.is_plague) {
                cout << "\tАнубис послал черную смерть в наш город;\n";
            }

            cout << "\tЧисло ваших слуг составляет " << game.population << ";\n";

            if (game.new_baskets_of_grain != 0) {
                cout << "\tОсириса позволила собрать " << game.new_baskets_of_grain << " корзин зерна, по " << game.baskets_of_grain_from_faddan << " с каждого фаддана земли;\n";
            }

            if (game.baskets_of_grain_eaten_by_mouse != 0) {
                cout << "\tМыши погубили " << game.baskets_of_grain_eaten_by_mouse << " корзин зерна;\n";
            }

            cout << "\tВ хранилищах осталось " << game.baskets_of_grain << " корзин зерна\n"
                << "\tВаши владения сейчас охватывают " << game.faddans_of_lands << " фадданов земли;\n"
                << "\tСтоимость одного фаддана в корзинах зерна состовляет " << game.price_of_faddan << ";\n";
        }

        cout << "\nКакие мудрые решения примете, фараон?\n";

        while (true) {
            cout << "\tНа каждого жителя требуется по 20 корзин зерна.\n\tСколько прикажете отдать на пропитание? ";
            cin >> game.baskets_of_grain_to_feed_population;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\tМой повелитель, я вас не понял. Прошу, повторите.\n";
            }
            else if (game.baskets_of_grain_to_feed_population > game.baskets_of_grain) {
                cout << "\n\tФараон, в наших запасах всего " << game.baskets_of_grain << " корзин зерна.\n";
            }
            else 
            {
                cout << "\tБудет исполнено.\n";
                game.baskets_of_grain -= game.baskets_of_grain_to_feed_population;

                break;
            }
        }

        cout << "\n\tТеперь в запасах " << game.baskets_of_grain << " корзин зерна.\n\n";

        while (true) {
            cout << "\tКаждый ваш слуга сможет засеять лишь 10 фадданов земли.\n\tНа каждый фадан потребуется по половине коризины зерна.\n\tСколько прикажете засеять? ";
            cin >> game.seeded_fadans;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\tМой повелитель, я вас не понял. Прошу, повторите.\n";
            }
            else if (game.seeded_fadans > game.baskets_of_grain * 2) {
                cout << "\n\tФараон, в наших запасах всего " << game.baskets_of_grain << " корзин зерна.\n";
            }
            else if (game.seeded_fadans > game.population * 10) {
                cout << "\n\tФараон, " << game.population << " слуг не хватит на ваши великие помыслы.\n";
            }
            else if (game.seeded_fadans > game.faddans_of_lands) {
                cout << "\n\tФараон, в ваших владениях только " << game.faddans_of_lands << " фадданов земли.\n";
            }
            else 
            {
                cout << "\tБудет исполнено.\n";
                game.baskets_of_grain -= game.seeded_fadans * 0.5;

                break;
            }
        }

        helper = game.faddans_of_lands - game.seeded_fadans;

        cout << "\n\tТеперь в запасах " << game.baskets_of_grain << " корзин зерна.\n\tЧисло незасеянных фадданов состоявлет " << helper << ".\n\n";

        while (true) {
            cout << "\tСтоимость одного фаддана в корзинах зерна состовляет " << game.price_of_faddan << ".\n\tЖелаете ли вы приобрести или сбыть? ";
            cin >> game.faddans_to_buy;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорируем некорректный ввод
                cout << "\n\tМой повелитель, я вас не понял. Прошу, повторите.\n";
            }
            else if (game.faddans_to_buy > 0 && game.baskets_of_grain < game.price_of_faddan * game.faddans_to_buy) {
                cout << "\n\tФараон, в наших запасах всего " << game.baskets_of_grain << " корзин зерна.\n";
            }
            else if (game.faddans_to_buy < 0 && helper < -game.faddans_to_buy) {
                cout << "\n\tФараон, в ваших владениях только " << game.faddans_of_lands << " незасеянных фадданов земли.\n";
            }
            else
            {
                game.baskets_of_grain += game.price_of_faddan * game.faddans_to_buy;
                game.faddans_of_lands += game.faddans_to_buy;
                cout << "\tБудет исполнено.\n";

                break;
            }
        }

        game.round++;
        game.was_calculated = false;

        saveToFile(game);

        clearConsole();
    }

    game.sum_percentages_of_dead /= 10;
    double help = static_cast<double>(game.baskets_of_grain) / game.population;

    if (game.sum_percentages_of_dead > 33 && help < 7)
    {
        cout << "Вы правили 10 долгих лет, фараон. Но слуги считают, что боги оказались неправы, выбрав вас. Народ требует вашей казни!\n";
    }
    else if (game.sum_percentages_of_dead > 10 && help < 9){
        cout << "\tВы правите жестоко, но не глупо, фараон. Народ боится вас, но готов служить дальше! Да направляет вас Ра!\n";
    }
    else if (game.sum_percentages_of_dead > 3 && help < 10) {
        cout << "\tВы правите как настоящий правитель, фараон. Слуги уважают вас. Они готовы умереть ради вас! Да направляет вас Ра!\n";
    }
    else {
        cout << "\tФараон, вы правите как никто до вас. Слуги боготворят вас. Мы хотим, чтобы Анубис наградил вас бессмертием, чтобы вы вечно правили!\n";
    }

    deleteSave();

    return 0;
}