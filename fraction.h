/*
    The MIT License

    Copyright 2017 Andrey Varfolomeev <varf.andrey@hotmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
    to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _FRACTION_
#define _FRACTION_

class fraction
{
private:
    int a;          // Числитель
    int b;          // Знаменатель
    bool positive;  // Положительна ли дробь?

    /*
     *  Если надо меняет отрицательные числа на положительные
     *  Обновляет переменную positive
     *  Приводит дробь к несократимой
     */
    void balance ()
    {
        if(this->b == 0)
        {
            throw "ZeroDivisionError!";
        }
        if(this->a < 0)
        {
            this->a = -(this->a);
            this->positive = !(this->positive);
        }

        if(this->b < 0)
        {
            this->b = -(this->b);
            this->positive = !(this->positive);
        }

        int gcd_ = gcd__(this->a, this->b);

        this->a /= gcd_;
        this->b /= gcd_;
    }
    /*
     *  Бинарный алгоритм вычисления НОД
     *  https://ru.wikipedia.org/wiki/Бинарный_алгоритм_вычисления_НОД
     */
    int gcd__ (int a, int b)
    {
        if(a == 0)
            return b;
        if(b == 0)
            return a;
        if(a == b)
            return a;
        if(a == 1 || b == 1)
            return 1;
        if((a & 1) == 0)
            return ((b & 1) == 0)
                ? gcd__(a >> 1, b >> 1) << 1
                : gcd__(a >> 1, b);
        else
            return ((b & 1) == 0)
                ? gcd__(a, b >> 1)
                : gcd__(b, a > b ? a - b : b - a);
    }
public:
    /*
     *  Конструктор
     */
    fraction (int a_ = 1, int b_ = 1, bool positive_ = true)
    {
        a = a_;
        b = b_;
        positive = positive_;
        this->balance();
    }
    /*
     *  Деструктор
     */
    ~fraction (void) {}
    /*
     * Возвращает НОД числителя и знаменателя
     */
    int gcd ()
    {
        return gcd__(this->a, this->b);
    }
    /*
     *  Возвращает числитель
     */
    int get_numerator ()
    {
        return this->a;
    }
    /*
     *  Возвращает знаменатель
     */
    int get_denominator ()
    {
        return this->b;
    }
    /*
     *  Возвращает положительна ли дробь
     */
    bool is_positive ()
    {
        return this->positive;
    }
    /*
     *  Переводит Fraction в double
     */
    double toDouble()
    {
        return (double)this->a / (double) this->b;
    }
    /*
     *  Перегрузка ввода/вывода
     *
     *  Выполняется только в том случае, если в основном файле
     *  подключена библиотека <iostream>
     *
     *  _GLIBCXX_IOSTREAM в MinGW
     *  _IOSTREAM_        в MSCV
     */
    #if defined(_GLIBCXX_IOSTREAM) || defined(_IOSTREAM_)
        friend std::ostream& operator << (std::ostream &out, const fraction& my_fraction)
        {
            out << "Fraction(";
            if(!my_fraction.positive)
                out << "-";
            out << my_fraction.a << ", " << my_fraction.b << ")";
            return out;
        }
        friend std::istream& operator >> (std::istream &in, fraction& my_fraction)
        {
            std::cin >> my_fraction.a >> my_fraction.b;
            my_fraction.balance();
            return in;
        }
    #endif
    /*
     *  Присваевание
     */
    fraction operator = (const fraction& my_fraction)
    {
        if(&my_fraction == this)
            return *this;

        this->a = my_fraction.a;
        b = my_fraction.b;
        positive = my_fraction.positive;
        return *this;
    }
    /*
     *  Вспомогательная функция для сложения и вычитания
     *  Вычисляем значение числителя с учетом знаков дробей
     */
    int getNumerator_after_Add_Or_Sub (const fraction& first_fraction,
                                       const fraction& second_fraction,
                                       bool is_addition)
    {
        int a_new = 0;
        if(first_fraction.positive)
            a_new += first_fraction.a * second_fraction.b;
        else
            a_new -= first_fraction.a * second_fraction. b;
        if(second_fraction.positive && is_addition)
            a_new += first_fraction.b * second_fraction.a;
        else
            a_new -= first_fraction.b * second_fraction.a;

        return a_new;
    }
    /*
     *  Оператор сложения
     */
    fraction operator + (const fraction& my_fraction)
    {
        int a_new = getNumerator_after_Add_Or_Sub(*this, my_fraction, true);

        fraction tmp(a_new, b * my_fraction.b);
        tmp.balance();
        return tmp;
    }
    fraction operator + (const int& x)
    {
        return *this + fraction(x);
    }
    fraction& operator += (const fraction& my_fraction)
    {
        *this = *this + my_fraction;

        return *this;
    }
    /*
     *  Оператор вычетания
     */
    fraction operator - (const fraction& my_fraction)
    {
        int a_new = getNumerator_after_Add_Or_Sub(*this, my_fraction, false);

        fraction tmp(a_new, b * my_fraction.b);
        tmp.balance();
        return tmp;
    }
    fraction& operator -= (const fraction& my_fraction)
    {
        *this = *this - my_fraction;

        return *this;
    }
    /*
     *  Оператор умножения
     */
    fraction operator * (const fraction& my_fraction)
    {
        int a_new = 0;

        if(positive)
            a_new = a * my_fraction.a;
        else
            a_new = -a * my_fraction.a;
        if(!my_fraction.positive)
            a_new = -a_new;

        fraction tmp(a_new, b * my_fraction.b);
        tmp.balance();
        return tmp;
    }
    fraction& operator *= (const fraction& my_fraction)
    {
        *this = *this * my_fraction;

        return *this;
    }
    /*
     *  Оператор деления
     */
    fraction operator / (const fraction& my_fraction)
    {
        int a_new = 0;

        if(positive)
            a_new = a * my_fraction.b;
        else
            a_new = -a * my_fraction.b;
        if(!my_fraction.positive)
            a_new = -a_new;

        fraction tmp(a_new, b * my_fraction.a);
        tmp.balance();
        return tmp;
    }
    fraction& operator /= (const fraction& my_fraction)
    {
        *this = *this / my_fraction;

        return *this;
    }
    /*
     *  Унарные операторы
     */
    fraction operator ++ ()
    {
        *this += fraction(1, 1);
        this->balance();

        return *this;
    }
    fraction operator ++ (int)
    {
        *this += fraction(1, 1);
        this->balance();

        return *this - fraction(1, 1);
    }
    fraction operator -- ()
    {
        *this -= fraction(1, 1);
        this->balance();

        return *this;
    }
    fraction operator -- (int)
    {
        *this -= fraction(1, 1);
        this->balance();

        return *this + fraction(1, 1);
    }
    fraction operator - ()
    {
        this->positive = !(this->positive);

        return *this;
    }
    /*
     *  Операторы сравнения
     */
    bool operator == (const fraction& my_fraction)
    {
        return (a == my_fraction.a && b == my_fraction.b);
    }
    bool operator != (const fraction& my_fraction)
    {
        return !(*this == my_fraction);
    }
    bool operator < (const fraction& my_fraction)
    {
        if(this->b == my_fraction.b)
            return (this->a < my_fraction.a);

        return (this->a * my_fraction.b < my_fraction.a * this->b);
    }

    bool operator > (const fraction& my_fraction)
    {
        if(this->b == my_fraction.b)
            return (this->a > my_fraction.a);

        return (this->a * my_fraction.b > my_fraction.a * this->b);
    }
    bool operator <= (const fraction& my_fraction)
    {
        return !(*this > my_fraction);
    }
    bool operator >= (const fraction& my_fraction)
    {
        return !(*this < my_fraction);
    }
};
#endif /* _FRACTION_ */
