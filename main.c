#include <stdio.h>
#include "malloc.h"

#define MAXSIZE 10
typedef int ElemType;

typedef struct List{
    ElemType *pList;
    int length;
    int listSize;                                   //��̬����
}SqlList;

void extend(SqlList *L){                            //����˳���
    ElemType *pNew = (ElemType *) malloc(sizeof(ElemType) * (2 * (*L).listSize));
    for (int i = 0; i < (*L).length; ++i) { //����L�е�ÿһ��Ԫ��
        pNew[i] = (*L).pList[i];
    }
    (*L).pList = pNew;
    (*L).listSize = L->listSize * 2;
}

void output(SqlList *L){                            //���˳���
    for (int i = 0; i < L->length; ++i) {
        printf("  %d",L->pList[i]);
    }
    printf("\n");
}

//����1
void initial(SqlList *L){                           //��ʼ����̬˳���
    (*L).pList = (ElemType *) malloc(sizeof(ElemType) * MAXSIZE);
    (*L).listSize = MAXSIZE;
    (*L).length = 0;
}
//����2
int delete(ElemType *e,int n,SqlList *L){            //ɾ��
    if(n < 1 || n > L->length) return 0;
    *e = L->pList[n];
    for (int i = n - 1; i < L->length; ++i) {
        L->pList[i] =L->pList[i + 1];
    }
    --L->length;
    return 1;
}

//����3
int insert(ElemType e,int n,SqlList *L){            //�ڵ�n��λ��ǰ����e
    if(n < 1 || n > (*L).length) return 0;
    if((*L).length >= (*L).listSize){
        extend(L);
    }
    for (int i = (*L).length - 1; i >= n - 1; --i) { //��n��λ�������ƶ�һ��λ��
        (*L).pList[i + 1] = (*L).pList[i];
    }
    (*L).pList[n - 1] = e;
    (*L).length ++;
    return 1;
}

//����4
void deleterMinElem(SqlList *L){                     //��Сֵɾ���㷨�������һ��Ԫ���
    int minElemIndex = 0;
    for (int i = 1; i < L->length; ++i) {
        if(L->pList[i] < L->pList[minElemIndex]){
            minElemIndex = i;
        }
    }
    L->pList[minElemIndex] = L->pList[L->length - 1];
    L->length--;
}

//����5
void deleteElemsRange_1(SqlList *L,ElemType s,ElemType t){         //�����㷨������˳���ɾ��ֵ��s��t֮���Ԫ��
    int newIndex = 0;
    for (int i = 0; i < L->length; ++i) {
        if(L->pList[i] < s || L->pList[i] > t){
            L->pList[newIndex++] = L->pList[i];
        }
    }
    L->length = newIndex;
}

//����6
void deleteElemsRange_2(SqlList *L,ElemType s,ElemType t){      //ɾ���ǵݼ�˳����е�ֵ��[s,t]֮���Ԫ��
    int fIndex,bIndex;
    for (fIndex = 0; fIndex < L->length && L->pList[fIndex] < s; ++fIndex);   //�ƶ�fIndex�����һ��С��s��λ��
    if(fIndex >= L->length) return;     //û�д��ڵ���s��ֵ
    for (bIndex = fIndex;bIndex < L->length && L->pList[bIndex] <= t; ++bIndex);     //�ƶ�bIndex����һ������t��λ��
    for (; bIndex < L->length; ++bIndex) {       //����һ������t��Ԫ���ƶ���fIndex��
        L->pList[fIndex++] = L->pList[bIndex];
    }
    L->length = fIndex;
}

//����7
void deleteRepeatElem(SqlList *L){          //ɾ���ǵݼ�˳���L�е��ظ�Ԫ��
    int fIndex = 0,bIndex = 1;              //fIndex:�����ظ�ֵ��
    for (;bIndex < L->length; ++bIndex) {
        if(L->pList[bIndex] != L->pList[fIndex]) {      //�����һ��Ԫ�غ�ǰһ��Ԫ�ز���ȣ��ͷŵ�fIndex��
            L->pList[++fIndex] = L->pList[bIndex];
        }
    }
    L->length = fIndex + 1;
}

//����8��˳���A��B��Ԫ�ظ����ֱ�Ϊm��n����A�������򣬱�B���������������ж���������ͬ��Ԫ�ء�
//����8.1
void mergeSqList(SqlList *A,SqlList *B,SqlList *C){     //������ϲ��������е�Ԫ�ض��洢����C�С�
    int aIndex = 0,bIndex = 0,cIndex = 0;
    while(aIndex < A->length && bIndex < B->length){    //����A,B�н�С��Ԫ��
        if(A->pList[aIndex] <= B->pList[bIndex]){
            C->pList[cIndex++] = A->pList[aIndex++];
        } else {
            C->pList[cIndex++] = B->pList[bIndex++];
        }
    }
    while(aIndex < A->length){          //��ʣ���Ԫ�ط����C��
        C->pList[cIndex++] = A->pList[aIndex++];
    }
    while(bIndex < B->length){
        C->pList[cIndex++] = B->pList[bIndex++];
    }
    C->length = cIndex;     //����C�ĳ���
}

//����8.2
void sortSqList(SqlList *A,SqlList *B){        // ����B��Ԫ�غϲ�����A�У�ʹ��A�е�Ԫ�ر�������
    int aIndex = A->length - 1;         //A���һ��Ԫ�ص�λ��
    int bIndex = 0;         //B���һ��Ԫ�ص�λ��
    int endIndex = A->length + B->length - 1;      //�ϲ������Ա�����һ��λ��
    while(aIndex >= 0 && bIndex < B->length){
        if(A->pList[aIndex] >= B->pList[bIndex]) {  //���A�����ֵ����B�����ֵ����A�����ֵ����A�����
            A->pList[endIndex--] = A->pList[aIndex--];
        } else {                                    //���A�����ֵС��B�����ֵ����B�����ֵ����A�����
            A->pList[endIndex--] = B->pList[bIndex++];
        }
    }
    while(bIndex < B->length){
        A->pList[endIndex++] = B->pList[bIndex++];
    }
    A->length = A->length + B->length;
}

//����8.3
void sort(SqlList *A,int k){        //Aǰk��Ԫ�ص�������K��Ԫ�صݼ���A����
    int curIndex = k - 1;          //��������������λ�ÿ�ʼ����
    while(curIndex < A->length){
        ElemType e = A->pList[curIndex];    //e���浱ǰ��Ҫ���������ֵ
        int i = curIndex - 1;               //i�ǵ�ǰ����Ԫ�ص�ǰ��
        while(i >= 0 && A->pList[i] > e){   //�����ǰ����Ԫ��С��ǰ����ǰ���������һλ���ڳ�λ��
            A->pList[i + 1] = A->pList[i];  //A->pList[i + 1]����e
            --i;
        }
        A->pList[i + 1] = e;                //�ҵ���λ��
        ++curIndex;
    }
}

//����9
void intersect(SqlList *A,SqlList *B){      //A,B��������A,B�Ľ�����������A
    int curLength = 0;      //�±���
    int aIndex = 0;
    int bIndex = 0;
    while(aIndex < A->length && bIndex < B->length){
        if(A->pList[aIndex] < B->pList[bIndex]){            //��A->pList[aIndex]��С������A������
            ++aIndex;
        }
        else if(A->pList[aIndex] > B->pList[bIndex]){       //��B->pList[bIndex]��С������B������
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

//����10
void except(SqlList *A,SqlList *B){       //A,B��������A-B��������A
    int curIndex = 0;           //�±���
    int aIndex = 0;
    int bIndex = 0;
    while(aIndex < A->length && bIndex < B->length){
        if(A->pList[aIndex] < B->pList[bIndex]) {       //A->pList[aIndex]��B->pList[bIndex]С��ֱ�Ӱ�A->pList[aIndex]�ŵ���ͷ
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

//����11
void reverse(SqlList *L) {          //��תA
    int begin = 0;
    int end = L->length - 1;
    ElemType e;
    while (begin < end) {
        e = L->pList[begin];
        L->pList[begin++] = L->pList[end];
        L->pList[end--] = e;
    }
}

//����12
void subReverse(SqlList *L, int begin, int end) {
    ElemType e;
    while (begin < end) {
        e = L->pList[begin];
        L->pList[begin++] = L->pList[end];
        L->pList[end--] = e;
    }
}

void leftROL(SqlList *L,int r){         //ѭ������
    reverse(L);                                           //1.��תL
    subReverse(L,0,L->length - 1 - r);          //2.��תǰlength - 1 - r��
    subReverse(L,L->length - r,L->length - 1);  //3.��ת����r - 1����1��2��3��ת˳�����⣩
}
void rightROL(SqlList *L,int r){        //ѭ������
    reverse(L);                                           //1.��תL
    subReverse(L,0,r - 1);                      //2.��תǰr - 1��
    subReverse(L,r,L->length - 1);              //3.��ת��length - 1 - r����1��2��3��ת˳�����⣩
}
//1��2��3��ת˳������,����ע�����ƺ����Ƶ�ǰ��ת�������෴��
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
