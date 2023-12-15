#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <iostream> 
#include <fstream>
#include <random>
#include <unistd.h>
#include <fstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

std::random_device rd;
std::mt19937 range(rd());
//генератор стоимости украденного
std::uniform_int_distribution<int> costRange(200, 1500000);
//генератор, сколько секунд требуется Иванову на взятие и принос в кучу товара для Петрова
std::uniform_int_distribution<int> timeToTake(3, 10);
//генератор, сколько секунд требуется Петрову, чтобы он перенёс товар до машины
std::uniform_int_distribution<int> timeToCarry(2, 15);
//генератор, сколько секунд требуется Нечепоруку, чтобы он посчитал стоимость товара
std::uniform_int_distribution<int> timeToCount(1, 2);

//переменная для файла, в который мы будем записывать вывод
std::ofstream fileForWrite;


//простой класс для реализации имущества
class Production
{
    private:
    int Cost;
    public:
    Production(int cost){
        Cost = cost;
    }
    int GetCost(){
        return Cost;
    }
};

//метод для проверки строки, является ли она числом
bool is_number(const std::string& s)
{
    return !s.empty() && (s.find_first_not_of("0123456789") == s.npos);
}


sem_t semaphore1;
sem_t semaphore2;
sem_t semaphore3;

//стоимость похищенного
static int productionCost = 0;

//массив имущества, уже перетащенных
std::vector<Production> productionsInTheCar;

//массив имущества, которые Иван принёс в кучу для Петрова
std::vector<Production> CurrProdroductions;

void* TakeOutGood(void* args){
    int count = *(int*)args;
    for (size_t i = 0; i < count; i++)
    {
        std::cout<<"Иванов в процессе выноса товара со склада..." << std::endl;
        fileForWrite << "Иванов в процессе выноса товара со склада..." <<'\n';
        sleep(timeToTake(range)); //делаем вид, что он выносит товар какое-то время через метод sleep
        Production prod(costRange(range)); //генерируем украденное им имущество
        CurrProdroductions.push_back(prod);
        std::cout<<"Иванов принёс со склада в кучу товар стоимостью: "<< prod.GetCost() <<" руб.\nВсего предметов в куче, принесённых Иваном: "<<CurrProdroductions.size() << std::endl;
        fileForWrite << "Иванов принёс со склада в кучу товар стоимостью: "<< prod.GetCost() <<" руб.\nВсего предметов в куче, принесённых Иваном: "<<CurrProdroductions.size() << '\n';
        sem_post(&semaphore2);//разрешаем Петрову перенос, так как куча уже не пустая, и может даже накапливаться
    }
    return nullptr;
}

void* CarryGood(void* args){
    int count = *(int*)args;
    for (size_t i = 0; i < count; i++)
    {
        sem_wait(&semaphore2); // ожидаем, когда куча не перестанет быть пустой (семофор позволяет отображать потоку, сколько раз он может обратиться к вектору дальше по коду, благодаря связи с функцией выше
        Production currProd = CurrProdroductions.back(); //берём товар
        CurrProdroductions.pop_back(); //изымаем имущество из кучи Ивана
        std::cout<<"Петров в процессе переноса товара из кучи в машину\nВсего предметов в куче после изъятия одного предмета, принесённых Иваном: "<<CurrProdroductions.size()<< std::endl;
        fileForWrite << "Петров в процессе переноса товара из кучи в машину\nВсего предметов в куче после изъятия одного предмета, принесённых Иваном: "<<CurrProdroductions.size()<< '\n';
        sleep(timeToCarry(range));
        productionsInTheCar.push_back(currProd); //добавляем принесённый товар к остальным
        std::cout<<"Петров перетащил товар стоимостью "<< (productionsInTheCar[productionsInTheCar.size() - 1]).GetCost() << " руб. в машину" << std::endl;
        fileForWrite << "Петров перетащил товар стоимостью "<< (productionsInTheCar[productionsInTheCar.size() - 1]).GetCost() << " руб. в машину" << '\n';
        sem_post(&semaphore3); //разрешаем Нечепоруку считать
    }
    return nullptr;
}

void* CountSum(void* args){
    int count = *(int*)args;
    for (size_t i = 0; i < count; i++)
    {
        sem_wait(&semaphore3);
        sleep(timeToCount(range));
        productionCost += (productionsInTheCar[productionsInTheCar.size() - 1]).GetCost(); //прибавляем к итоговой сумме стоимость последнего принесённого товара
        std::cout<<"Нечепорук посчитал стоимость украденного имущества. Всего украдено украдено товаров: " << productionsInTheCar.size() << " из " << count << " общей стоимостью: " << productionCost <<" руб." << std::endl;
        fileForWrite << "Нечепорук посчитал стоимость украденного имущества. Всего украдено украдено товаров: " << productionsInTheCar.size() << " из " << count << " общей стоимостью: " << productionCost <<" руб." << '\n';
    }
    //итоги
    std::cout<<"Итоговая стоимость похищенного: "<< productionCost <<" руб.\nВсего украдено в итоге: "<< productionsInTheCar.size() << " предметов" << std::endl;
    fileForWrite << "Итоговая стоимость похищенного: "<< productionCost <<" руб.\nВсего украдено в итоге: "<< productionsInTheCar.size() << " предметов" << '\n';
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        std::cout<< "Неверное количество аргументов! Введите 0, положительное число, путь к файлу для вывода\nили\n1, путь к файлу для чтения, путь к файлу для вывода";
        return 0;
    }
    fileForWrite = std::ofstream(argv[3]);
    if(!fileForWrite.is_open()){
        std::cout<<"Такого файла не существует или он не открылся";
        return 0;
    }
    //проверка ключей
    if (*(argv[1]) != '0' && *(argv[1]) != '1')
    {
        std::cout<< "Первый аргумент должен быть числом: 0 или 1!";
        fileForWrite << "Первый аргумент должен быть числом: 0 или 1!";
        return 0;
    }
    int* count = new int(0);
    if (*(argv[1]) == '0')
    {
        if (!is_number(argv[2]) || std::stoi(argv[2]) <= 0)
        {
            std::cout<< "Второй аргумент должен быть положительным числом, если первый является нулём!";
            fileForWrite<< "Второй аргумент должен быть положительным числом, если первый является нулём!";
            return 0;
        }
        *count = std::stoi(argv[2]);
    }
    else{
        std::ifstream fileForRead(argv[2]);
        if(!fileForRead.is_open()){
            std::cout<< "Такого файла не существует или он не открылся";
            fileForWrite<< "Такого файла не существует или он не открылся";
            return 0;
        }
        else{
            int number = 0;
            fileForRead >> number;
            if (number <= 0)
            {
                std::cout << "в файле должно быть число и больше нуля!";
                fileForWrite<< "в файле должно быть число и больше нуля!";
                return 0;
            }
            *count = number;
            fileForRead.close();
        }
    }
    pthread_t Ivanov;
    pthread_t Petrov;
    pthread_t Necheporuck;
    sem_init(&semaphore2, 0, 0);
    sem_init(&semaphore3, 0, 0);
    std::cout<<"Операция началась!\n";
    fileForWrite<< "Операция началась!\n";
    pthread_create(&Ivanov, NULL, TakeOutGood, (void *)(count));
    pthread_create(&Petrov, NULL, CarryGood, (void *)(count));
    pthread_create(&Necheporuck, NULL, CountSum, (void *)(count));
    
    pthread_join(Ivanov, NULL);
    pthread_join(Petrov, NULL);
    pthread_join(Necheporuck, NULL);

    sem_destroy(&semaphore2);
    sem_destroy(&semaphore3);
    fileForWrite.close();
    return 0;
}
#pragma GCC diagnostic pop
