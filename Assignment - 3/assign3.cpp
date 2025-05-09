#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip> // for setprecision
using namespace std;

struct date {
    int day, month, year;
};

class person {
protected:
    string name;
    int age;
    string id;
    string contact_number;

public:
    person(string name, int age, string id, string contact_number)
        : name(name), age(age), id(id), contact_number(contact_number) {
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
protected:
    date enrollment_date;
    string program;
    float GPA;
    vector<string> courses;

public:
    student(string name, int age, string id, string contact_number, date enrollment_date, string program, float GPA = 0)
        : person(name, age, id, contact_number), enrollment_date(enrollment_date), program(program), GPA(GPA) {
        if (GPA < 0 || GPA > 4.0) throw invalid_argument("GPA must be between 0 and 4.0.");
    }

    void enroll_in_course(string course_code) {
        courses.push_back(course_code);
    }

    virtual ~student() {}

    virtual void getter() override {
        cout << "Name: " << name << "\nAge: " << age << "\nID: " << id << "\nContact: " << contact_number
            << "\nEnrollment Date: " << enrollment_date.day << "/" << enrollment_date.month << "/" << enrollment_date.year
            << "\nProgram: " << program << "\nGPA: " << fixed << setprecision(2) << GPA << "\nCourses: "; //added fixed << setprecision(2)
        for (const auto& c : courses) cout << c << " ";
        cout << endl;
    }

    virtual void setter() override {
        // Simplified for brevity
        cout << "Setting student fields is supported.\n";
    }

    virtual void display_details() override {
        getter();
    }

    virtual double calculate_payment() override {
        return 5000;
    }
};

// === Inherited Student Classes ===

class UndergraduateStudent : public student {
    string major, minor;
    date expected_graduation;

public:
    UndergraduateStudent(string name, int age, string id, string contact_number, date enroll_date,
        string program, float GPA, string major, string minor, date grad_date)
        : student(name, age, id, contact_number, enroll_date, program, GPA),
        major(major), minor(minor), expected_graduation(grad_date) {}

    void display_details() override {
        student::display_details();
        cout << "Major: " << major << ", Minor: " << minor
            << ", Graduation: " << expected_graduation.day << "/" << expected_graduation.month << "/" << expected_graduation.year << endl;
    }

    double calculate_payment() override {
        return 4000; // Lower tuition
    }
};

class GraduateStudent : public student {
    string research_topic, advisor, thesis_title;

public:
    GraduateStudent(string name, int age, string id, string contact_number, date enroll_date,
        string program, float GPA, string research_topic, string advisor, string thesis_title)
        : student(name, age, id, contact_number, enroll_date, program, GPA),
        research_topic(research_topic), advisor(advisor), thesis_title(thesis_title) {}

    void display_details() override {
        student::display_details();
        cout << "Research Topic: " << research_topic << ", Advisor: " << advisor << ", Thesis: " << thesis_title << endl;
    }

    double calculate_payment() override {
        return 3000; // With assistantship
    }
};

// === Professor Classes ===

class professor : public person {
protected:
    string department, specialization;
    date hire_date;

public:
    professor(string name, int age, string id, string contact_number, string dept, string spec, date hire)
        : person(name, age, id, contact_number), department(dept), specialization(spec), hire_date(hire) {}

    virtual ~professor() {}

    virtual void getter() override {
        cout << "Name: " << name << "\nAge: " << age << "\nID: " << id << "\nContact: " << contact_number
            << "\nDepartment: " << department << "\nSpecialization: " << specialization
            << "\nHire Date: " << hire_date.day << "/" << hire_date.month << "/" << hire_date.year << endl;
    }

    virtual void setter() override {
        // Simplified
        cout << "Setting professor fields is supported.\n";
    }

    virtual void display_details() override {
        getter();
    }

    virtual double calculate_payment() override {
        return 8000;
    }
};

class AssistantProfessor : public professor {
    int years_of_service;

public:
    AssistantProfessor(string name, int age, string id, string contact, string dept, string spec, date hire, int years)
        : professor(name, age, id, contact, dept, spec, hire), years_of_service(years) {}

    void display_details() override {
        professor::display_details();
        cout << "Rank: Assistant Professor\nYears of Service: " << years_of_service << endl;
    }

    double calculate_payment() override {
        return 6000 + 100 * years_of_service;
    }
};

class AssociateProfessor : public professor {
    int publications;

public:
    AssociateProfessor(string name, int age, string id, string contact, string dept, string spec, date hire, int pubs)
        : professor(name, age, id, contact, dept, spec, hire), publications(pubs) {}

    void display_details() override {
        professor::display_details();
        cout << "Rank: Associate Professor\nPublications: " << publications << endl;
    }

    double calculate_payment() override {
        return 9000 + 50 * publications;
    }
};

class FullProfessor : public professor {
    double research_grants;

public:
    FullProfessor(string name, int age, string id, string contact, string dept, string spec, date hire, double grants)
        : professor(name, age, id, contact, dept, spec, hire), research_grants(grants) {}

    void display_details() override {
        professor::display_details();
        cout << "Rank: Full Professor\nResearch Grants: $" << fixed << setprecision(2) << research_grants << endl; //added fixed << setprecision(2)
    }

    double calculate_payment() override {
        return 12000 + 0.05 * research_grants;
    }
};

// === Aggregation ===

class course {
private:
    string code, title;
    float credits;
    string description;
    professor* instructor; // Aggregated

public:
    course(string code, string title, float credits, string desc, professor* prof)
        : code(code), title(title), credits(credits), description(desc), instructor(prof) {}

    void getter() {
        cout << "Course: " << title << " (" << code << ") - " << credits << " credits\nDescription: " << description << endl;
        if (instructor) {
            cout << "Instructor: ";
            instructor->display_details();
        }
    }
};

class department {
private:
    string name, location;
    double budget;
    vector<professor*> professors;

public:
    department(string name, string location, double budget)
        : name(name), location(location), budget(budget) {}

    void add_professor(professor* prof) {
        professors.push_back(prof);
    }

    void getter() {
        cout << "Department: " << name << ", Location: " << location << ", Budget: $" << fixed << setprecision(2) << budget << endl; //added fixed << setprecision(2)
        cout << "Professors:\n";
        for (auto* prof : professors)
            prof->display_details();
    }
};

// === Test ===

void show_person_details(person* p) {
    p->display_details();
    cout << "Payment: ₹" << fixed << setprecision(2) << p->calculate_payment() << "\n-------------------\n"; //added fixed << setprecision(2)
}

int main() {
    date d = { 1, 1, 2022 }, grad = { 30, 6, 2025 };

    UndergraduateStudent ugs("Ram", 19, "UG001", "111111", d, "B.Tech", 3.8, "CSE", "Maths", grad);
    GraduateStudent gs("Rohan", 24, "GR002", "222222", d, "M.Tech", 3.9, "AI Research", "Dr. Ahuja", "AI for Safety");

    AssistantProfessor ap("Dr. Raj", 35, "P001", "333-333", "CSE", "Systems", d, 5);
    AssociateProfessor asp("Dr. Meena", 45, "P002", "444-444", "ECE", "Electronics", d, 12);
    FullProfessor fp("Dr. Kumar", 55, "P003", "555-555", "CSE", "AI", d, 1000000);

    department cse("CSE", "Block A", 2000000);
    cse.add_professor(&ap);
    cse.add_professor(&fp);

    course cs101("CS101", "Intro to CS", 3.0, "Basic programming concepts", &ap);

    person* people[] = { &ugs, &gs, &ap, &asp, &fp };
    for (auto* p : people) show_person_details(p);

    cout << "\nDepartment Info:\n";
    cse.getter();

    cout << "\nCourse Info:\n";
    cs101.getter();

    return 0;
}
