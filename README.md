# Drink Shop 🍹
 Drink shop con architettura 3 tier client-server-data. Android Java client + C Http Socket Server + Postgres DB.
 
 Il server è dockerizzato e ha due container:
 - postgres-db
 - server-c

prima di fare il *compose dei container* assicurarsi che il server non sia configurato in debug mode. Controllare che la stringa di connessione al database *PRODUCTION CONFIGURATION* non sia commentata.
 ```
 // Configurazione del file server.h
 
 // PRODUCTION CONFIGURATION 
#define DATABASE "host=postgres-db port=5432 dbname=drinks user=docker password=12345"

// DEBUG CONFIGURATION 
//#define DATABASE "host=127.0.0.1 port=42069 dbname=drinks user=docker password=12345"
 ```
 Nel caso fosse commentata basterà commentare la *DEBUG CONFIGURATION*, decommentare *PRODUCTION CONFIGURATION*, eseguire
 ```
# installare librerie se non presenti
# installa libreria postgres
RUN apt-get update && apt-get install -y libpq-dev
# installa libreria json
RUN apt-get install libjson-c-dev -y
# installa libreria jwt token
RUN sudo apt-get install libjwt-dev -y
# installa libreria bcrypt
sudo apt-get install libcrypt-dev -y

cd <directory server>

./build.sh  # compilo server
 
cd <root progetto>
# ! se hai una vecchia immagine del progetto in cache di docker eliminala
docker compose up
 ```
 # Features del server
 - Elabora richieste Http GET, POST, PUT, DELETE
 - Parsing di richieste Http con body application/json
 - Risposte Http con content application/json
 - Smart path routing con URL parameter (esempio: /user, /user/:id) 
 - Può includere files nella risposta Http
 - Jwt bearer token authentication
 - Password encryption
 - Models per interfacciamento con tabelle del database
 - Possibilità di eseguire custom middleware per ogni route
 - Handler per ogni route
 - Multithreading
 - Pool di connessioni con custom max clients  
 - Custom socket timeout
 - Gestione interrupt sigterm di terminazione (esempio CTRL+C)
 - Messaggi di stato del server sulle connessioni attive e eventi
 ### 404 Server custom response
 Quando il router non trova la route richiesta serve il client con un codice 404 e questa immagine:
![404](https://github.com/AlessandroBonomo28/DrinkShop/assets/75626033/0ce9c02a-5902-4b20-9742-b2f655e3f2d4)
 ## Come si potrebbe migliorare
 - IP spam filter
 - Incomplete request detection
 - Token expire in payload
 - Datetime helper class
 - check for mem leaks 💀
