#include <cilk/cilk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THRESHOLD 50

long *cilk_sort(long *array, long size, unsigned int num_threads);
void p_merge_sort(long *outArray, long *array, int size);
void p_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize);
void s_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize);
int do_binary_search(long key, long *array, int size);
int binary_search(long *array, int left, int right, long key);
void s_merge_sort(long *outArray, long *array, int size);

long *cilk_sort(long *array, long size, unsigned int num_threads)
{
  // NOTE: It is not required that you use the variable num_threads in
  //       your solution.
  long *out_array = (long *)malloc(sizeof(long) * size);
  if(num_threads ==0){
	s_merge_sort(out_array,array,size);
  }else{
  	p_merge_sort(out_array, array, size);
  }
  return out_array;
}

void p_merge_sort(long *outArray, long *array, int size)
{
  if (size == 1)
  {
    outArray[0] = array[0];
  }
  else
  {
    long *temp = (long *)malloc(size * sizeof(long));
    cilk_spawn p_merge_sort(temp, array, size / 2);
    p_merge_sort(temp + size / 2, array + size / 2, size - size / 2);
    cilk_sync;

    p_merge(outArray, temp, temp + size / 2, size / 2, size - size / 2);
    free(temp);
  }
}

void p_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize)
{
  if (lSize < rSize)
  {
    p_merge(out, inRight, inLeft, rSize, lSize);
  }
  else if (lSize < THRESHOLD)
  {
    s_merge(out, inLeft, inRight, lSize, rSize);
  }
  else
  {
    int newLS = lSize / 2;
    int newRS = do_binary_search(inLeft[newLS], inRight, rSize);
    out[newLS + newRS] = inLeft[newLS];
    cilk_spawn p_merge(out, inLeft, inRight, newLS, newRS);
    p_merge(out + newLS + newRS + 1, inLeft + newLS + 1, inRight + newRS, lSize - newLS - 1, rSize - newRS);
    cilk_sync;
  }
}

int do_binary_search(long key, long *array, int size)
{
  return binary_search(array, 0, size - 1, key);
}

int binary_search(long *array, int left, int right, long key)
{
  int mid = left + (right - left) / 2;
  if (right >= left)
  {
    if (array[mid] == key) // key is found
    {
      return mid;
    }
    if (array[mid] > key) // search in the left subarray
    {
      return binary_search(array, left, mid - 1, key);
    }
    // Search in the right subarray
    return binary_search(array, mid + 1, right, key);
  }
  return mid; // key is not in the list
}

void s_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize)
{
  while (lSize > 0 && rSize > 0)
  {
    if (*inLeft <= *inRight)
    {
      *out++ = *inLeft++;
      lSize--;
    }
    else
    {
      *out++ = *inRight++;
      rSize--;
    }
  }
  while (lSize > 0)
  {
    *out++ = *inLeft++;
    lSize--;
  }
  while (rSize > 0)
  {
    *out++ = *inRight++;
    rSize--;
  }
  //CHANGED
  inLeft = out;
}

void s_merge_sort(long *outArray, long *array, int size)
{
  if (size == 1)
  {
    outArray[0] = array[0];
  }
  else
  {
    long *temp = (long *)malloc(size * sizeof(long));
    s_merge_sort(temp, array, size / 2);
    s_merge_sort(temp + size / 2, array + size / 2, size - size / 2);
    

    s_merge(outArray, temp, temp + size / 2, size / 2, size - size / 2);
    free(temp);
  }
}
