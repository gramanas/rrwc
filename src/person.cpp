#include "person.hpp"

Person::Person(int age, QString name)
    : m_age(age),
      m_name(name),
      m_timesModified(0) {
}

Person::Person(const Person& person) {
    m_name = person.getName();
    m_name = person.getAge();
    m_timesModified = 0;
}

Person::~Person() {
}

int Person::getAge() const {
    return m_age;
}

QString Person::getName() const {
    return m_name;
}

void Person::slotInfoChanged(int age, const QString& name) {
    m_name = name;
    m_age = age;
    m_timesModified++;
}
