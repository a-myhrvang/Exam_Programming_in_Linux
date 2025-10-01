#include "flight.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pgdbglog.h"

/* Sjekker om en streng inneholder tall */
int isDigitsOnly(const char* str) { 
   while(*str) {
      if (!isdigit((unsigned char)*str)) return 0;
      str++;
   }
   return 1;
}

/* Sjekker om en streng kun inneholder bokstaver */
int isAlphaOnly(const char* str) {
   while (*str) {
      if (!isalpha((unsigned char)*str)) return 0;
      str++;
   }
   return 1;
}

/* Legger til et nytt fly i starten av dobbeltlenket liste */
Flight* addFlight(Flight* head, const char* flightID, const char* destination,
                  int seats, int time) {

   if (!isDigitsOnly(flightID)) {
      fprintf(stderr, "Ugyldig FlightID: kun tall er tillatt\n");
      return head;
   }

   Flight* newFlight = (Flight*)malloc(sizeof(Flight));
   if (!newFlight) {
      pgdebug("Malloc feilet ved addFlight");
      return head;
   }
   memcpy(newFlight->flightID, flightID, sizeof(newFlight->flightID));
   memcpy(newFlight->destination, destination, sizeof(newFlight->destination));
   newFlight->seats = seats;
   newFlight->time = time;
   newFlight->passengerList = NULL;
   newFlight->prev = NULL;
   newFlight->next = head;
   if (head) head->prev = newFlight;
   pgdebug("Fly lagt til: %s", flightID);
   return newFlight;
}

/* Legger til en passasjer i sortert (etter sete) liste i et bestemt fly */
int addPassenger(Flight* head, const char* flightID, int seatNumber,
                  const char* name, int age) {

   if (!isDigitsOnly(flightID)) {
      fprintf(stderr, "Ugyldig FlightID: kun tall er tillatt\n");
      return -1;
   }

   if (!isAlphaOnly(name)) {
      fprintf(stderr, "Ugyldig navn: kun bokstaver er tillatt\n");
      return -1;
   }

   Flight* curr = head;
   while (curr) {
      if (strcmp(curr->flightID, flightID) == 0) 
      break;
      curr = curr->next;
   }
   if (!curr) {
      pgdebug("Fly ikke funnet %s", flightID);
      return -1;
   }
   Passenger* newP = (Passenger*)malloc(sizeof(Passenger));
   if (!newP) {
      pgdebug("Malloc feilet ved addPassenger");
      return -1;
   }

   newP->seatNumber = seatNumber;
   memcpy(newP->name, name, sizeof(newP->name));
   newP->age = age;
   newP->next = NULL;
   Passenger** pp = &curr->passengerList;
   while (*pp && (*pp)->seatNumber < seatNumber) {
      pp = &(*pp)->next;
   }
   newP->next = *pp;
   *pp = newP;
   pgdebug("Passasjer lagt til: %s på fly %s", name, flightID);
   return 0;
}

/* Henter fly nr. N fra listen */
Flight* getFlightN(Flight* head, int n) {
   Flight* curr = head;
   int count = 1;

   while (curr && count < n) {
      curr = curr->next;
      count++;
   }
   return curr;
}

/* Finner fly med gitt destinasjon og returnerer posisjonen i listen */
int findFlightByDestination(Flight* head, const char* destination) {
   Flight* curr = head;
   int idx = 1;
   while (curr) {
      if (strcmp(curr->destination, destination) == 0) return idx;
      curr = curr->next;
      idx++;
   }
   return -1;
}

/* Sletter et fly og alle tilhørende passasjerer fra listen */
Flight* deleteFlight(Flight* head, const char* flightID) {
   Flight* curr = head;

   if (!isDigitsOnly(flightID)) {
      fprintf(stderr, "Ugyldig FlightID: kun tall er tillatt\n");
      return NULL;
   }

   while (curr) {
      if (strcmp(curr->flightID, flightID) == 0) {
         if (curr->prev) curr->prev->next = curr->next;
         if (curr->next) curr->next->prev = curr->prev;
         if (curr == head) head = curr->next;

         Passenger* p = curr->passengerList;
         while (p) {
            Passenger* tmp = p;
            p = p->next;
            free(tmp);
         }
         free(curr);
         pgdebug("Fly slettet: %s", flightID);
         return head;
      }
      curr = curr->next;
   }
   pgdebug("Fly ikke funnet for sletting: %s", flightID);
   return head;
}

/* Endrer sete til en passasjer på et fly (sletter gammel og legger til ny) */
int changePassengerSeat(Flight* head, const char* flightID, const char* name,
                        int newSeat) {
                             
   Flight* curr = head;

   if (!isDigitsOnly(flightID)) {
      fprintf(stderr, "Ugyldig FlightID: kun tall er tillatt\n");
      return -1;
   }

   if (!isAlphaOnly(name)) {
      fprintf(stderr, "Ugyldig navn: kun bokstaver er tillatt\n");
      return -1;
   }
                             
   while (curr) {
      Passenger* p = curr->passengerList;
      Passenger* prev = NULL;
      while (p) {
         if (strcmp(p->name, name) == 0) {
            int age = p->age;
            if (prev) prev->next = prev->next;
            else curr->passengerList = p->next;
            free(p);
            return addPassenger(curr, flightID, newSeat, name, age);
         }
         prev = p;
         p = p->next;
      }
      curr = curr->next;
   }
   pgdebug("Passasjer ikke funnet for setebytte: %s", name);
   return -1;
}

/* Søker etter en passasjer i alle fly og skriver ut hvilke fly passasjeren
/* har billetter på */
void searchPassengerFlights(Flight* head, const char* name) {
   Flight* curr = head;

   if (!isAlphaOnly(name)) {
      fprintf(stderr, "Ugyldig navn: kun bokstaver er tillatt\n");
      return;
   }

   while (curr) {
      Passenger* p = curr->passengerList;
      while (p) {
         if (strcmp(p->name, name) == 0) {
            printf("%s har billet på fly %s\n", name, curr->flightID);
         }
         p = p->next;
      }
      curr = curr->next;
   }
}

/* Finner og skriver ut passasjerer som har billet på flere fly */
void findPassengersWithMultipleFlights(Flight* head) {
   Flight* curr = head;
   char seen[100][32] = {0};
   int count[100] = {0};
   int total = 0;
   while (curr) {
      Passenger* p = curr->passengerList;
      while (p) {
         int found = 0;
         for (int i = 0; i < total; i++) {
            if (strcmp(seen[i], p->name) == 0) {
               count[i]++;
               found = 1;
               break;
            }
         }
         if (!found) {
            memcpy(seen[total], p->name, sizeof(seen[total]));
            count[total++] = 1;
         }
         p = p->next;
      }
      curr = curr->next;
   }
   for (int i = 0; i < total; i++) {
      if (count[i] > 1) printf("%s har billetter på flere fly\n", seen[i]);
   }
}

/* Frigjør all minne brukt av fly og passasjer */
void freeAllFlights(Flight*head) {
   while (head) {
      Flight* tmp = head;
      head = head->next;
      Passenger* p = tmp->passengerList;
      while (p) {
         Passenger* ptmp = p;
         p = p->next;
         free(ptmp);
      }
      free(tmp);
   }
   pgdebug("All minne frigjort for fly og passasjerer");
}

/* Skriver ut detaljert informasjon om et fly og dets passasjerer */
void printFlightDetails(Flight* flight) {
   if (!flight) return;
   printf("Fly: %s til %s, Sete: %d, Tid: %d\n", flight->flightID,
                                                 flight->destination,
                                                 flight->seats,
                                                 flight->time);
   Passenger* p = flight->passengerList;
   printf("Passasjerinfo:\n");
   while (p) {
      printf("Sete: %d, Navn: %s, Alder: %d\n", p->seatNumber,
                                                p->name,
                                                p->age);
      p = p->next;
   }
}


      
   
