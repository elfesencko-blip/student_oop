#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Класс обслуживания: имя и приоритет
class TrafficClass {
protected:
    string name;
    int priority;
public:
    TrafficClass(const string& name, int priority) : name(name), priority(priority) {
        if (priority < 0) throw invalid_argument("priority must be non-negative");
    }

    string getName() const { return name; }
    int getPriority() const { return priority; }
};

// Пакет: ассоциирован с классом обслуживания (не владеет им)
class Packet {
protected:
    int id;
    const TrafficClass* cls;   // ассоциация
    double arrival;
public:
    Packet(int id, const TrafficClass& cls, double arrival)
        : id(id), cls(&cls), arrival(arrival) {
        if (arrival < 0) throw invalid_argument("arrival must be non-negative");
    }

    int getId() const { return id; }
    const TrafficClass& getTrafficClass() const { return *cls; }
    double getArrival() const { return arrival; }
};

int main() {
    TrafficClass voice("voice", 2);
    TrafficClass data("data", 1);

    Packet p1(1, voice, 0.0);
    Packet p2(2, data, 0.1);

    cout << "Packet " << p1.getId() << ": " << p1.getTrafficClass().getName()
         << ", priority=" << p1.getTrafficClass().getPriority() << "\n";
    cout << "Packet " << p2.getId() << ": " << p2.getTrafficClass().getName()
         << ", priority=" << p2.getTrafficClass().getPriority() << "\n";

    // Валидация
    try {
        Packet bad(3, voice, -1.0);
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}
