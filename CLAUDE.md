# C++ CODING CONVENTION & STYLE GUIDE

## 0. КРИТИЧЕСКИЕ ПРАВИЛА (STRICT OVERRIDES)
Эти правила имеют наивысший приоритет над остальными пунктами.

1.  **Язык:**
    * Идентификаторы (переменные, классы, функции): **Строго Английский**.
    * Тексты ошибок и исключений: **Строго Русский**.
2.  **Комментарии:**
    * **Никогда не писать комментарии в коде.** Код должен быть самодокументируемым.
3.  **Структура кода:**
    * **Атомарность:** Функции должны быть максимально дробными. Одна функция — одно действие.
    * **Разделение:** Все классы строго разделяются на интерфейс (`.h`) и реализацию (`.cpp`).
    * **Скрытая реализация:** Все вспомогательные методы, необходимые для работы внутри класса, **выносить** в `namespace {}` (анонимный) или именованный namespace внутри файла реализации (`.cpp`). Не оставлять приватных вспомогательных методов в хедере, если они не требуют доступа к `this` (или передавать контекст явно).
4.  **Обработка ошибок:**
    * Все проверки с выбрасыванием исключений выносить в атомарные методы вида `AssertIs(ИмяПроверки)`.
    * Текст ошибок внутри `AssertIs` писать на **русском языке**.
5.  **Стандарт:**
    * Использовать **C++20**.
    * Активно использовать `std::filesystem`, `std::unique_ptr`, `std::shared_ptr`.
    * Использовать RAII обертки для жизненно важных ресурсов и чувствительных данных.

---

## 1. ИМЕНОВАНИЕ (NAMING CONVENTIONS)

| Категория | Стиль / Префикс | Пример |
| :--- | :--- | :--- |
| **Класс** | PascalCase | `PublishDialog` |
| **Интерфейс** | Prefix `I` + PascalCase | `IConverterPipeline` |
| **Типы (struct, using, enum)** | PascalCase | `LicenseState` |
| **Функция / Метод** | PascalCase | `ExtractZip`, `ShouldQuit` |
| **Поле класса (member)** | Prefix `m_` + camelCase | `m_taskQueue`, `m_children` |
| **Локальная переменная** | camelCase | `bookCount`, `env` |
| **Параметры функции** | camelCase | `bookCount`, `env` |
| **Enum class value** | PascalCase | `BusinessTrial` |
| **Template parameter** | PascalCase | `Callback`, `ArraySize` |
| **Макросы** | SCREAMING_SNAKE_CASE | `ISPRING_BRAND_NAME` |
| **Global variable** | Prefix `g_` (Avoid usage) | `g_cachedTypes` |

* **Важно:** Никогда не использовать префикс `C` для классов реализации (например, `CMyClass` — запрещено).
* **NL.5:** Не кодировать типы данных в именах (венгерская нотация запрещена, кроме `m_` и `I`).
* **NL.7:** Длина имени пропорциональна области видимости.

---

## 2. ФОРМАТИРОВАНИЕ (FORMATTING)

Использовать `.clang-format` (расширение для VS).
* **Отступы:** Табуляция (Tabs), размер 4.
* **Длина строки:** Максимум 100 символов.
* **Скобки (Braces):** Стиль Allman (открывающая скобка всегда на новой строке), кроме лямбд и списков инициализации.
* **Пробелы:** Умеренно (NL.15).
* **Указатели/Ссылки:** Прижимаются к типу: `T& operator[](size_t);`.
* **Потоковый вывод:** Для завершения строк и переноса на новую строку в потоках (например, std::cout) всегда использовать std::endl (кроме случаев, когда важно не использовать std::flush).

**Пример оформления:**
```cpp
void AddWords(std::string& text, const std::vector<std::string>& words) override
{ // Скобка на новой строке
    auto addOneWord = [&text](const auto& word) { // Лямбда-исключение
        if (!text.empty())
        {
            text += " ";
        }
        text += word;
    };
    
    for (const auto& word : words)
    {
        addOneWord(word);
    }
}

```

---

## 3. ПРОЕКТИРОВАНИЕ И ФУНКЦИИ

* **F.2:** Функция выполняет одну логическую подзадачу.
* **F.9:** Неиспользуемые параметры не должны иметь имени (можно `(void)var;` или `/*var*/`).
* **F.20:** Возвращать значения через `return`, а не через out-параметры.
* **F.21:** Если возвращаемых значений много — возвращать структуру/tuple.
* **Умные указатели:**
* `std::unique_ptr` — эксклюзивное владение (по умолчанию).
* `std::shared_ptr` — совместный доступ.
* `std::weak_ptr` — разрыв циклов/наблюдение.
* `nullptr` вместо `0` или `NULL`.


* **Виртуальные функции:** Явно указывать `virtual`, `override` или `final`.
* **noexcept:** Помечать функции, которые не бросают исключений.
* **Лямбды:** Не использовать захват по умолчанию `[=]`, если захватывается `this` или поля класса (F.54).

---

## 4. АРХИТЕКТУРА И БЕЗОПАСНОСТЬ

**Разделение уровней:**

1. **Высокий уровень:** Бизнес-логика.
2. **Низкий уровень:** Работа с памятью, сырые циклы, API ОС.
* Низкоуровневый код (открытие файлов, сложные циклы) изолировать в отдельные функции/классы.
* Смешивание C и C++: Изолировать C API.



**Безопасность:**

* Проверять контракты (входные параметры).
* Защита от `nullptr`, `dangling pointer`, деления на ноль.

---

## 5. ПРИМЕРЫ РЕАЛИЗАЦИИ (ШАБЛОНЫ)

### Пример 1: Хедер (.h)

```cpp
#pragma once

class MyClassName
{ 
public: 
    MyClassName();
    ~MyClassName();

    void DoSomething(int parameterName, int someAnotherMethodName);

protected:
    int SomeProtectedMethod(int someParameter) const;

private:
    int m_memberVariable;
};

```

### Пример 2: Реализация (.cpp) с атомарными проверками

```cpp
#include "stdafx.h"
#include "MyClassName.h"
#include <stdexcept>

// Вспомогательные методы в анонимном неймспейсе (скрытая реализация)
namespace
{
    void AssertIsPositive(int value)
    {
        if (value <= 0)
        {
            // Текст ошибки строго на русском
            throw std::runtime_error("Значение должно быть положительным");
        }
    }

    int CalculateOffset(int base, int multiplier)
    {
        return base * multiplier;
    }
}

MyClassName::MyClassName()
    : m_memberVariable(10)
{
}

MyClassName::~MyClassName()
{
}

void MyClassName::DoSomething(int parameterName, int someAnotherMethodName)
{
    // Использование атомарной проверки
    AssertIsPositive(parameterName);

    // Логика разбита на части
    int offset = CalculateOffset(someAnotherMethodName, 2);
    m_memberVariable += parameterName + offset;
}

int MyClassName::SomeProtectedMethod(int someParameter) const
{
    return m_memberVariable + someParameter;
}
```

## 6. ПРАВИЛА ОФОРМЛЕНИЯ ТЕСТОВ И БЕНЧМАРКОВ (GoogleTest)

Тесты должны быть лаконичными, атомарными и легко читаемыми.

1.  **Именование Test Suite:** Имя набора тестов должно строго совпадать с именем тестируемого класса + суффикс `Test` (например, `TimerTest`, `RectangleTest`).
2.  **Именование Теста:** Имя самого теста должно быть **коротким** и отражать суть. Избегайте длинных предложений.
    * *Плохо:* `ConstructorInitWithCoordinates`
    * *Хорошо:* `InitCorrectly` или `CorrectInit`
3.  **Исключение из правила "Без комментариев":** Строго разрешен **только один** однострочный комментарий непосредственно над макросом `TEST(Suite, Name)`.
    * Комментарий должен кратко объяснять, что проверяет тест, и начинаться с `// Проверка ...`.
4.  **Структура теста:** Используйте негласное правило AAA (Arrange, Act, Assert). Инициализация, действие, проверка. Не мешайте всё в кучу.
5.  **Использование namespace:** Для тестов всегда используйте `using namespace testing;` после инклюдов, чтобы не засорять код префиксами `testing::`.

**Пример идеального теста:**
```cpp
#include "src/timer/Timer.h"
#include <gtest/gtest.h>

using namespace testing;

// Проверка корректности подсчета прошедшего времени
TEST(TimerTest, ElapsedTimeIsPositive)
{
    Timer timer;
    EXPECT_GE(timer.Elapsed(), 0.0);
}
```

## 7. СТРУКТУРА ПРОЕКТА
Можно не думать о системе сборки или подключении библиотек. В проекте настроен автоматический CMakeLists.txt (с `GLOB_RECURSE`), который сам подхватывает все файлы, тесты и бенчмарки.

**Структура папок следующая:**
* `/src/` — исходный код бизнес-логики. Каждый класс лежит в своей подпапке (например, `/src/timer/Timer.h`).
* `/tests/` — тесты GoogleTest (например, `/tests/timer.test.cpp`).
* `/benchmarks/` — бенчмарки Google Benchmark (например, `/benchmarks/timer.benchmark.cpp`).