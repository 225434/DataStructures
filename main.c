#include <stdio.h>
#include "malloc.h"

#define MAXSIZE 10
typedef int ElemType;

typedef struct List{
    ElemType *pList;
    int length;
    int listSize;                                   //动态数组
}SqlList;

void extend(SqlList *L){                            //扩充顺序表
    ElemType *pNew = (ElemType *) malloc(sizeof(ElemType) * (2 * (*L).listSize));
    for (int i = 0; i < (*L).length; ++i) { //访问L中的每一个元素
        pNew[i] = (*L).pList[i];
    }
    (*L).pList = pNew;
    (*L).listSize = L->listSize * 2;
}

void output(SqlList *L){                            //输出顺序表
    for (int i = 0; i < L->length; ++i) {
        printf("  %d",L->pList[i]);
    }
    printf("\n");
}

//例题1
void initial(SqlList *L){                           //初始化动态顺序表
    (*L).pList = (ElemType *) malloc(sizeof(ElemType) * MAXSIZE);
    (*L).listSize = MAXSIZE;
    (*L).length = 0;
}
//例题2
int delete(ElemType *e,int n,SqlList *L){            //删除
    if(n < 1 || n > L->length) return 0;
    *e = L->pList[n];
    for (int i = n - 1; i < L->length; ++i) {
        L->pList[i] =L->pList[i + 1];
    }
    --L->length;
    return 1;
}

//例题3
int insert(ElemType e,int n,SqlList *L){            //在第n个位置前插入e
    if(n < 1 || n > (*L).length) return 0;
    if((*L).length >= (*L).listSize){
        extend(L);
    }
    for (int i = (*L).length - 1; i >= n - 1; --i) { //第n个位置往后移动一个位置
        (*L).pList[i + 1] = (*L).pList[i];
    }
    (*L).pList[n - 1] = e;
    (*L).length ++;
    return 1;
}

//例题4
void deleterMinElem(SqlList *L){                     //最小值删除算法，由最后一个元素填补
    int minElemIndex = 0;
    for (int i = 1; i < L->length; ++i) {
        if(L->pList[i] < L->pList[minElemIndex]){
            minElemIndex = i;
        }
    }
    L->pList[minElemIndex] = L->pList[L->length - 1];
    L->length--;
}

//例题5
void deleteElemsRange_1(SqlList *L,ElemType s,ElemType t){         //过滤算法，无序顺序表删除值在s和t之间的元素
    int newIndex = 0;
    for (int i = 0; i < L->length; ++i) {
        if(L->pList[i] < s || L->pList[i] > t){
            L->pList[newIndex++] = L->pList[i];
        }
    }
    L->length = newIndex;
}

//例题6
void deleteElemsRange_2(SqlList *L,ElemType s,ElemType t){      //删除非递减顺序表中的值在[s,t]之间的元素
    int fIndex,bIndex;
    for (fIndex = 0; fIndex < L->length && L->pList[fIndex] < s; ++fIndex);   //移动fIndex到最后一个小于s的位置
    if(fIndex >= L->length) return;     //没有大于等于s的值
    for (bIndex = fIndex;bIndex < L->length && L->pList[bIndex] <= t; ++bIndex);     //移动bIndex到第一个大于t的位置
    for (; bIndex < L->length; ++bIndex) {       //将第一个大于t的元素移动到fIndex后
        L->pList[fIndex++] = L->pList[bIndex];
    }
    L->length = fIndex;
}

//例题7
void deleteRepeatElem(SqlList *L){          //删除非递减顺序表L中的重复元素
    int fIndex = 0,bIndex = 1;              //fIndex:最大非重复值长
    for (;bIndex < L->length; ++bIndex) {
        if(L->pList[bIndex] != L->pList[fIndex]) {      //如果后一个元素和前一个元素不相等，就放到fIndex后
            L->pList[++fIndex] = L->pList[bIndex];
        }
    }
    L->length = fIndex + 1;
}

//例题8：顺序表A和B的元素个数分别为m和n，表A升序排序，表B降序排序，两个表中都不存在相同的元素。
//例题8.1
void mergeSqList(SqlList *A,SqlList *B,SqlList *C){     //将两表合并，两表中的元素都存储到表C中。
    int aIndex = 0,bIndex = 0,cIndex = 0;
    while(aIndex < A->length && bIndex < B->length){    //放入A,B中较小的元素
        if(A->pList[aIndex] <= B->pList[bIndex]){
            C->pList[cIndex++] = A->pList[aIndex++];
        } else {
            C->pList[cIndex++] = B->pList[bIndex++];
        }
    }
    while(aIndex < A->length){          //将剩余的元素放入表C中
        C->pList[cIndex++] = A->pList[aIndex++];
    }
    while(bIndex < B->length){
        C->pList[cIndex++] = B->pList[bIndex++];
    }
    C->length = cIndex;     //更新C的长度
}

//例题8.2
void sortSqList(SqlList *A,SqlList *B){        // 将表B的元素合并到表A中，使表A中的元素保持有序
    int aIndex = A->length - 1;         //A最后一个元素的位置
    int bIndex = 0;         //B最后一个元素的位置
    int endIndex = A->length + B->length - 1;      //合并后线性表的最后一个位置
    while(aIndex >= 0 && bIndex < B->length){
        if(A->pList[aIndex] >= B->pList[bIndex]) {  //如果A的最大值大于B的最大值，把A的最大值放在A最后面
            A->pList[endIndex--] = A->pList[aIndex--];
        } else {                                    //如果A的最大值小于B的最大值，把B的最大值放在A最后面
            A->pList[endIndex--] = B->pList[bIndex++];
        }
    }
    while(bIndex < B->length){
        A->pList[endIndex++] = B->pList[bIndex++];
    }
    A->length = A->length + B->length;
}

//例题8.3
void sort(SqlList *A,int k){        //A前k个元素递增，后K个元素递减，A排序
    int curIndex = k - 1;          //从有序递增的最后位置开始排序
    while(curIndex < A->length){
        ElemType e = A->pList[curIndex];    //e储存当前需要进行排序的值
        int i = curIndex - 1;               //i是当前排序元素的前驱
        while(i >= 0 && A->pList[i] > e){   //如果当前排序元素小于前驱，前驱就往后放一位，腾出位置
            A->pList[i + 1] = A->pList[i];  //A->pList[i + 1]就是e
            --i;
        }
        A->pList[i + 1] = e;                //找到了位置
        ++curIndex;
    }
}

//例题9
void intersect(SqlList *A,SqlList *B){      //A,B递增，求A,B的交集，并放在A
    int curLength = 0;      //新表长度
    int aIndex = 0;
    int bIndex = 0;
    while(aIndex < A->length && bIndex < B->length){
        if(A->pList[aIndex] < B->pList[bIndex]){            //若A->pList[aIndex]较小，则往A后面找
            ++aIndex;
        }
        else if(A->pList[aIndex] > B->pList[bIndex]){       //若B->pList[bIndex]较小，则往B后面找
            ++bIndex;
        } else {
           A->pList[curLength] = A->pList[aIndex];
           ++curLength;
           ++aIndex;
           ++bIndex;
        }
    }
    A->length  = curLength;
}

//例题10
void except(SqlList *A,SqlList *B){       //A,B递增，求A-B，并放在A
    int curIndex = 0;           //新表长度
    int aIndex = 0;
    int bIndex = 0;
    while(aIndex < A->length && bIndex < B->length){
        if(A->pList[aIndex] < B->pList[bIndex]) {       //A->pList[aIndex]比B->pList[bIndex]小，直接把A->pList[aIndex]放到表头
            A->pList[curIndex] = A->pList[aIndex];
            ++curIndex;
            ++aIndex;
        } else if (A->pList[aIndex] > B->pList[bIndex]){
            ++bIndex;
        } else {
            ++aIndex;
            ++bIndex;
        }
    }
    A->length = curIndex;
}

//例题11
void reverse(SqlList *L) {          //反转A
    int begin = 0;
    int end = L->length - 1;
    ElemType e;
    while (begin < end) {
        e = L->pList[begin];
        L->pList[begin++] = L->pList[end];
        L->pList[end--] = e;
    }
}

//例题12
void subReverse(SqlList *L, int begin, int end) {
    ElemType e;
    while (begin < end) {
        e = L->pList[begin];
        L->pList[begin++] = L->pList[end];
        L->pList[end--] = e;
    }
}

void leftROL(SqlList *L,int r){         //循环左移
    reverse(L);                                           //1.反转L
    subReverse(L,0,L->length - 1 - r);          //2.反转前length - 1 - r个
    subReverse(L,L->length - r,L->length - 1);  //3.反转后面r - 1个（1、2、3反转顺序任意）
}
void rightROL(SqlList *L,int r){        //循环右移
    reverse(L);                                           //1.反转L
    subReverse(L,0,r - 1);                      //2.反转前r - 1个
    subReverse(L,r,L->length - 1);              //3.反转后length - 1 - r个（1、2、3反转顺序任意）
}
//1、2、3反转顺序任意,但是注意左移和右移的前后反转个数是相反的
//////////////////////////////////////////////////////////////////////////////////
void test(){
    SqlList L;
    initial(&L);
    L.length = 10;
    for (int i = 0; i <= 10; ++i) {
        L.pList[i] = 11 + i;
    }
    L.pList[3] = 13;
    L.pList[1] = 13;
    L.pList[5] = 15;
    output(&L);
//    insert(666,1,&L);
//    output(&L);
//    int e;
//    delete(&e,1,&L);
//    output(&L);
//    deleterMinElem(&L);
//    output(&L);
//    deleteElemsRange_2(&L,12,15);
//    output(&L);
    deleteRepeatElem(&L);
    output(&L);
}

void test2(){
//    SqlList A,B,C;
    SqlList A,B;
    initial(&A);
    initial(&B);
//    initial(&C);
    A.length = 5;
    B.length = 5;
    for (int i = 0; i < 5; ++i) {
        A.pList[i] =  1 + i;
        B.pList[i] =  1 + i * 2;
    }
    output(&A);
//    output(&B);
//    mergeSqList(&A,&B,&C);
//    sortSqList(&A,&B);
//    intersect(&A,&B);
//    except(&A,&B);
//    reverse(&A);
//    output(&A);
//    subReverse(&A,1,3);
    rightROL(&A,2);
    output(&A);
}

int main() {
    test2();
    return 0;
}
