#pragma once

#include <array>

namespace math
{

/**
 * @brief Swaps two (non-equal) values of type T.
 * @tparam T. Type of the values.
 * @param[in] first. Pointer to the first value.
 * @param[in] second. Pointer to the second value.
 */
template <typename T> void swap(T* first, T* second)
{
    if (first != second)
    {
        auto val = *second;
        *second = *first;
        *first = val;
    }
}

/**
 * @brief Partitions the given range of the given array around a pivot such that all values smaller than the pivot are to its left and all values greater than the pivot are to its right.
 * @tparam T. Type of the values in the array.
 * @tparam S. (Optional) Type of the values in the (optional) side array.
 * @param[in] arr. The array to partition.
 * @param[in] begin. First index of the range to partition.
 * @param[in] end. Last index of the range to partition (inclusive).
 * @param[in] side_arr. (Optional) Side array in which to swap values based on the partitioning of the first array.
 * @return The index of the pivot in the partitioned array.
 */
template <typename T, typename S = T> T partition(T arr[], int begin, int end, S side_arr[] = nullptr)
{
    // Select the last element as the pivot
    auto pivot_index = end;
    auto const pivot = arr[pivot_index];
    // Swap the leftmost values in the array with values in the array smaller than the pivot
    auto leftmost_index_not_yet_swapped = begin;
    for (auto it = begin; it < end; it++)
    {
        if (arr[it] <= pivot)
        {
            swap(&arr[leftmost_index_not_yet_swapped], &arr[it]);
            // If there is a side array to swap based on the sorting of the first array, do so
            if (side_arr != nullptr)
                swap(&side_arr[leftmost_index_not_yet_swapped], &side_arr[it]);
            // Update the leftmost index iterator
            leftmost_index_not_yet_swapped++;
        }
    }
    // Place the pivot at the leftmost index that has not yet been swapped
    swap(&arr[leftmost_index_not_yet_swapped], &arr[pivot_index]);
    if (side_arr != nullptr)
        swap(&side_arr[leftmost_index_not_yet_swapped], &side_arr[pivot_index]);
    // Return the new index of the pivot
    pivot_index = leftmost_index_not_yet_swapped;
    return pivot_index;
}

/**
 * @brief Applies quick sort to the given array of values.
 * @tparam T. Type of values in the array.
 * @tparam S. (Optional) Type of values in an (optional) side array to sort based on the sorting of the first array.
 * @param[in] arr. Array of values to sort.
 * @param[in] begin. Index at which to begin the sort.
 * @param[in] end. Index at which to end the sort (inclusive).
 * @param[in] side_arr. (Optional) Side array to sort based on the sorting of the first array.
 */
template <typename T, typename S = T> void quick_sort(T arr[], int begin, int end, S side_arr[] = nullptr)
{
    if (begin < end)
    {
        // Partition the array so that values smaller than the pivot are before it, and values larger than the pivot are after it
        auto pivot_index = partition(arr, begin, end, side_arr);
        // Apply this method recursively to the left and right sub-arrays relative to the pivot
        quick_sort(arr, begin, pivot_index - 1, side_arr);
        quick_sort(arr, pivot_index + 1, end, side_arr);
    }
}

/**
 * @brief Merges the two subarrays, defined as the elements in the array respectively from begin to middle (inclusive) and from middle (exclusive) to end, to obtain a sorted output.
 * @tparam T. Type of the values in the array.
 * @param[in] merge_arr. Array containing the two subarrays.
 * @param[in] begin. First index of the first subarray.
 * @param[in] middle. Last index of the first subarray (inclusive), and index before the first index of the second subarray.
 * @param[in] end. Last index of the second subarray (inclusive).
 */
template <typename T> void merge_subarrays(T merge_arr[], int begin, int middle, int end)
{
    // Copy the data into left and right sub-arrays
    int arr_one_size = middle - begin + 1;
    int arr_two_size = end - middle;
    auto index_merge = begin;
    auto index_one = 0;
    T* arr_one = new T[arr_one_size];
    while (index_one < arr_one_size)
        arr_one[index_one++] = merge_arr[index_merge++];
    auto index_two = 0;
    T* arr_two = new T[arr_two_size];
    while (index_two < arr_two_size)
        arr_two[index_two++] = merge_arr[index_merge++];
    // Iterate over each sorted array to add elements to the merged array
    index_merge = begin;
    index_one = 0;
    index_two = 0;
    while (index_one < arr_one_size && index_two < arr_two_size)
    {
        // If the first array has a smaller element, add this element
        if (arr_one[index_one] < arr_two[index_two])
        {
            merge_arr[index_merge++] = arr_one[index_one++];
        }
        // Otherwise, add the element of the second array
        else
        {
            merge_arr[index_merge++] = arr_two[index_two++];
        }
    }
    // Complete the merged array with the remaining elements of the largest sorted array (only one of those loops actually executes)
    while (index_one < arr_one_size)
    {
        merge_arr[index_merge++] = arr_one[index_one++];
    }
    while (index_two < arr_two_size)
    {
        merge_arr[index_merge++] = arr_two[index_two++];
    }
    // Delete the temporary sub-arrays
    delete[] arr_one;
    delete[] arr_two;
}

/**
 * @brief Applies merge sort to the given array of values.
 * @tparam T. Type of values in the array.
 * @param[in] arr. Array of values to sort.
 * @param[in] begin. Index at which to begin the sort.
 * @param[in] end. Index at which to end the sort (inclusive).
 */
template <typename T> void merge_sort(T arr[], int begin, int end)
{
    if (begin < end)
    {
        // Get the middle index
        auto middle = (end + begin) / 2;
        // Apply this method recursively to the left and right sub-arrays
        merge_sort(arr, begin, middle);
        merge_sort(arr, middle + 1, end);
        // Merge the left and right sub-arrays
        merge_subarrays(arr, begin, middle, end);
    }
}

} // namespace math
