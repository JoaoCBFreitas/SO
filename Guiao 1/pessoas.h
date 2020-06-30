typedef struct Person{
    char name[256];
    int age;
}Person;
int new_person(char*name,int age);
int person_change_age(char*name,int age);
int person_change_age_v2(long pos,int age);