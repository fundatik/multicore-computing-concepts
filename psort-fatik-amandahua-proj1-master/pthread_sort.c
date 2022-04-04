#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define THRESHOLD 50
unsigned int MAX_THREADS = 1;

// thread arguments for sorting
typedef struct _thr_args_
{
    int size;
    long *array;
    long *out_array;
    int level;
    int need_exit;
} thr_args_t;

// thread arguments for merging
typedef struct _thr_args_merge_
{
    int l_size;
    int r_size;
    long *in_left;
    long *in_right;
    long *out;
    int level;
    int need_exit;
} thr_args_merge_t;

long *pthread_sort(long *array, long size, unsigned int num_threads);

int do_cmp_func(const void *a, const void *b);
void s_sort(long *out, long *array, int size);

void sp_merge_sort(long *outArray, long *array, int size);
void s_pthr_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize);

void *p_pthr_sort(void *arg);
void *p_pthr_merge(void *arg);

int pthr_binary_search(long *array, int left, int right, long key);
int pthr_do_binary_search(long key, long *array, int size);

long *pthread_sort(long *array, long size, unsigned int num_threads)
{
    long *out = (long *)malloc(size * sizeof(long));
    MAX_THREADS = num_threads;

    if (num_threads == 0)
    {
        sp_merge_sort(out, array, size);
    }
    else
    {
        thr_args_t thr_args;
        thr_args.out_array = out;
        thr_args.array = array;
        thr_args.size = size;
        thr_args.level = 1;
        thr_args.need_exit = 1;
        //thr_args.need_exit = 0;
        //p_pthr_sort(&thr_args);

        pthread_t tid;
        int flag = 1;
        int rc = pthread_create(&tid, NULL, p_pthr_sort, &thr_args);
        if (rc)
        {
            sp_merge_sort(out, array, size);
            flag--;
        }

        if (flag)
        {
            pthread_join(tid, NULL);
        }
    }
    return out;
}

// thread function for parallel sort
void *p_pthr_sort(void *arg)
{
    thr_args_t *data = (thr_args_t *)arg;

    if (pow(2, data->level - 1) >= MAX_THREADS || data->size < THRESHOLD)
    {
        sp_merge_sort(data->out_array, data->array, data->size);
    }
    else
    {
        long *temp = (long *)malloc(data->size * sizeof(long));

        thr_args_t thr_args1;
        thr_args1.out_array = temp;
        thr_args1.array = data->array;
        thr_args1.size = data->size / 2;
        thr_args1.level = data->level + 1;
        thr_args1.need_exit = 1;

        pthread_t tid1;
        int flag1 = 1;
        int rc = pthread_create(&tid1, NULL, p_pthr_sort, &thr_args1);
        if (rc)
        {
            sp_merge_sort(temp, data->array, data->size / 2);
            //printf(stderr, "err: pthread_create, rc: %d\n", rc);
            flag1--;
        }

        thr_args_t thr_args2;
        thr_args2.out_array = temp + data->size / 2;
        thr_args2.array = data->array + data->size / 2;
        thr_args2.size = data->size - data->size / 2;
        thr_args2.level = data->level + 1;
        thr_args2.need_exit = 0;

        p_pthr_sort(&thr_args2);

        if (flag1)
        {
            pthread_join(tid1, NULL);
        }

        thr_args_merge_t thr_args;
        thr_args.out = data->out_array;
        thr_args.in_left = temp;
        thr_args.in_right = temp + data->size / 2;
        thr_args.l_size = data->size / 2;
        thr_args.r_size = data->size - data->size / 2;
        thr_args.level = data->level;
        thr_args.need_exit = 0;

        p_pthr_merge(&thr_args);

        free(temp);
    }

    if (data->need_exit)
    {
        pthread_exit(NULL);
    }
    else
    {
        return NULL;
    }
}

int do_cmp_func(const void *a, const void *b)
{
    return (*(long *)a - *(long *)b);
}

void s_sort(long *out, long *array, int size)
{
    qsort(array, size, sizeof(long), do_cmp_func);
    int i;
    for (i = 0; i < size; i++)
    {
        *out++ = *array++;
    }
}

void sp_merge_sort(long *outArray, long *array, int size)
{
    if (size == 1)
    {
        outArray[0] = array[0];
    }
    else
    {
        long *temp = (long *)malloc(size * sizeof(long));
        sp_merge_sort(temp, array, size / 2);
        sp_merge_sort(temp + size / 2, array + size / 2, size - size / 2);

        s_pthr_merge(outArray, temp, temp + size / 2, size / 2, size - size / 2);
        free(temp);
    }
}
void s_pthr_merge(long *out, long *inLeft, long *inRight, int lSize, int rSize)
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
    inLeft = out;
}

// thread function for parallel merge
void *p_pthr_merge(void *arg)
{
    thr_args_merge_t *data = (thr_args_merge_t *)arg;

    if (pow(2, data->level - 1) >= MAX_THREADS || data->l_size < THRESHOLD)
    {
        s_pthr_merge(data->out, data->in_left, data->in_right, data->l_size, data->r_size);
    }
    else
    {
        int l_size = data->l_size;
        int r_size = data->r_size;
        long *in_left = data->in_left;
        long *in_right = data->in_right;
        long *out = data->out;

        if (l_size < r_size)
        {
            thr_args_merge_t new_args;
            new_args.out = out;
            new_args.in_left = in_right;
            new_args.in_right = in_left;
            new_args.l_size = r_size;
            new_args.r_size = l_size;
            new_args.level = data->level;
            new_args.need_exit = data->need_exit;

            p_pthr_merge(&new_args);
        }
        else if (l_size < THRESHOLD)
        {
            s_pthr_merge(out, in_left, in_right, l_size, r_size);
        }
        else
        {
            int newLS = l_size / 2;
            int newRS = pthr_do_binary_search(in_left[newLS], in_right, r_size);
            out[newLS + newRS] = in_left[newLS];

            thr_args_merge_t thr_args1;
            thr_args1.out = out;
            thr_args1.in_left = in_left;
            thr_args1.in_right = in_right;
            thr_args1.l_size = l_size;
            thr_args1.r_size = r_size;
            thr_args1.level = data->level + 1;
            thr_args1.need_exit = 1;

            pthread_t tid1;
            int flag1 = 1;
            int rc = pthread_create(&tid1, NULL, p_pthr_merge, &thr_args1);
            if (rc)
            {
                s_pthr_merge(out, in_left, in_right, newLS, newRS);
                flag1--;
            }

            thr_args_merge_t thr_args2;
            thr_args2.out = out + newLS + newRS + 1;
            thr_args2.in_left = in_left + newLS + 1;
            thr_args2.in_right = in_right + newRS;
            thr_args2.l_size = l_size - newLS - 1;
            thr_args2.r_size = r_size - newRS;
            thr_args2.level = data->level + 1;
            thr_args2.need_exit = 0;

            p_pthr_merge(&thr_args2);

            if (flag1)
            {
                pthread_join(tid1, NULL);
            }
        }
    }
    if (data->need_exit)
    {
        pthread_exit(NULL);
    }
    else
    {
        return NULL;
    }
}

int pthr_do_binary_search(long key, long *array, int size)
{
    return pthr_binary_search(array, 0, size - 1, key);
}

int pthr_binary_search(long *array, int left, int right, long key)
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
            return pthr_binary_search(array, left, mid - 1, key);
        }
        // Search in the right subarray
        return pthr_binary_search(array, mid + 1, right, key);
    }
    return mid; // key is not in the list
}


