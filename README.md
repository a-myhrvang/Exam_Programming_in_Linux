# Exam_Programming_in_Linux
Exam project, programming in Linux writing C

---

TEA Encryption & Flyreservasjonssystem - Eksamensprosjekt i C
Dette prosjektet er resultatet av en eksamensoppgave i faget PG3401, og består av to hoveddeler: implementasjon og bruk av TEA-kryptering/dekryptering, samt utviklingen av et flyreservasjonssystem i C89-standard. Koden er skrevet i C og følger retningslinjer for strukturell programmering uten objektorientering. Prosjektet kombinerer lavnivå programmering, filhåndtering, dynamisk minneallokering, validering av input, samt nettverkskommunikasjon.

---
 
Del 1-5: Flyreservasjonssystem

Del 1 - Grunnstruktur og datastrukturer

Her implementerte jeg grunnleggende datastrukturer ved hjelp av lenkede lister for å representere fly og passasjerer. Flyene lagres som en Flight-struktur med flyID, destinasjon, antall seter og avgangstid. Passasjerer er koblet til fly via en egen struktur med navn, alder og setenummer.

Hva jeg lærte:

•	Bruk av struct og typedef i C
•	Dynamisk minneallokering og friing av minne med malloc og free
•	Innføring i hvordan datastrukturer henger sammen i praksis

---
 
Del 2 - Lagre og laste data fra fil

I denne delen implementerte jeg funksjonalitet for å skrive fly- og passasjerdata til fil og lese det tilbake. Binær filhåndtering ble benyttet for effektivitet.

Hva jeg lærte:

•	Binær lesing og skriving i C (fread, fwrite)
•	Feilhåndtering ved filoperasjoner
•	Persistens av datastrukturer

---
 
Del 3 - Sletting og søk

Her la jeg til funksjoner for å slette spesifikke fly eller passasjerer, samt søkefunksjoner basert på ID og navn. Funksjonene inkluderer både direkte og betingede søk.

Hva jeg lærte:

•	Manipulasjon av lenkede lister
•	Feilhåndtering ved søk og sletting
•	Forståelse av pekere og indirekte tilgang til minne

---
 
Del 4 - Validering av brukerinput

For å sikre programstabilitet og gyldig data, implementerte jeg flere funksjoner for å validere brukerinput. Eksempler inkluderer kontroll på at flyID kun består av tall, at antall seter er positivt, og at navn ikke inneholder ugyldige tegn.

Hva jeg lærte:

•	Hvordan sikre datakvalitet gjennom inputvalidering
•	Bruk av isdigit, strlen, og egne hjelpefunksjoner
•	Sammenheng mellom brukergrensesnitt og intern datasikkerhet

---
 
Del 5 - Brukervennlig meny og interaktivitet

I denne delen implementerte jeg et brukervennlig tekstbasert menygrensesnitt, hvor brukeren enkelt kan legge til, vise, søke, og slette data.

Hva jeg lærte:

•	Kontrollflyt i interaktive programmer
•	Hvordan strukturere kode for lesbarhet og vedlikehold
•	Bruk av fgets, atoi og feilhåndtering ved brukerinput

---
 
Del 6 - TEA-dekryptering og nettverkskommunikasjon

I denne delen bygget jeg en klient som kobler til en server (EWA), mottar kryptert binærdata, og forsøker å dekryptere det ved hjelp av TEA-algoritmen. Nøkkelen er 128-bit hvor alle bytes er like. Utfordringen var å gjette riktig nøkkel og skrive ut lesbar tekst til fil etter dekryptering.

Hva jeg lærte:

•	Hvordan TEA (Tiny Encryption Algorithm) fungerer i praksis
•	Bitoperasjoner, skift og XOR-operasjoner i C
•	Nettverksprogrammering med sockets i lavnivå C
•	Debugging og analyse av binærdata
•	PKCS5 padding-konseptet

Til tross for korrekt implementert algoritme, støtte jeg på utfordringer med nøkkelgjettingen. Jeg valgte å dokumentere dette i stedet for å bruke tid på videre feilsøking - noe jeg vurderte som riktig prioritering med tanke på eksamenstiden.

---
 
Refleksjon

Gjennom dette prosjektet har jeg virkelig fått kjenne på både frustrasjon og mestring. Når programmene ikke fungerer som forventet, kan det være utmattende og tidkrevende å feilsøke. Likevel er det få ting som slår følelsen av å endelig få outputen til å stemme. Jeg har opplevd ekte programmeringsglede, særlig når flyreservasjonssystemet endelig kunne lagre og hente passasjerdata uten feil.
Jeg har blitt mer komfortabel med C som språk, og samtidig forstått hvorfor man må være ekstremt nøye - det er ingen beskyttelsesnett! Små feil kan få store konsekvenser, og det krever disiplin å følge C89-standarden, spesielt når man står midt i en feilsøkingsøkt og glemmer å flytte deklarasjoner til starten av funksjonen.

---
 
Brukt teknologi og verktøy

•	Språk: C (C89 standard)
•	Kompilator: GCC
•	Debugging: printf-debugging og valideringsfunksjoner
•	Nettverksprogrammering: TCP/IP sockets
•	Git og GitHub for versjonskontroll og dokumentasjon

---

 
Kontakt

Har du spørsmål, eller er du en rekrutterer som vil vite mer? Ta gjerne kontakt via GitHub eller LinkedIn-profilen min - www.linkedin.com/in/anders-myhrvang-9a6629339.


