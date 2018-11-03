#pragma once
template<typename ...Args>
struct all_types_are_same;
template<typename T>
struct all_types_are_same<T> {
    static constexpr bool value = true;
};
template<typename T, typename U, typename ...Args>
struct all_types_are_same<T, U, Args...> {
    static constexpr bool value = false;
};
template<typename T, typename ...Args>
struct all_types_are_same<T, T, Args...> {
    static constexpr bool value = all_types_are_same<T, Args...>::value;
};

