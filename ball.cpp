// создать классы, создать несколько объектов Ball, два или несколько - Box
// распределить несколько шариков по коробкам
// коробка-объект отвечает, какие в ней шарики (цвет)
// шарик "отвечает", в коробке ли он, если да - в какой

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

class Box;

class Ball {
    std::string color;

    // Указатель нужен потому, что шарик может быть "не в коробке", т.е. ссылаться "в никуда"
    Box* box = nullptr;

public:
    Ball(const std::string& c) : color(c) {}

    // Возвращаем ссылку на строку без копирования, ссылка гарантирует, что объект существует
    const std::string& getColor() const { return color; }

    // шарик отвечает, в коробке ли он
    // Сравниваем указатель с nullptr, проверяем лежит ли шарик где-нибудь.
    bool isInBox() const { return box != nullptr; }

    // шарик отвечает, в какой коробке он
    std::string whichBox() const;

    // Коробка передаётся по ссылке, а не по значению -
    // это избегает копирования и гарантирует, что коробка существует.
    // Внутри метода адрес этой коробки сохранится в указателе box
    void putInto(Box& target);

    void remove();

    // friend даёт классу Box доступ к приватному полю Ball::color
    friend class Box;
};

class Box {
    std::string name;
    double length, width, height;

    // вектор указателей, так как коробка не владеет шариками, а лишь ссылается на них
    std::vector<Ball*> balls;

public:
    // конструкторы куба и параллепипеда
    Box(const std::string& n, double side)
            : name(n), length(side), width(side), height(side) {}

    Box(const std::string& n, double l, double w, double h)
            : name(n), length(l), width(w), height(h) {}

    // Ссылка const& без копирования строки имени
    const std::string& getName() const { return name; }
    double getVolume() const { return length * width * height; }

    // Rоробка отвечает, какого цвета в ней шарики
    std::vector<std::string> getBallColors() const {
        std::vector<std::string> colors;
        // разыменовываем
        for (const Ball* b : balls) colors.push_back(b->getColor());
        return colors;
    }

    // Указатель принимает адрес шарика.
    void add(Ball* b) {
        if (std::find(balls.begin(), balls.end(), b) == balls.end())
            balls.push_back(b);
    }

    void remove(Ball* b) {
        balls.erase(std::remove(balls.begin(), balls.end(), b), balls.end());
    }

    void info() const {
        std::cout << "Коробка \"" << name << "\" (объём " << getVolume() << "): ";
        if (balls.empty()) { std::cout << "пуста\n"; return; }
        for (size_t i = 0; i < balls.size(); ++i) {
            std::cout << balls[i]->getColor();
            if (i + 1 < balls.size()) std::cout << ", ";
        }
        std::cout << "\n";
    }
};


// Реализация методов Ball
void Ball::putInto(Box& target) {
    // &target — берём АДРЕС коробки, переданной по ссылке.
    // Сравниваем с текущим указателем box: если шарик уже в этой коробке — выходим.
    if (box == &target) return;

    if (box) box->remove(this);

    // Сохраняем адрес новой коробки в указателе
    box = &target;
    target.add(this);
}

void Ball::remove() {
    if (!box) return;

    // Сохраняем указатель на старую коробку прежде чем обнулить box
    Box* old = box;

    // Обнуляем указатель, шарик нигде
    box = nullptr;

    // Через сохранённый указатель просим старую коробку убрать нас из vector
    old->remove(this);
}

std::string Ball::whichBox() const {
    return box ? box->getName() : "не в коробке";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // создать несколько объектов Ball
    // Объекты создаются на стеке, ими владеет main
    Ball red("красный"), green("зелёный"), blue("синий"),
            yellow("жёлтый"), white("белый");

    // создать два или несколько объектов Box
    Box boxA("A", 5.0);
    Box boxB("B", 10.0, 5.0, 4.0);
    Box boxC("C", 3.0);

    // распределить несколько шариков по коробкам
    // putInto принимает ссылку на коробку,
    // без взятия адреса &, компилятор сам подставит ссылку
    red.putInto(boxA);
    green.putInto(boxA);
    blue.putInto(boxB);
    yellow.putInto(boxB);
    white.putInto(boxC);

    // коробка отвечает, какие в ней шарики
    std::cout << "Коробки отвечают, какие в них шарики\n";
    boxA.info();
    boxB.info();
    boxC.info();

    // шарик отвечает, в коробке ли он, и если да - в какой
    std::cout << "\nШарики отвечают, где они\n";
    Ball* all[] = { &red, &green, &blue, &yellow, &white };
    for (Ball* b : all) {
        if (b->isInBox())
            std::cout << "Шарик " << b->getColor()
                      << ": в коробке \"" << b->whichBox() << "\"\n";
        else
            std::cout << "Шарик " << b->getColor()
                      << ": не в коробке\n";
    }

    // перекладывание и удаление
    std::cout << "\nПерекладываем красный из A в B\n";
    red.putInto(boxB);
    boxA.info();
    boxB.info();
    std::cout << "красный -> " << red.whichBox() << "\n";

    std::cout << "\nУбираем зелёный из коробки\n";
    green.remove();
    boxA.info();
    std::cout << "зелёный в коробке? " << (green.isInBox() ? "да" : "нет") << "\n";

    return 0;
}