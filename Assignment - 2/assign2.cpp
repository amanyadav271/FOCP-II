#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

struct date {
    int day;
    int month;
    int year;
};

class person {
protected:
    string name;
    int age;
    string id;
    string contact_number;

public:
    person(string name, int age, string id, string contact_number) : name(name), age(age), id(id), contact_number(contact_number) {
        if (name.empty()) throw invalid_argument("Name cannot be empty.");
        if (age <= 0 || age > 130) throw invalid_argument("Age must be between 1 and 130.");
    }

    virtual ~person() {}
    virtual void getter() = 0;
    virtual void setter() = 0;
    virtual void display_details() = 0;
    virtual double calculate_payment() = 0;
};

class student : public person {
private:
    date enrollment_date;
    string program;
    float GPA;

public:
    student(string name, int age, string id, string contact_number, date enrollment_date, string program, float GPA = 0)
        : person(name, age, id, contact_number), enrollment_date(enrollment_date), program(program), GPA(GPA) {
        if (GPA < 0 || GPA > 4.0) throw invalid_argument("GPA must be between 0 and 4.0.");
    }

    ~student() {}

    void getter() override {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "ID: " << id << endl;
        cout << "Contact Number: " << contact_number << endl;
        cout << "Enrollment date: " << enrollment_date.day << "/" << enrollment_date.month << "/" << enrollment_date.year << endl;
        cout << "Program: " << program << endl;
        cout << "GPA: " << GPA << endl;
    }

    void setter() override {
        int choice;
        cout << "Choose what you need to change:" << endl << "1. Age" << endl << "2. Contact Number" << endl << "3. Enrollment date" << endl << "4. Program" << endl << "5. GPA" << endl;
        cin >> choice;
        switch (choice) {
            case 1:
                cin >> age;
                if (age <= 0 || age > 130) throw invalid_argument("Age must be between 1 and 130.");
                break;
            case 2:
                cin >> contact_number;
                break;
            case 3:
                cout << "DD MM YYYY: ";
                cin >> enrollment_date.day >> enrollment_date.month >> enrollment_date.year;
                break;
            case 4:
                cin >> program;
                break;
            case 5:
                cin >> GPA;
                if (GPA < 0 || GPA > 4.0) throw invalid_argument("GPA must be between 0 and 4.0.");
                break;
            default:
                cout << "Invalid Choice" << endl;
                break;
        }
    }

    void display_details() override {
        getter();
    }

    double calculate_payment() override {
        return 5000;
    }
};

class professor : public person {
private:
    string department;
    string specialization;
    date hire_date;

public:
    professor(string name, int age, string id, string contact_number, string department, string specialization, date hire_date) : person(name, age, id, contact_number), department(department), specialization(specialization), hire_date(hire_date) {}

    ~professor() {}

    void getter() override {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Teacher ID: " << id << endl;
        cout << "Contact Number: " << contact_number << endl;
        cout << "Hiring Date: " << hire_date.day << "/" << hire_date.month << "/" << hire_date.year << endl;
        cout << "Department: " << department << endl;
        cout << "Specialization: " << specialization << endl;
    }

    void setter() override {
        int choice;
        cout << "Choose what you need to change:" << endl << "1. Age" << endl << "2. Contact Number" << endl << "3. Hire date" << endl << "4. Department" << endl << "5. Specialization" << endl;
        cin >> choice;
        switch (choice) {
            case 1:
                cin >> age;
                if (age <= 0 || age > 130) throw invalid_argument("Age must be between 1 and 130.");
                break;
            case 2:
                cin >> contact_number;
                break;
            case 3:
                cout << "DD MM YYYY: ";
                cin >> hire_date.day >> hire_date.month >> hire_date.year;
                break;
            case 4:
                cin >> department;
                break;
            case 5:
                cin >> specialization;
                break;
            default:
                cout << "Invalid Choice" << endl;
                break;
        }
    }

    void display_details() override {
        getter();
    }

    double calculate_payment() override {
        return 8000;
    }
};

class course {
private:
    string code;
    string title;
    float credits;
    string description;

public:
    course(string code, string title, float credits, string description)
        : code(code), title(title), credits(credits), description(description) {
        if (credits <= 0) throw invalid_argument("Credits must be positive.");
    }

    ~course() {}

    void getter() {
        cout << "Code: " << code << endl;
        cout << "Title: " << title << endl;
        cout << "Credits: " << credits << endl;
        cout << "Description: " << description << endl;
    }
};

class department {
private:
    string name;
    string location;
    double budget;

public:
    department(string name, string location, double budget) : name(name), location(location), budget(budget) {}

    ~department() {}

    void getter() {
        cout << "Name: " << name << endl;
        cout << "Location: " << location << endl;
        cout << "Budget: " << budget << endl;
    }
};

class GradeBook {
private:
    vector<string> studentIds;
    vector<float> grades;

public:
    void add_grade(string id, float g) {
        if (g >= 0 && g <= 100) {
            studentIds.push_back(id);
            grades.push_back(g);
        } else {
            throw invalid_argument("Grade must be between 0 and 100.");
        }
    }

    float calculate_average_grade() {
        float total = 0;
        for (float grade : grades) total += grade;
        return grades.empty() ? 0 : total / grades.size();
    }

    float get_highest_grade() {
        float max = 0;
        for (float grade : grades) if (grade > max) max = grade;
        return max;
    }

    vector<string> get_failing_students() {
        vector<string> fail;
        for (int i = 0; i < grades.size(); i++) {
            if (grades[i] < 40) fail.push_back(studentIds[i]);
        }
        return fail;
    }
};

class EnrollmentManager {
private:
    vector<string> studentIds;

public:
    void enroll_student(string id) {
        studentIds.push_back(id);
    }

    void drop_student(string id) {
        for (int i = 0; i < studentIds.size(); i++) {
            if (studentIds[i] == id) {
                studentIds.erase(studentIds.begin() + i);
                break;
            }
        }
    }

    int get_enrollment_count() {
        return studentIds.size();
    }
};

void show_person_details(person* p) {
    try {
        p->display_details();
        cout << "Payment: " << p->calculate_payment() << endl;
    } catch (const exception& e) {
        cerr << "Error displaying person details: " << e.what() << endl;
    }
}

int main() {
    try {
        date d1 = {1, 1, 2020}, d2 = {15, 7, 2020}, d3 = {5, 3, 2015}, d4 = {9, 9, 2018};

        student s1("Aman", 20, "24CSU012", "6666666666", d1, "B.Tech", 3.5);
        student s2("Harsh", 20, "24CSU013", "555555555", d2, "B.Sc", 2.9);

        professor p1("Dr. Pratham", 45, "101", "777777777", "CSE", "AI & ML", d3);
        professor p2("Dr. Swaran Ahuja", 50, "102", "123456789", "ECE", "Electrical", d4);

        course c1("CS101", "CS", 3.0, "Basic concepts.");
        course c2("BEEE", "Electrical", 3.0, "Electrical.");

        department dpt1("CSE", "Block A", 5000000);
        department dpt2("ECE", "Block B", 4000000);

        GradeBook gb;
        gb.add_grade("24CSU012", 85);
        gb.add_grade("24CSU013", 35);

        EnrollmentManager em;
        em.enroll_student("24CSU012");
        em.enroll_student("24CSU013");

        person* people[] = {&s1, &s2, &p1, &p2};
        for (int i = 0; i < 4; i++) {
            show_person_details(people[i]);
            cout << "---------------------" << endl;
        }

        cout << "Average Grade: " << gb.calculate_average_grade() << endl;
        cout << "Highest Grade: " << gb.get_highest_grade() << endl;
        vector<string> fail = gb.get_failing_students();
        cout << "Failing Students: ";
        for (string s : fail) cout << s << " ";
        cout << endl;
        cout << "Total Enrollment: " << em.get_enrollment_count() << endl;

    } catch (const exception& e) {
        cerr << "Initialization error: " << e.what() << endl;
    }
    return 0;
}
