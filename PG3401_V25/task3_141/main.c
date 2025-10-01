#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "pgdbglog.h"

#define INPUT_SIZE 100 /* Bufferstørrelse for tekstinput */

int main() {
   Flight* head = NULL; /* Start på dobbeltlenket liste med fly */
   int running = 1;
   char input[INPUT_SIZE];

   while (running) {
      printf("      ==============================================     \n");
      printf("      ||                                          ||     \n");
      printf("      ||     Flyreservasjonssystem - Oppgave 3    ||     \n");
      printf("      ||                                          ||     \n");
      printf("      +============================================+     \n");
      printf("      |                                            |     \n");
      printf("      |  (1) Legg til fly                          |     \n");
      printf("      |  (2) Legg til passasjer                    |     \n");
      printf("      |  (3) Hent og vis fly nr. N                 |     \n");
      printf("      |  (4) Finn fly til destinasjon              |     \n");
      printf("      |  (5) Slett fly                             |     \n");
      printf("      |  (6) Endre sete til passasjer              |     \n");
      printf("      |  (7) Finn alle fly for passasjer           |     \n");
      printf("      |  (8) Finn passasjerer med flere fly        |     \n");
      printf("      |  (9) Avslutt                               |     \n");
      printf("      |                                            |     \n");
      printf("      +--------------------------------------------+     \n");
      printf("                                                         \n");
      printf("           Velg et alternativ: ");
      

      int choice = 0;
      fflush(stdout);
      
      if (fgets(input, sizeof(input), stdin)) {
         choice = atoi(input);
      } else {
         printf("Ugyldig input.\n");
         continue;
      }

      char flightID[16], destination[32], name[32];
      int seats, time, n, seatNumber, age, newSeat;

      switch (choice) {
         case 1: /* Legg til fly */ 
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Registrer nytt fly +\n");
            printf("\n");          
            printf(" >   FlightID:  ");
            fgets(flightID, sizeof(flightID), stdin);
            flightID[strcspn(flightID, "\n")] = 0;

            if (!isDigitsOnly(flightID)) {
               printf(" >   Ugyldig FlightID: kun tall er tillatt\n");
               continue;
            }
   
            printf(" >   Destinasjon: ");
            fgets(destination, sizeof(destination), stdin);
            destination[strcspn(destination, "\n")]= 0;

            if (!isAlphaOnly(destination)) {
               printf(" >   Ugyldig navn: kun bokstaver er tillatt\n");
               continue;
            }

            printf(" >   Antall seter: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';

             if (!isDigitsOnly(input)) {
               printf(" >   Ugyldig antall! Kun tall er tillatt\n");
               continue;
            }
            seats = atoi(input);

            printf(" >   Avgangstid: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';

            if (!isDigitsOnly(input)) {
               printf(" >   Ugyldig tid! Kun tall er tillatt\n");
               continue;
            }
            time = atoi(input);

            head = addFlight(head, flightID, destination, seats, time);
            break;

         case 2: /* Legg til passasjer */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Registrer ny passasjer +\n");
            printf("\n");
            printf(" >   FlightID: ");
            fgets(flightID, sizeof(flightID), stdin);
            flightID[strcspn(flightID, "\n")] = 0;
            
            if (!isDigitsOnly(flightID)) {
               printf(" >   Ugyldig FlightID: kun tall er tillatt\n");
               continue;
            }
            printf(" >   Setenummer: ");
            fgets(input, sizeof(input), stdin); 
            input[strcspn(input, "\n")] = '\0';
            
            if (!isDigitsOnly(input)) {
               printf(" >   Ugyldig setenummer! Kun tall er tillatt\n");
               continue;
            }
            seatNumber = atoi(input);

            printf(" >   Navn: ");
            fgets(name, sizeof(name), stdin); 
            name[strcspn(name, "\n")] = 0;

             if (!isAlphaOnly(name)) {
               printf(" >   Ugyldig navn: kun bokstaver er tillatt\n");
               continue;
            }
            
            printf(" >   Alder: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            
            if (!isDigitsOnly(input)) {
               printf(" >   Ugyldig alder! Kun tall er tillatt\n");
               continue;
            }
            age = atoi(input);

            addPassenger(head, flightID, seatNumber, name, age);
            break;

         case 3: /* Vis fly nr. N */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Hent fly basert på nummer +\n");
            printf("\n");
            printf(" >   Hent fly nr: ");
            if (fgets(input, sizeof(input), stdin)) n = atoi(input);
            printFlightDetails(getFlightN(head, n));
            break;

         case 4: /* Finn fly til destinasjon */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Velg destinasjon +\n");
            printf("\n");
            printf(" >   Destinasjon: ");
            fgets(destination, sizeof(destination), stdin);
            destination[strcspn(destination, "\n")] = 0;

             if (!isAlphaOnly(destination)) {
               printf(" >   Ugyldig navn: kun bokstaver er tillatt\n");
               continue;
            }

            {            
               int pos = findFlightByDestination(head, destination);
                  if (pos > 0)
                     printf(" >   Fly til %s funnet på posisjon %d\n", destination,
                            pos);
                  else 
                     printf(" >   Ingen fly til %s funnet\n", destination);
            }            
            break;

         case 5: /* Slett fly */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Slett et fly +\n");
            printf("\n");
            printf(" >   FlightID som skal slettes: ");
            fgets(flightID, sizeof(flightID), stdin);
            flightID[strcspn(flightID, "\n")] = 0;

            head = deleteFlight(head, flightID);
            break;

         case 6: /* Endre sete til passasjer */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Endre sete til passasjer +\n");
            printf("\n");
            printf(" >   FlightID: ");
            fgets(flightID, sizeof(flightID), stdin);
            flightID[strcspn(flightID, "\n")] = 0;

            printf(" >   Passasjernavn: ");
            fgets(name, sizeof(name), stdin); 
            name[strcspn(name, "\n")] = 0;

             if (!isAlphaOnly(name)) {
               printf(" >   Ugyldig navn: kun bokstaver er tillatt\n");
               continue;
            }
            
            printf(" >   Nytt sete: ");
            fgets(input, sizeof(input), stdin); 
            input[strcspn(input, "\n")] = 0;

            if (!isDigitsOnly(input)) {
               printf(" >   Ugyldig setenummer! Kun tall er tillatt\n");
               continue;
            }
            newSeat = atoi(input);

            changePassengerSeat(head, flightID, name, newSeat);
            break;

         case 7: /* Finn alle fly passasjeren er på */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Se alle fly booket for en passasjer +\n");
            printf("\n");
            printf(" >   Passasjernavn: ");
            fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;

            searchPassengerFlights(head, name);
            break;

         case 8: /* Finn passasjerer med flere fly */
            printf("\n");
            printf("+--------------------------------------------------+\n");
            printf("\n");
            printf("   + Se alle passasjerer med flere flybookinger +\n");
            printf("\n");
            findPassengersWithMultipleFlights(head);
            break;

         case 9: /* Avslutt */
            running = 0;
            break;

         default:
            printf("Ugyldig valg. Prøv igjen.\n");
      }
   }

   /* Rydder opp minne før avslutning */
   freeAllFlights(head);
   pgdebug("Program avsluttet og minne frigjort");
   return 0;
}
            
      
