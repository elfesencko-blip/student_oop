#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

//создать класс Student с хранением информации об оценках в объекте,
// предусмотреть методы для добавления оценок и их просмотра

class Student {
private:
    std::string name;
    std::string faculty;
    int course;
    std::vector<int> grades;

public:
    // основной конструктор позволяет создать студента с обязательными данными,
    //но без оценок
    Student(const std::string& n, const std::string& f, int c)
            : name(n), faculty(f), course(c) {
    }

    // конструктор с оценками используется при добавлении студента,
    // уже имеющего оценки
    Student(const std::string& n, const std::string& f, int c,
            std::initializer_list<int> g)
            : name(n), faculty(f), course(c) {
        for (int grade : g) {
            add_grade(grade);
        }
    }

    // проверка
    void add_grade(int grade) {
        if (grade < 1 || grade > 5) {
            std::cout << "Ошибка: оценка должна быть от 1 до 5\n";
            return;
        }
        grades.push_back(grade);
    }

    // просмотр оценок
    void print_grades() const {
        std::cout << "Студент: " << name
                  << " | Факультет: " << faculty
                  << " | Курс: " << course << "\n";
        if (grades.empty()) {
            std::cout << "  Оценок нет\n";
            return;
        }
        std::cout << "  Оценки: ";
        for (int g : grades) {
            std::cout << g << " ";
        }
        std::cout << "\n";
    }

    // считаем средний балл
    double average() const {
        if (grades.empty()) return 0.0;
        int sum = 0;
        for (int g : grades) sum += g;
        return static_cast<double>(sum) / grades.size();
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Student s1("Иван Петров", "ФИТ", 2);           // основной конструктор
    Student s2("Анна Сидорова", "Экономика", 1);
    Student s3("Пётр Иванов", "ФИТ", 3, {5, 4, 5}); // с оценками

    s1.add_grade(5);
    s1.add_grade(4);
    s1.add_grade(5);

    s2.add_grade(3);
    s2.add_grade(4);

    s3.add_grade(4);
    s3.add_grade(4);

    s1.print_grades();
    std::cout << "  Средний балл: " << s1.average() << "\n\n";

    s2.print_grades();
    std::cout << "  Средний балл: " << s2.average() << "\n\n";

    s3.print_grades();
    std::cout << "  Средний балл: " << s3.average() << "\n";

    return 0;
}