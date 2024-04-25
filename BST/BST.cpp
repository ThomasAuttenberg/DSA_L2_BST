// BST.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "BinaryTree.h"
#include <map>
#include <list>
#include "Menu.h"
#include <conio.h>
#include <random>
#include <time.h>

using namespace std;
typedef unsigned long long INT_64;

//переменная и константы генератора LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//функция установки первого случайного числа от часов
//компьютера
void sRand() {
    srand(time(0));
    RRand = (INT_64)rand();
}
//функция генерации случайного числа
//линейный конгруэнтный генератор Xi+1=(a*Xi+c)%m
//habr.com/ru/post/208178
INT_64 LineRand()
{
    INT_64 y1, y2;
    y1 = (aRand * RRand + cRand) % mRand;
    y2 = (aRand * y1 + cRand) % mRand;
    RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 &
        0xFFFFFFFF00000000LL) >> 32;
    return RRand;
}


void test_rand(int n)
{
    //создание дерева для 64 – разрядных ключей типа INT_64
    BinaryTree< INT_64, int > tree;
    //массив для ключей, которые присутствуют в дереве
    INT_64* m = new INT_64[n];
    //установка первого случайного числа
    sRand();
    //заполнение дерева и массива элементами
    //со случайными ключами
    for (int i = 0; i < n; i++)
    {
        m[i] = LineRand();
        tree.insert(m[i], 1);
    }
    //вывод размера дерева до теста
    std::cout << "items count:" << tree.size() << std::endl;
    //обнуление счётчиков трудоёмкости вставки,
    //удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;
    //генерация потока операций, 10% - промахи операций
    for (int i = 0; i < n / 2; i++)
        if (i % 10 == 0) //10% промахов
        {
            tree.erase(LineRand());
            D += tree.getLastOpPassedNodesNum();
            tree.insert(m[rand() % n], 1);
            I += tree.getLastOpPassedNodesNum();
            try {
                tree.at(LineRand());
                S += tree.getLastOpPassedNodesNum();
            }
            //обработка исключения при ошибке операции поиска
            catch (...) { S += tree.getLastOpPassedNodesNum(); }
        }
        else //90% успешных операций
        {
            int ind = rand() % n;
            tree.erase(m[ind]);
            D += tree.getLastOpPassedNodesNum();
            INT_64 key = LineRand();
            tree.insert(key, 1);
            I += tree.getLastOpPassedNodesNum();
            m[ind] = key;
            try {
                tree.at(m[rand() % n]);
                S += tree.getLastOpPassedNodesNum();
            }
            //обработка исключения при ошибке операции поиска
            catch (...) { S += tree.getLastOpPassedNodesNum(); }
        } //конец теста
       //вывод результатов:
       //вывод размера дерева после теста
    cout << "items count:" << tree.size() << endl;
    //теоретической оценки трудоёмкости операций BST
    cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
    //экспериментальной оценки трудоёмкости вставки
    cout << "Count insert: " << I / (n / 2) << endl;
    //экспериментальной оценки трудоёмкости удаления
    cout << "Count delete: " << D / (n / 2) << endl;
    //экспериментальной оценки трудоёмкости поиска
    cout << "Count search: " << S / (n / 2) << endl;
    //освобождение памяти массива m[]
    delete[] m;
}
void test_ord(int n)
{
    //создание дерева для 64 – разрядных ключей типа INT_64
    BinaryTree< INT_64, int > tree;
    //массив для ключей, которые присутствуют в дереве
    INT_64* m = new INT_64[n];
    //заполнение дерева и массива элементами
    // с возрастающими чётными ключами
    //на интервале [0, 10000, 20000, ... ,10000*n]
    for (int i = 0; i < n; i++) {
        m[i] = i * 10000;
        tree.insert(m[i], 1);
    }
    //вывод размера дерева до теста
    cout << "items count:" << tree.size() << endl;
    //обнуление счётчиков трудоёмкости вставки,
    // удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;
    //установка первого случайного числа
    sRand();
    //генерация потока операций, 10% - промахи операций
    for (int i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0) //10% промахов
        {
            int k = LineRand() % (10000 * n);
            k = k + !(k % 2); //случайный нечётный ключ
            tree.erase(k);
            D += tree.getLastOpPassedNodesNum();
            tree.insert(m[rand() % n], 1);
            I += tree.getLastOpPassedNodesNum();
            k = LineRand() % (10000 * n);
            k = k + !(k % 2); // случайный нечётный ключ
            try {
                tree.at(k);
                S += tree.getLastOpPassedNodesNum();
            }
            //обработка исключения при ошибке операции поиска
            catch (...) { S += tree.getLastOpPassedNodesNum(); }
        }
        else //90% успешных операций
        {
            int ind = rand() % n;
            tree.erase(m[ind]);
            D += tree.getLastOpPassedNodesNum();;
            int k = LineRand() % (10000 * n);
            k = k + k % 2; // случайный чётный ключ
            tree.insert(k, 1);
            I += tree.getLastOpPassedNodesNum();;
            m[ind] = k;
            try {
                tree.at(m[rand() % n]);
                S += tree.getLastOpPassedNodesNum();
            }
            //обработка исключения при ошибке операции поиска
            catch (...) { S += tree.getLastOpPassedNodesNum(); }
        }
    }
    //вывод результатов:
    // вывод размера дерева после теста
    cout << "items count:" << tree.size() << endl;
    //теоретической оценки трудоёмкости операций BST
    cout << "n/2 =" << n / 2 << endl;
    //экспериментальной оценки трудоёмкости вставки
    cout << "Count insert: " << I / (n / 2) << endl;
    //экспериментальной оценки трудоёмкости удаления
    cout << "Count delete: " << D / (n / 2) << endl;
    //экспериментальной оценки трудоёмкости поиска
    cout << "Count search: " << S / (n / 2) << endl;
    //освобождение памяти массива m[]
    delete[] m;
} //конец теста



int main()
{

   

    /*
        std::map<std::string, unsigned> products;
        products.insert(std::make_pair("2",5));
        products.insert(std::make_pair("2", 6));
        std::cout<<products.at("2")<<"\n\n\n";
        // установка значений
        products["bread"] = 30;
        products["milk"] = 80;
        //products["apple"];
        const auto newMap = products;

        //std::cout << newMap["apple"];

        struct meow {
            int a;
        };
        BinaryTree<int, int> s;
        s.keys();
        s.insert(6, 5);
        s.insert(2, 0);
        s.insert(3, 11);
        s.insert(11, 15);
        s.insert(8,0);
        //s.insert(13, 0);
        s.insert(9,0);
        //s.insert(13, 0);
        //s.insert(12,0);
        s.erase(11);
        //auto it = s.begin();
        //s[2] = 555;
        std::cout << (s.contains(6) ? "yes\n\n" : "no\n\n");
        auto list = s.keys();

        for (auto it = list.begin(); it != list.end(); it++) {
            std::cout << *it <<" ";
        }

        s.forEach([&](int key, int& val) {
            std::cout << "[" << key << "][" << val << "] ";
            });
        //s.print();
        std::cout << "\n\n";
        int depth_ = 0;
        s.forEach([&](int key, int& val) {
            std::cout << "[" << key << "][" << val << "] ";
        });
        std::cout << "\n";
        const BinaryTree<int, int> as = s;
        as.forEachHorizontal([&](const int& key, const int& val, size_t depth, size_t ordinalNumber) {
            std::cout << depth<<":"<< key << " ";
         });
        */


    system("chcp 1251");
    using value = int;
    using key = int;
    value inputValueBuffer;
    key inputKeyBuffer;
    BinaryTree<key, value> bstree;
    auto iter = bstree.begin();
    auto riter = bstree.rbegin();
    
    enum ITERATOR_TYPE {
        Forward,
        Reverse
    };
    ITERATOR_TYPE iteratorMode = ITERATOR_TYPE::Forward;
    bool autoIteratorCorrection = true;
    //list.push_back(3);
    //OPERATION_TYPE operationType;
    Menu navigationMenu([&]() {

        std::cout <<
            " ==================================================================\n\n"
            << ""
            << " Используйте стрелки, чтобы переключаться между пунктами меню\n"
            << " Используйте TAB чтобы сменить тип итератора, ~, чтобы переключить автокоррекцию невалидных итераторов\n"
            << " Используемый итератор:\033[1;36;40m " << (iteratorMode == ITERATOR_TYPE::Forward ? "ПРЯМОЙ" : "ОБРАТНЫЙ") << "\033[0m \n"
            << " Автокоррекция невалидных итераторов: " << (autoIteratorCorrection ? "\033[1;32;40mВКЛЮЧЕНА" : "\033[1;31;40mВЫКЛЮЧЕНА") << "\033[0m"
            << "\n ==================================================================\n\n";

        if (iteratorMode == ITERATOR_TYPE::Reverse) {

            for (auto it = bstree.rbegin(); it != bstree.rend(); it++) {
                //if (fixedIterActive && &(*iter_fixed) == &(*it)) std::cout << "\033[1;37;46m";
                if (riter != bstree.rend() && &((*riter).first) == &((*it).first)) std::cout << "\033[1;37;42m";
                //std::cout << "[this:]" << it.targetIndex << "[prev:]" << list.getNode(it).previousNodeIndex << "[next:]" << list.getNode(it).nextNodeIndex << " value: ";
                std::cout << "[" << (*it).first << " : " << (*it).second << "]\033[0m ";
            }
            if (riter == bstree.rend()) std::cout << "\033[1;37;42m[rend]\033[0m";

        }
        else {

            for (auto it = bstree.begin(); it != bstree.end(); it++) {
                //if (fixedIterActive && &(*iter_fixed) == &(*it)) std::cout << "\033[1;37;46m";
                if (iter != bstree.end() && &((*iter).first) == &((*it).first)) std::cout << "\033[1;37;42m";
                //std::cout << "[this:]" << it.targetIndex << "[prev:]" << list.getNode(it).previousNodeIndex << "[next:]" << list.getNode(it).nextNodeIndex << " value: ";
                std::cout << "[" << (*it).first << " : " << (*it).second << "]\033[0m ";
            }
            if (iter == bstree.end()) std::cout << "\033[1;37;42m[end]\033[0m";
        }

        });

    navigationMenu.addKeyListener(75, [&]() {
        try {
            if (ITERATOR_TYPE::Forward == iteratorMode)
                iter--;
            else
                riter--;
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
        }, true);

    navigationMenu.addKeyListener(77, [&]() {
        try {
            if (ITERATOR_TYPE::Forward == iteratorMode)
                iter++;
            else
                riter++;
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
        }, true);

    navigationMenu.addKeyListener(9, [&]() {


        if (iteratorMode == ITERATOR_TYPE::Forward) {
            iteratorMode = ITERATOR_TYPE::Reverse;
            riter = bstree.rbegin();

        }
        else {
            iteratorMode = ITERATOR_TYPE::Forward;
            iter = bstree.begin();
        }
        }, false);

    navigationMenu.addKeyListener(96, [&]() { 
        autoIteratorCorrection = !autoIteratorCorrection;   
        }, false);
    navigationMenu.addKeyListener(-72, [&]() {
        autoIteratorCorrection = !autoIteratorCorrection;
        }, false);

    MenuItem inputMenu(" insert(key,val) [Вставка значения]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        std::cout << "\n Введите значение: ";
        std::cin >> inputValueBuffer;
        bstree.insert(inputKeyBuffer, inputValueBuffer);
        //if (bstree.size() == 1) {
        if (autoIteratorCorrection) riter = bstree.rbegin();
        if (autoIteratorCorrection) iter = bstree.begin();
        //}
        Menu::console.setMenu(&navigationMenu);
        });
    MenuItem at(" at(key) [Получение значения по ключу]", [&]{
        std::cout << "\nВведите ключ: ";
        std::cin >> inputKeyBuffer;
        try {
            std::cout<<"\nЗначение по заданному ключу: "<<bstree.at(inputKeyBuffer);
            _getch();
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem atnwrite(" at(key) (+write) [Запись значения по существующему ключу]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        std::cout << " Введите новое значение: ";
        std::cin >> inputValueBuffer;
        try {
            bstree.at(inputKeyBuffer) = inputValueBuffer;
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem size(" size() [Опрос размера дерева]", [&] {
        std::cout << " Размер дерева: " << bstree.size();
        _getch();
    });
    MenuItem isEmpty(" empty() [Опрос дерева на пустоту]", [&] {
        std::cout <<"\n"<< (bstree.empty() ? "Дерево пусто" : "Дерево не пусто");
        _getch();
    });
    MenuItem clear(" clear() [Очистка дерева]", [&] {
        bstree.clear();
        if (autoIteratorCorrection) iter = bstree.begin();
        if (autoIteratorCorrection) riter = bstree.rbegin();
     });
    MenuItem erase(" erase(key) [Удаление элемента по ключу]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        try {
            if (!bstree.erase(inputKeyBuffer)) {
                std::cout << " erase вернул false: значение с заданным ключом не представлено";
                _getch();
            }
            else {
               if(autoIteratorCorrection) iter = bstree.begin();
               if (autoIteratorCorrection) riter = bstree.rbegin();
            }
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem keys(" keys() [Получение списка ключей t -> L -> R ]", [&] {
        auto list = bstree.keys();
        std::cout << "\n\nПолный список ключей: ";
        for (auto i : list) {
            std::cout << "[" << i << "] ";
        }
        _getch();
    });
    MenuItem find(" find(key) [Получение итератора по ключу]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        try {
            if (iteratorMode == ITERATOR_TYPE::Forward)
                iter = bstree.find(inputKeyBuffer);
            else
                riter.copy(bstree.find(inputKeyBuffer));
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
        });
    MenuItem tests(" Тестирование коллекции ", [&] {
        int input;
        std::cout << " Введите размер коллекции: ";
        std::cin >> input;
        std::cout << "\n Случайное дерево:\n===========================\n";
        test_rand(input);
        std::cout << "===========================\n Вырожденное дерево:\n===========================\n";
        test_ord(input);
        std::cout << "===========================";
        _getch();
    });
    MenuItem print(" Вывести дерево ", [&] {
        bstree.print();
        _getch();
    });
    
    /*
    
    Меню итераторов
    
    */
    auto savedIter = bstree.begin();
    auto savedRiter = bstree.rbegin();
    bool usingSavedIter = false;
    Menu iteratorOperations([&] {
        std::cout <<
            " ==================================================================\n\n"
            << ""
            << " Используйте стрелки, чтобы переключаться между пунктами меню\n"
            << " Используйте TAB, чтобы переключаться между итераторами\n"
            << " Используйте ~, чтобы переключаться между типами итераторов\n"
            << " Тип итераторов:\033[1;36;40m " << (iteratorMode == ITERATOR_TYPE::Forward ? "ПРЯМОЙ" : "ОБРАТНЫЙ") << "\033[0m \n"
            << " Используемый итератор:" << (usingSavedIter ? "\033[1;37;46mДОПОЛНИТЕЛЬНЫЙ\033[0m \n\n" : "\033[1;37;42mОСНОВНОЙ\033[0m\n\n")
            << " Основной: \033[1;37;42m";
        if (iteratorMode == ITERATOR_TYPE::Forward) {
            if (iter == bstree.end()) {
                std::cout << "[end]\033[0m\n";
            }
            else {
                std::cout << "[" << (*iter).first << " : " << (*iter).second << "]\033[0m\n";
            }
        }
        else {
            if (riter == bstree.rend()) {
                std::cout << "[rend]\033[0m\n";
            }
            else {
                std::cout << "[" << (*riter).first << " : " << (*riter).second << "]\033[0m\n";
            }
        }
        std::cout << " Дополнительный: \033[1;37;46m";
        if (iteratorMode == ITERATOR_TYPE::Forward) {
            if (savedIter == bstree.end()) {
                std::cout << "[end]\033[0m\n";
            }
            else {
                std::cout << "[" << (*savedIter).first << " : " << (*savedIter).second << "]\033[0m\n";
            }
        }
        else {
            if (savedRiter == bstree.rend()) {
                std::cout << "[rend]\033[0m\n";
            }
            else {
                std::cout << "[" << (*savedRiter).first << " : " << (*savedRiter).second << " ]\033[0m\n";
            }
        }
        std::cout<< "\n ==================================================================\n\n";

            if (iteratorMode == ITERATOR_TYPE::Reverse) {

                for (auto it = bstree.rbegin(); it != bstree.rend(); it++) {
                    bool isSavedRiterPointingAt = savedRiter != bstree.rend() && &(*savedRiter).first == &(*it).first;
                    bool isIterPointingAt = (riter != bstree.rend() && &((*riter).first) == &((*it).first));
                    if (isSavedRiterPointingAt && isIterPointingAt) {
                        std::cout << "\033[1;37;46m \033[1;37;42m ";
                    }
                    else {
                        if (isSavedRiterPointingAt) std::cout << "\033[1;37;46m";
                        if (isIterPointingAt) std::cout << "\033[1;37;42m";
                    }
                    //std::cout << "[this:]" << it.targetIndex << "[prev:]" << list.getNode(it).previousNodeIndex << "[next:]" << list.getNode(it).nextNodeIndex << " value: ";
                    std::cout << "[" << (*it).first << " : " << (*it).second << "]\033[0m ";
                }
                if (riter == bstree.rend() && savedRiter == bstree.rend()) {
                    std::cout << "\033[1;37;46m \033[1;37;42m[rend]\033[0m";
                }
                else {
                    if (riter == bstree.rend()) std::cout << "\033[1;37;42m[rend]\033[0m";
                    if (savedRiter == bstree.rend()) std::cout << "\033[1;37;46m[rend]\033[0m";
                }
            }
            else {

                for (auto it = bstree.begin(); it != bstree.end(); it++) {
                    bool isSavedIterPointingAt = savedIter != bstree.end() && &(*savedIter).first == &(*it).first;
                    bool isIterPointingAt = (iter != bstree.end() && &((*iter).first) == &((*it).first));
                    if (isSavedIterPointingAt && isIterPointingAt) {
                        std::cout << "\033[1;37;46m \033[1;37;42m";
                    }
                    else {
                        if (isSavedIterPointingAt) std::cout << "\033[1;37;46m";
                        if (isIterPointingAt) std::cout << "\033[1;37;42m";
                    }
                    //std::cout << "[this:]" << it.targetIndex << "[prev:]" << list.getNode(it).previousNodeIndex << "[next:]" << list.getNode(it).nextNodeIndex << " value: ";
                    std::cout << "[" << (*it).first << " : " << (*it).second << "]\033[0m ";
                }
                if (iter == bstree.end() && savedIter == bstree.end()) {
                    std::cout << "\033[1;37;46m \033[1;37;42m[rend]\033[0m";
                }
                else {
                    if (iter == bstree.end()) std::cout << "\033[1;37;42m[end]\033[0m";
                    if (savedIter == bstree.rend()) std::cout << "\033[1;37;46m[end]\033[0m";
                }
            }
    });

    iteratorOperations.addKeyListener(9, [&]() {
        usingSavedIter = !usingSavedIter;
    }, false);

    iteratorOperations.addKeyListener(96, [&]() {
        if (iteratorMode == ITERATOR_TYPE::Forward) {
            iteratorMode = ITERATOR_TYPE::Reverse;
            riter = bstree.rbegin();

        }
        else {
            iteratorMode = ITERATOR_TYPE::Forward;
            iter = bstree.begin();
        }
        }, false);
    iteratorOperations.addKeyListener(-72, [&]() {
        if (iteratorMode == ITERATOR_TYPE::Forward) {
            iteratorMode = ITERATOR_TYPE::Reverse;
            riter = bstree.rbegin();

        }
        else {
            iteratorMode = ITERATOR_TYPE::Forward;
            iter = bstree.begin();
        }
        }, false);

    MenuItem iteratorMenu(" Меню итераторов", iteratorOperations);

    MenuItem plus(" ++", [&] {
        try {
            if (iteratorMode == ITERATOR_TYPE::Reverse) {
                if (usingSavedIter)
                    ++savedRiter;
                else
                    ++riter;
            }
            else {
                if (usingSavedIter)
                    ++savedIter;
                else
                    ++iter;
            }
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
       
    });
    MenuItem minus(" --", [&] {
        try {
            if (iteratorMode == ITERATOR_TYPE::Reverse) {
                if (usingSavedIter)
                    --savedRiter;
                else
                    --riter;
            }
            else {
                if (usingSavedIter)
                    --savedIter;
                else
                    --iter;
            }
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem rbegin(" rbegin()", [&] {
        iteratorMode = ITERATOR_TYPE::Reverse;
        if (usingSavedIter)
            savedRiter = bstree.rbegin();
        else
            riter = bstree.rbegin();
       // Menu::console.setMenu(&navigationMenu);
    });
    MenuItem rend(" rend()", [&] {
        iteratorMode = ITERATOR_TYPE::Reverse;
        if (usingSavedIter)
            savedRiter = bstree.rend();
        else
            riter = bstree.rend();
        //Menu::console.setMenu(&navigationMenu);
    });
    MenuItem end(" end()", [&] {
        iteratorMode = ITERATOR_TYPE::Forward;
        if (usingSavedIter)
            savedIter = bstree.end();
        else
            iter = bstree.end();
        //Menu::console.setMenu(&navigationMenu);
    });
    MenuItem begin(" begin()", [&] {
        iteratorMode = ITERATOR_TYPE::Forward;
        if (usingSavedIter)
            savedIter = bstree.begin();
        else
            iter = bstree.begin();
        //Menu::console.setMenu(&navigationMenu);
    });
    MenuItem compare(" <=>", [&]{
        std::strong_ordering result;
        if (iteratorMode == ITERATOR_TYPE::Forward) {
            result = iter <=> savedIter;
        }
        else {
            result = riter <=> savedRiter;
        }
        std::cout << "\n";
        if (result == std::strong_ordering::equal) {
            std::cout << " Результат сравнения: основной == дополнительный";
        }
        if (result == std::strong_ordering::less) {
            std::cout << " Результат сравнения: основной < дополнительный";
        }
        if (result == std::strong_ordering::greater) {
            std::cout<< " Результат сравнения: основной > дополнительный";
        }
        _getch();
       
    });
    MenuItem outputIter(" * (чтение)", [&] {
        try {
            if (iteratorMode == ITERATOR_TYPE::Forward) {
                if (usingSavedIter) {
                    std::cout << "\n Значение по итератору: [" << (*savedIter).first << " : " << (*savedIter).second << "]";
                }
                else {
                    std::cout << "\n Значение по итератору: [" << (*iter).first << " : " << (*iter).second<<"]";
                }
            }
            else {
                if (usingSavedIter) {
                    std::cout << "\n Значение по итератору: [" << (*savedRiter).first << " : " << (*savedRiter).second << "]";
                }
                else {
                    std::cout << "\n Значение по итератору: [" << (*riter).first << " : " << (*riter).second << "]";
                }
            }
            _getch();
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem inputIter(" * (обновление)", [&] {
        std::cout << "\n Введите новое значение: ";
        value inputBuffer;
        std::cin >> inputBuffer;

        try {
            if (iteratorMode == ITERATOR_TYPE::Forward) {
                if (usingSavedIter) {
                    (*savedIter).second = inputBuffer;
                }
                else {
                    (*iter).second = inputBuffer;
                }
            }
            else {
                if (usingSavedIter) {
                    (*savedRiter).second = inputBuffer;
                }
                else {
                    (*riter).second = inputBuffer;
                }
            }

        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
        });

    iteratorOperations.addItem(begin);
    iteratorOperations.addItem(end);
    iteratorOperations.addItem(rbegin);
    iteratorOperations.addItem(rend);
    iteratorOperations.addItem(compare);
    iteratorOperations.addItem(plus);
    iteratorOperations.addItem(minus);
    iteratorOperations.addItem(outputIter);
    iteratorOperations.addItem(inputIter);
    //test_rand(50);
    //_getch();

    navigationMenu.addItem(iteratorMenu);
    navigationMenu.addItem(inputMenu);
    navigationMenu.addItem(at);
    navigationMenu.addItem(atnwrite);
    navigationMenu.addItem(size);
    navigationMenu.addItem(isEmpty);
    navigationMenu.addItem(clear);
    navigationMenu.addItem(erase);
    navigationMenu.addItem(keys);
    navigationMenu.addItem(find);
    navigationMenu.addItem(tests);
    navigationMenu.addItem(print);
    
    Menu::console.setMenu(&navigationMenu);
    Menu::console.show();

   



  
    //std::cout << s.size();
    //s.print();
    //std::cout << s.size();
    //s.clear();
    //s.insert(5, 2);
    //for (auto it = s.rbegin(); it < s.rend(); it++) {
    //    std::cout << (*it).first<<" ";
    //}
    //BinaryTree<std::string, int>::const_iterator a;
    //BinaryTree<std::string, int>::const_iterator b = a;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
