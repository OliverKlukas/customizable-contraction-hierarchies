#ifndef OPTIMIZEDKIT_MATH_HPP
#define OPTIMIZEDKIT_MATH_HPP

 namespace OptimizedKit{
    /**
     * @brief Checks if the addition of two numbers will overflow and saves the result in the result parameter. By default
     *        the result parameter is set to the maximum value of the type. No negative numbers are allowed.
     *
     * @details IMPORTANT: this does not work for every type, check docs and tests for more information.
     *
     * @tparam T - The type of the numbers.
     * @param result - The result of the addition.
     * @param a - The first number.
     * @param b - The second number.
     * @return Returns true if the addition did overflow, false otherwise.
     */
    template<typename T>
    bool positiveOverflowAddition(T* result, T a, T b)
    {
        if(a < 0 || b < 0 || a == std::numeric_limits<T>::max() || b == std::numeric_limits<T>::max() || a > std::numeric_limits<T>::max() - b){
            *result = std::numeric_limits<T>::max();
            return true;
        }
        *result = a + b;
        return false;
    }

     /**
      * @brief Checks if the addition of three numbers will overflow and saves the result in the result parameter. By
      *        default the result parameter is set to the maximum value of the type. No negative numbers are allowed.
      *
      * @details IMPORTANT: this does not work for every type, check docs and tests for more information.
      *
      * @tparam T - The type of the numbers.
      * @param result - The result of the addition.
      * @param a - The first number.
      * @param b - The second number.
      * @param c - The third number.
      * @return Returns true if the addition did overflow, false otherwise.
      */
     template<typename T>
     bool positiveOverflowAddition(T* result, T a, T b, T c)
     {
         T temp;
        bool overflow = positiveOverflowAddition(&temp, a, b);
        overflow = positiveOverflowAddition(result, temp, c) || overflow;
        return overflow;
     }

     /**
      * @brief Updates the existing value with the update value if the update value is smaller than the existing value.
      *
      * @tparam T - The type of the numbers.
      * @param existingValue - The existing value.
      * @param updateValue - The update value.
      */
     template<typename T>
     void updateIfSmaller(T &existingValue, const T &updateValue){
         if(updateValue < existingValue)
             existingValue = updateValue;
     }
 }

#endif //OPTIMIZEDKIT_MATH_HPP
