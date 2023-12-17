# Хавронич Евгений Алексеевич, БПИ226
---
## Вариант 30
### Условие:
Военная операция. Темной-темной ночью прапорщики Иванов, Петров и Нечепорук занимаются хищением военного имущества со склада родной военной части. Будучи умными людьми и отличниками боевой и строевой подготовки, прапорщики ввели разделение труда. Иванов выносит имущество со склада и передает его в руки Петрову, который грузит его в грузовик. Нечепорук стоит на шухере и заодно подсчитывает рыночную стоимость добычи поле погрузки в грузовик очередной партии похищенного. Требуется составить многопоточное приложение, моделирующее деятельность прапорщиков–потоков. Необходимо учесть случайное время выполнения каждым прапорщиком своей боевой задачи и организовать в программе корректную их синхронизацию.
### Программы:
[Программа с комментариями и файлы ввода/выводаразмещены здесь](files)

## Программа была написана на оценку 8.

### Требования на 8:
- В программу, наряду с выводом в консоль, добавлен вывод результатов в файл. Имя файла для вывода данных задается в командной строке как один из ее параметров.

Итого список аргументов: ключ для определения ввода данных (0, если ввод числа в следующем аргументе или 1, если считать из файла), число или путь к файлу, откуда его необходимо считать, путь к файлу, куда необходимо записаь вывод.
- Результаты работы программы выводится на экран и записывается в файл.
- Наряду с вводом исходных данных через командную строку добавлен альтернативный вариант их ввода из файла, который по сути играет роль конфигурационного файла. Имя этого файла задается в командной строке вместо параметров, которые в этом случае не вводятся.

Управление вводом в командной строке осуществляется с использованием соответствующих ключей( в качестве первого параметра передаются 0 или 1, см. первый пункт).
- Приведено не менее трех вариантов входных ([input1.txt](files/input1.txt), [input2.txt](files/input2.txt), [input3.txt](files/input3.txt)) и выходных([output1.txt](files/output1.txt), [output2.txt](files/output2.txt), [output3.txt](files/output3.txt)) файлов с различными исходным данными и результатами выполнения программы.
- Ввод данных из командной строки расширен с учетом введенных изменений (см. первый пункт).
<table>
    <tr>
        <th>№ теста</th>
        <th>Какую ситуацию проверяет</th>
        <th>Вывод</th>
        <th>Корректен?</th>
    </tr>
    <tr>
        <td>1</td>
        <td>Файл содержит число, не входящее в нужный диапозон (больше нуля)</td>
        <td>в файле должно быть число и больше нуля!</td>
        <td>Корректен, так как мы принимаем только положительные числа</td>
    </tr>
    <tr>
        <td>2</td>
        <td>Файл содержит небольшое число</td>
        <td>В файле представлен вывод кражи 5 вещей</td>
        <td>Корректен, так как мы указали, что они будут красть 5 товаров</td>
    </tr>
    <tr>
        <td>3</td>
        <td>Файл содержит достаточно большое число (берём с тем учётом, что программа выполняется подольше на большем количестве предметов, поэтому 25 уже считается большим)</td>
        <td>В файле представлен вывод кражи 25 вещей</td>
        <td>Корректен, так как мы указали, что они будут красть 25 товаров</td>
    </tr>
</table>

### Требования на 6-7:
- В отчете подробно описан обобщенный алгоритм, используемый при реализации программы исходного словесного сценария. В котором показано, как на программу отображается каждый из субъектов предметной области.
Создаём три потока, олицетворяющих каждого человека из задания. У каждого потока есть своя функция:
1. У Иванова void* TakeOutGood(void* args) – принимает аргумент, который будет означать, сколько товаров ему необходимо украсть. Прежде всего оповещаем, что Иванов начинает работать, при помощи sleep делаем иллюзию по времени, что он какое-то время переносит(при этом время генерируется случайно), генерируем продукт, украденный им (стоимость генерируется случайно), добавляем объект в кучу для Петра (второй из векторов), сообщаем, что товар перенесён, и даём потоку Петра начать действовать, увеличивая счётчик семафора.
2. У Петрова void* CarryGood(void* args) – принимает аргумент, который будет означать, сколько товаров ему необходимо будет перенести в общей сложности. Через семафор ждёт, пока в куче появится предмет для изъятия (ненулевой счётчик семафора), после чего берём предмет, убирая его из кучи, сообщаем о начале переноса, по аналогии через sleep делаем иллюзию протяжённости по времени. Приносим товар в машину (первый вектор), сообщаем об этом, и даём через пост второго семафора доступ Нечепоруку добавить к общей стоимости принесённый новый товар.
3. У Нечепорука void* CountSum(void* args) – принимает аргумент, который будет означать, сколько раз ему необходимо будет прибавить к итоговой стоимости принесённых товаров. Через семафор уменьшаем счётчик, чтобы потом CarryGood его снова добавила в случае чего, и обращаемся к стоимости последнего принесённого в машину товара (в первом векторе это последний элемент), прибавляя его к итоговой стоимости – статической переменной. В последнем вызове делаем итог, сколько было украдено товаров и на какую стоимость.
- В программу добавлена генерация случайных данных в допустимых диапазонах.
В начале файла программы сгенерированы переменные для генерации стоимости товара, времени переноса товара Ивановым, Петровым и подсчёта стоимости Нечепоруком.
- Реализован ввод исходных данных из командной строки при запуске программы вместо ввода параметров с консоли во время выполнения программы.
В командной строке вводилось одно число – количество имущества, которого нужного украсть.
- Результаты изменений отражены в отчете.

### Требования на 4-5:
Соблюдены общие требования к отчету.
- В отчете должен быть приведен сценарий, описывающий одновременное поведение представленных в условии задания сущностей в терминах предметной области. То есть, описан сценарий, задающий ролевое поведение субъектов и объектов задачи (интерпретация условия с большей степенью детализации происходящего), а не то, как это будет реализовано в программе.
В нашем случае Иванов похищает имущество, кладёт его в кучу (то есть может копить товары). Петров ожидает, пока эта куча не станет не пустой, и тогда переносит. При переносе Петровым, Нечепорук добавляет стоимость принесённого товара к общей стоимости награбленного. При этом все они знают, сколько товаров они хотят украсть.
- Описана модель параллельных вычислений, используемая при разработке многопоточной программы.
Иванов крадёт товары независимо от действий Петрова и Нечепорука. При этом Петров также независимо от Нечепорука переносит товары. При этом Иванов влияет на Петрова (количество товаров в его куче), а Петров на Нечепорука. (принёс новый товар в машину)
- Описаны входные данные программы, включающие вариативные диапазоны, возможные при многократных запусках.
На вход подаётся число, означающее, сколько товаров украдут трое за вылазку. Учитывается, что может быть введено не int число, отрицательное число или 0 (такое мы не принимаем, и сообщаем о некорректных данных с корректным преждевременным завершением программы через return 0)
- Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта изученных синхропримитивов.
Используются семафоры, так как они позволяют разным потокам изменять их состояние, а это необходимо в нашем задании.
- Ввод данных в приложение реализовывался с консоли во время выполнения программы (без использования аргументов командной строки).
- Для используемых генераторов случайных чисел описаны их диапазоны и то, как интерпретируются данные этих генераторов.
1. Для генерации стоимости каждого украденного имущества применяется std::uniform_int_distribution<int> costRange(200, 1500000) с соответствующим диапозоном (200-1500000);
2. Для генерации количества секунд, которое нужно Иванову для выноса конкретного имущества используется std::uniform_int_distribution<int> timeToTake(3, 10) с диапозоном 3-10 секунд;
3. Для генерации количества секунд, которое нужно Петрову для переноса конкретного имущества используется std::uniform_int_distribution<int> timeToCarry(2, 15) с диапозоном 2-15 секунд;
4. Для генерации количества секунд, которое нужно Нечепоруку для добавления стоимости конкретного принесённого имущества к итоговому используется std::uniform_int_distribution<int> timeToCount(1, 2) с диапозоном 1-2 секунды;
- Вывод программы информативен, отражая все ключевые протекающие в ней события в терминах предметной области.
- Наблюдатель на основе вывода программы понимает, что в ней происходит в каждый момент времени ее работы.
- В программе присутствуют комментарии, поясняющие выполняемые действия и описание используемых объектов и переменных.
- Результаты работы программы представлены в отчете ниже.

### Таблица прогона тестов
<table>
    <tr>
        <th>№ теста</th>
        <th>Какую ситуацию проверяет</th>
        <th>Что произошло</th>
        <th>Корректно ли</th>
    </tr>
    <tr>
        <td>1</td>
        <td>Введено не число</td>
        <td>Сообщение о том, что число должно быть положительным</td>
        <td>Да, так как мы работаем только с числами</td>
    </tr>
    <tr>
        <td>2</td>
        <td>Введено отрицательное число</td>
        <td>Сообщение о том, что число должно быть положительным</td>
        <td>Да, так как мы работаем только с положительными числами</td>
    </tr>
    <tr>
        <td>3</td>
        <td>Введён 0</td>
        <td>Сообщение о том, что число должно быть положительным</td>
        <td>Да, так как мы работаем только с положительными числами</td>
    </tr>
    <tr>
        <td>4</td>
        <td>Введено положительное число</td>
        <td>Программа выполнилась успешно и провела "кражу" имущества, по количеству равному введённому числу</td>
        <td>Да, так как мы ввели корректное значение</td>
    </tr>
    <tr>
        <td>5</td>
        <td>Некорректное число аргументов</td>
        <td>Сообщение о том, что введено неправильное количество аргументов</td>
        <td>Да, так как мы ожидаем получить 3 параметра, не считая саму функцию в командной строке </td>
    </tr>
    <tr>
        <td>6</td>
        <td>Некорректный ключ</td>
        <td>Сообщение о том, что ключ некорректный</td>
        <td>Да, так как мы ожидаем получить либо 0, либо 1</td>
    </tr>
    <tr>
        <td>7</td>
        <td>Некорректный файл для чтения</td>
        <td>Сообщение о том, что файла не существует или он не открылся</td>
        <td>Да, так как мы ожидаем, что файл для чтения корректен</td>
    </tr>
</table>

![](https://i.pinimg.com/564x/e2/c7/c4/e2c7c4a6fc90f3b8ee1c1d7e41c7fc02.jpg)
