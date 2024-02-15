/*
 * Title: Verkefni 2
 * Tinna Osk Traustadottir
 */
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
// Global variables and data structures
struct LinkedList
{
 int data;
 struct LinkedList *next;
};

LinkedList* first = NULL; // Fyrsta stakid bendir a null. null pointer
LinkedList* current = NULL;

void addFront (int data) {
 LinkedList* temp = (LinkedList*)malloc(sizeof(LinkedList)); // Gefum bendinum okkar minnis adressu
 temp->data = data; // Setjum gognin inn i stakid
 temp->next = first; // Bendum a fysta stakid
 first = temp; // Bendum a naesta stak?
}

void addBack (int data) {
 LinkedList* temp = (LinkedList*)malloc(sizeof(LinkedList)); // Gefum bendinum okkar minnis adressu
 temp->data = data; // Setjum gognin inn i stakid
 struct LinkedList*lk = first; // buum til bendi sem heiti lk
 temp->next = NULL; // Byrjum a thvi ad vera með null
 if(first == NULL) {
 first = temp;
 }
 else{
 lk = first;
 while(lk->next != NULL){
 // printf("%d\n",lk);
 lk = lk->next;
 }
 lk->next = temp;
 }
}
void removeFromFront (){
 LinkedList* temp = first;
 first = first->next;
 free(temp); // leysum ut minnid
}
void removeFromBack (){
 struct LinkedList*lk = first;
 while(lk->next->next != NULL){
 lk = lk->next;
 }
 lk->next = NULL;
 free(lk); //Leysum ut minnid
}
bool isEmpty(){
 return first == NULL;
}
void print(void){
 struct LinkedList* p = first;
 printf("[");
 while(p != NULL){
 printf(" %d ", p->data);
 // printf(" %p ", p->next);
 p = p->next;
 }
 printf("]\n");
}
int main() {
 stdio_init_all();
 sleep_ms(20000);
 printf("Started pico!\n");
 addFront(5);
 addFront(10);
 addFront(19);
 sleep_ms(2000);
 printf("Fronts:\n");
 print();
 printf("Back: \n");
 print();
 addBack(3);
 addBack(14);
 sleep_ms(2000);
 printf("Remove: \n");
 print();
 removeFromFront();
 sleep_ms(2000);
 removeFromBack();
 sleep_ms(2000);
 print();
 return 0;
}