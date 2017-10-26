#ifndef PERSON_HPP
#define PERSON_HPP

#include <QString>
#include <QObject>

class Person : public QObject {
    Q_OBJECT
  public:
    Person(int age, QString name);
    Person(const Person& person);
    Person() {
    }
    ~Person();

    int getAge() const;
    int getTimes() const {
        return m_timesModified;
    }
    QString getName() const;
  public slots:
    void slotInfoChanged(int age, const QString& name);

  private:
    int m_age;
    QString m_name;
    int m_timesModified;
};

#endif
