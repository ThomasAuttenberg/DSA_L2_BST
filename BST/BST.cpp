// BST.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "BinaryTree.h"
#include <map>
#include <list>
#include "Menu.h"
#include <conio.h>
#include <random>

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

    //list.push_back(3);
    //OPERATION_TYPE operationType;
    Menu navigationMenu([&]() {

        std::cout <<
            " ==================================================================\n\n"
            << " Используйте стрелки, чтобы переключаться между пунктами меню\n"
            << " Используйте TAB чтобы сменить тип итератора\n"
            << " Используемый итератор:\033[1;36;40m " << (iteratorMode == ITERATOR_TYPE::Forward ? "прямой" : "обратный") << "\033[0m "
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

    MenuItem inputMenu("insert(key,val) [Вставка значения]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        std::cout << "\n Введите значение: ";
        std::cin >> inputValueBuffer;
        bstree.insert(inputKeyBuffer, inputValueBuffer);
        if (bstree.size() == 1) {
            riter = bstree.rbegin();
            iter = bstree.begin();
        }
        Menu::console.setMenu(&navigationMenu);
        });
    MenuItem at("at(key) [Получение значения по ключу]", [&]{
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
    MenuItem atnwrite("at(key) (+write) [Запись значения по существующему ключу]", [&] {
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
    MenuItem size("size() [Опрос размера дерева]", [&] {
        std::cout << " Размер дерева: " << bstree.size();
        _getch();
    });
    MenuItem isEmpty("empty() [Опрос дерева на пустоту]", [&] {
        std::cout <<"\n"<< (bstree.empty() ? "Дерево пусто" : "Дерево не пусто");
        _getch();
    });
    MenuItem clear("clear() [Очистка дерева]", [&] {
        bstree.clear();
        iter = bstree.begin();
        riter = bstree.rbegin();
     });
    MenuItem erase("erase(key) [Удаление элемента по ключу]", [&] {
        std::cout << "\n Введите ключ: ";
        std::cin >> inputKeyBuffer;
        try {
            if (!bstree.erase(inputKeyBuffer)) {
                std::cout << " erase вернул false: значение с заданным ключом не представлено";
                _getch();
            }
            else {
                iter = bstree.begin();
                riter = bstree.rbegin();
            }
        }
        catch (std::exception ex) {
            std::cout << "\n\n Поймано исключение: " << ex.what();
            _getch();
        }
    });
    MenuItem keys("keys() [Получение списка ключей t -> L -> R ]", [&] {
        auto list = bstree.keys();
        std::cout << "\n\nПолный список ключей: ";
        for (auto i : list) {
            std::cout << "[" << i << "] ";
        }
        _getch();
    });

    
    //test_rand(50);
    //_getch();


    navigationMenu.addItem(inputMenu);
    navigationMenu.addItem(at);
    navigationMenu.addItem(atnwrite);
    navigationMenu.addItem(size);
    navigationMenu.addItem(isEmpty);
    navigationMenu.addItem(clear);
    navigationMenu.addItem(erase);
    navigationMenu.addItem(keys);
    Menu::console.setMenu(&navigationMenu);
    Menu::console.show();


   



    /*
    int n = 0;
    size_t _depth = 0;
    s.forEachHorizontal([&](int key, int val, size_t depth) {
        std::cout << "[" << key << ":" << val << "] ";
        if (_depth != depth) { _depth++; std::cout << "\n"; }
        });
    std::cout << "\n";
    size_t lastFixedDepth = 0;
    std::queue<std::vector<int>> toPrint;
    std::vector<int> currentDepthArray;
   /* s.forEachHorizontal([&](int key, int value, size_t depth) {
        if (lastFixedDepth != depth) {
            lastFixedDepth = depth;
            toPrint.push(currentDepthArray);
            currentDepthArray.clear();
        }
        currentDepthArray.push_back(key);
    });
    toPrint.push(currentDepthArray);
    int currentDepth = 0;
    while (!toPrint.empty()) {
        currentDepthArray = toPrint.front();
        toPrint.pop();
        for (int i = 0; i < currentDepthArray.size(); i++) {
            for (int j = 0; j < (lastFixedDepth - currentDepth)*3; j++) std::cout << " ";
            std::cout << currentDepthArray[i];
            for (int j = 0; j < (lastFixedDepth - currentDepth)*3; j++) std::cout << " ";
        }
        currentDepth++;
        std::cout << "\n";
    }*/
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
