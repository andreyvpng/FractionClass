FractionClass
=============

Простая реализация класса для работы с обыкновенными дробями, написанная в учебных целях.

Возможности
1. Сложение, вычитание, умножение, деление (с перезагруженными операторами)
2. Приведение к несократимой дроби
3. Сравнение дробей(== != < > <= >=)
4. Нахождение НОД числителя и знаменателя

Пример работы:
```cpp
#include <iostream>
#include "fraction.h"

using namespace std;

int main ()
{
    cout << fraction(1, 2) + fraction(1, 3) << "\n";
    cout << fraction(1, 2) - fraction(1, 3) << "\n";
    cout << fraction(1, 2) * fraction(1, 3) << "\n";
    cout << fraction(1, 2) / fraction(1, 3) << "\n";
    return 0;
}

```
