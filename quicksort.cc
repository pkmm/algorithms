
#include <stdio.h>


void exchage(int& i, int& j)
{
    int tmp = i;
    i = j;
    j = tmp;
}

int partition(int *a, int p, int r)
{
    int x = a[r];
    int i = p - 1;
    for (int j=p; j<r; j++)
    {
        if (a[j] <= x)
        {
            i = i + 1;
            exchage(a[i], a[j]);
        }
    }
    i = i + 1;
    exchage(a[i], a[r]);
    return i;
}

void quick_sort(int *a, int p, int r)
{
    if (p < r)
    {
        int q = partition(a, p, r);
        quick_sort(a, p, q-1);
        quick_sort(a, q+1, r);
    }
}

int main()
{
    int nums[] = {2,3,4,6,1,5,8,7};
    int len = sizeof(nums) / sizeof(int);
    quick_sort(nums, 0, len-1);
    for (int i=0; i<len; i++)
    {
        printf("%d ", nums[i]);
    }
}
