// BST.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "BinaryTree.h"

int main()
{
    struct meow {
        int a;
    };
    BinaryTree<int, int> s;
    s.insert(4, 5);
    s.insert(2, 0);
    s.insert(3, 11);
    s.insert(5, 15);
    //auto it = s.begin();
    
    for (auto i = s.rbegin(); i < s.rend(); i++) std::cout << (*i).first << " ";
    
    

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
