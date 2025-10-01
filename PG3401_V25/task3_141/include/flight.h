#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)
/* Struct for passasjer (enkeltlenket liste) */
typedef struct Passenger {
   int seatNumber;
   char name[50];
   int age;
   struct Passenger* next;
   } Passenger;

/* Struct for fly (dobbeltlenket liste) */
typedef struct Flight {
   char flightID[10];
   char destination[50];
   int seats;
   int time;
   Passenger* passengerList;
   struct Flight* prev;
   struct Flight* next;
  }   Flight;
#pragma pack()

/* Legger til et nytt fly i listen og returnerer ny head */
Flight* addFlight(
   Flight* head, const char* flightID, const char* destination,
   int seats, int time);

/* Legger til en passasjer(sortert på setenummer) til et spesifikt fly */
int addPassenger(
   Flight* head, const char* flightID, int seatNumber, const char* name,
   int age);

/* Returnerer fly nummer N i listen (starter på 1) */
Flight* getFlightN(Flight* head, int n);

/* Søker etter et fly til en gitt destinasjon og returnerer hvilkeet nummer 
det har i listen */
int findFlightByDestination(Flight* head, const char* destination);

/* Sletter et fly og alle tilhørende passasjerer fra listen, og returnerer
ny head */
Flight* deleteFlight(Flight* head, const char* flightID);

/* Endrer sete til en passasjer på et spesifikt fly */
int changePassengerSeat(Flight* head, const char* flightID, const char* name,
                        int newSeat);

/* Søker igjennom alle fly og skriver ut hvilke fly passasjeren er
registrert på */
void searchPassengerFlights(Flight* head, const char* name);

/* Søker etter passasjerer med flere enn 1 fly */
void findPassengersWithMultipleFlights(Flight* head);

/* Frigjør all dynamisk allokert minne(fly og passasjerlister) */
void freeAllFlights(Flight* head);

/* Skriver ut all informasjon om et fly og dets passasjerer */
void printFlightDetails(Flight* flight);

/* Sjekker om en strent inneholder tall */
int isDigitsOnly(const char* str);

/* Sjekker om en streng inneholder bokstaver */
int isAlphaOnly(const char* str);

#endif /* FLIGHT_H */


