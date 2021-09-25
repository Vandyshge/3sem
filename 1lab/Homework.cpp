#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Electromagnetic interaction
// You have to create world and be able to add and delete particles from it
// When you add or delete particle you must recalculate all interactions

// Требуется описать мир с некоторыми частицами
// Координата/скорость задается через структуру vector3

struct vector3 {
    double x;
    double y;
    double z;

    double length(vector3 r) {
        double L2 = pow((r.x - x), 2) + pow((r.y - y), 2) + pow((r.z - z), 2);
        // std::cout << "L2: " << L2 << std::endl;
        return pow(L2, 0.5);
    }

    vector3 r(vector3 r) {
        vector3 r1 =  {x-r.x, y-r.y, z-r.z};
        return r1;
    }

    std::string vector3_to_s() {
        std::string s = "{" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "}";
        return s;
    }
};

// Для описания событий происходящих в мире напишите свой небольшой логгер, который умеет
// Запоминать создание, изменение и уничтожение частиц

class Logger {
public:
    Logger() = default;
    void log(const std::string& s) {
        l.push_back(s);
    }
    ~Logger() = default;
private:
    std::vector<std::string> l;
};

// Существует базовый класс частиц, который внутри себя хранит информацию о частице
// Допишите этот родительский класс так, чтобы он мог считать количество объектов, созданных от него

class Particle {
protected:
    // Поля, описывающие частицу
    // Можно дописать свои
    double mass = 0;
    double charge = 0;
    vector3 coordinate = {0, 0, 0};
    vector3 velocity = {0, 0,0};
    vector3 force = {0, 0, 0};
    std::string name;
    // Логгер для записи событий
    static Logger logger;
    double dt = 0;
    static int num_particular;
public:
    Particle() {num_particular++;}
    explicit Particle(vector3 r, vector3 v, double mass_, double charge_, std::string name_, double dt_) :
        mass(mass_), charge(charge_), coordinate(r), velocity(v), name(name_), dt(dt_) {        name = name_;
        logger.log("created: " + name_);
        num_particular++;
    }
    // метод, который вам необходимо переопределить в дочерних классах
    // см. или вспомнить лекцию
    // внутри него напишите логику взаимодействия двух частиц (например, кулоновское)
    void change_force(Particle* p) {
        double l = coordinate.length(p->coordinate);
        vector3 r = coordinate.r(p->coordinate);
        double f = p->charge*charge/(pow(l, 2));
        // std::cout << l << "   " << r.vector3_to_s() << std::endl;
        force.x = force.x + f*r.x/l;
        force.z = force.z + f*r.z/l;
        force.y = force.y + f*r.y/l;
        // std::cout << force.vector3_to_s() << std::endl;
    }

    void interaction() {
        velocity = {velocity.x + force.x/mass*dt, velocity.y + force.y/mass*dt, velocity.z + force.z/mass*dt};
        force = {0, 0, 0};
        coordinate = {coordinate.x + velocity.x*dt, coordinate.y + velocity.y*dt, coordinate.z + velocity.z*dt};
        logger.log("change: name=" + name + ", coordinate=" + coordinate.vector3_to_s());
    }

    std::string getNum() {
        // std::cout << velocity.length({0, 0, 0}) << std::endl;
        return std::to_string(num_particular);
    }

    std::string getName() {
        return name;
    }

    std::string getEnergy() {
        // std::cout << velocity.length({0, 0, 0}) << std::endl;
        return std::to_string(mass * pow(velocity.length({0, 0, 0}), 2) / 2);
    }

    std::string getCoor() {
        return coordinate.vector3_to_s();
    }

    std::string getVel() {
        return velocity.vector3_to_s();
    }

    ~Particle() {
        logger.log("destroyed: " + name);
        num_particular--;
    }
};

int Particle::num_particular = 0;
Logger Particle::logger;


// описать несколько классов разных частиц
// например такой
class Electron : public Particle {
public:
    // в этом конструкторе предлагается задавать координаты и через энергию остальные параметры частицы
    explicit Electron(vector3 coords, vector3 vel, std::string name_, double dt) : Particle(coords, vel, 0.1, -1, name_, dt) {
        name = "e_" + name_;
    }
    ~Electron();
};

class Proton : public Particle {
public:
    // в этом конструкторе предлагается задавать координаты и через энергию остальные параметры частицы
    explicit Proton(vector3 coords, vector3 vel, std::string name_, double dt) : Particle(coords, vel, 1, 1, name_, dt) {
        name = "p_" + name_;
    }
    ~Proton();
};

class Neutron : public Particle {
public:
    // в этом конструкторе предлагается задавать координаты и через энергию остальные параметры частицы
    explicit Neutron(vector3 coords, vector3 vel, std::string name_, double dt) : Particle(coords, vel, 1, 0.01, name_, dt) {
        name = "n_" + name_;
    }
    ~Neutron();
};

// в этом классе опишите мир, состоящий из некоторого числа частиц
class World {
    double dt = 0.1;
public:
    std::vector<Particle *> v;

    double rand_v(int energy) {
        double pos = 1;
        if (rand()%2 == 0) {
            pos = -1;
        }
        return pos * (rand()% energy) / 100;
    }

    double rand_r() {
        double pos = 1;
        if (rand()%2 == 0) {
            pos = -1;
        }
        return pos * (rand()%1000) / 100;
    }

    // добавить частицу в мир (она должна провзаимодействовать со всеми остальными)
    void addParticle(int type, std::string name) {
        int energy = rand()%1000;
        vector3 vel = {rand_v(energy), rand_v(energy), rand_v(energy)};
        vector3 coor = {rand_r(), rand_r(), rand_r()};
        Particle *p = NULL;
        if (type == 0) {
            // std::cout << 100 << std::endl;
            p = (Particle *) new Electron(coor, vel, name, dt);
        } else if (type == 1) {
            p = (Particle *) new Proton(coor, vel, name, dt);
        } else if (type == 2) {
            p = (Particle *) new Neutron(coor, vel, name, dt);
        }
        if (p != NULL) v.push_back(p);
    }

    // удалить частицу из мира
    void deleteParticle(const Particle* p) {
        for (auto i = 0; i < v.size(); i++) {
            if (v[i] == p) {
                delete p;
                v.erase(v.begin() + i);
                
                break;
            }
        }
    }

    void interaction() {
        for (auto &pi : v) {
            for (auto &pj : v) {
                if (pi != pj) {
                    pi->change_force(pj);
                }
            }
        }
        for (auto p:v) {
            p->interaction();
        }
    }

    ~World() {
        for (auto p : v) {
            delete p;
        }
    }
};

// перегрузите оператор вывода, чтобы можно было вывести состав мира
// (может потребоваться перегрузка оператора вывода для частиц)
std::ostream &operator<<(std::ostream &os, Particle &p) {
    // std::cout << 0 << std::endl;
    os << p.getName() << ": energy: " << p.getEnergy() << " ; position: " << p.getCoor() << " ; velocity: " << p.getVel() << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, World &w) {
    // std::cout << 0 << std::endl;
    os << "-------------------------------------------------------------------------------------------" << std::endl;
    os << "info:" << std::endl;
    for (auto &p : w.v) {
        // std::cout << p << std::endl;
        os << (*p);
        // os << p->getName() << ": energy: " << p->getEnergy() << " ; position: " << p->getCoor() << " ; velocity: " << p->getVel() << std::endl;
    }
    os << "-------------------------------------------------------------------------------------------" << std::endl;
    return os;
}

// здесь создайте мир/миры, куда добавьте некоторое число частиц, взаимодействующих друг с другом
int main() {
    int epoch = 0;
    World world;
    world.addParticle(0, "1");
    // std::cout << world.v.size();
    world.addParticle(1, "2");
    world.addParticle(2, "3");
    world.addParticle(0, "4");
    std::cout << world;
    for (auto i = 0; i < epoch; i++) {
        world.interaction();
        std::cout << world;
    }
    world.deleteParticle(world.v[0]);
    std::cout << world;
    std::cout << world.v[0]->getNum() << "\n";
}
