#pragma once

/// \brief Базовая реализация метафункции проверки на равенство всех типов параметров
template<typename ...Args>
struct all_types_are_same;

/// \brief Частичная реализация метафункции проверки на равенство всех типов параметров
/// \details Если у нас только один параметр то он равен сам себе
template<typename T>
struct all_types_are_same<T> {
    static constexpr bool value = true;
};

/// \brief Частичная реализация метафункции проверки на равенство всех типов параметров
/// \details Если первые два параметра разные - значит условие не выполнено.
template<typename T, typename U, typename ...Args>
struct all_types_are_same<T, U, Args...> {
    static constexpr bool value = false;
};
/// \brief Частичная реализация метафункции проверки на равенство всех типов параметров
/// \details Если первые два параметра одинаковы - отбросим один из них и проверим снова.
template<typename T, typename ...Args>
struct all_types_are_same<T, T, Args...> {
    static constexpr bool value = all_types_are_same<T, Args...>::value;
};

